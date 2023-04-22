#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <optional>
#include <utility>
#include <memory>
#include <filesystem>
#include <map>

#include <QApplication>
#include <QPushButton>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QWindow>
#include <QKeyEvent>
#include <QLineEdit>
#include <QTreeView>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QDateTime>
#include <QStackedWidget>
#include <QBoxLayout>
#include <QListView>
#include <QStringListModel>
#include <QLabel>
#include <QTextEdit>
#include <QFileSystemWatcher>

#include <QFontDatabase>
#include <QStandardItemModel>
#include <QScrollArea>
#include <QDesktopServices>
#include <QProcess>
#include <QStandardPaths>
#include <QCommandLineParser>
#include <QDir>
#include <QSurfaceFormat>

#include <mupdf/fitz.h>
#include "sqlite3.h"

#include "input.h"
#include "database.h"
#include "book.h"
#include "utils.h"
#include "ui.h"
#include "pdf_renderer.h"
#include "document.h"
#include "document_view.h"
#include "pdf_view_opengl_widget.h"
#include "config.h"
#include "utf8.h"
#include "main_widget.h"
#include "RunGuard.h"
#include "checksum.h"
#include "OpenWithApplication.h"
#include "new_file_checker.h"

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fts_fuzzy_match.h"
#undef FTS_FUZZY_MATCH_IMPLEMENTATION

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

// #define LINUX_STANDARD_PATHS

std::vector<MainWidget*> windows;

std::wstring strip_uri(std::wstring pdf_file_name) {

    if (pdf_file_name.size() > 1) {
        if ((pdf_file_name[0] == '"') &&
            (pdf_file_name[pdf_file_name.size() - 1] == '"')) {
            pdf_file_name = pdf_file_name.substr(1, pdf_file_name.size() - 2);
        }
        // support URIs like this: file:///home/user/file.pdf
        if (QString::fromStdWString(pdf_file_name).startsWith("file://")) {
            return pdf_file_name.substr(7, pdf_file_name.size() - 7);
        }
    }
    return pdf_file_name;
}

QStringList convert_arguments(QStringList input_args) {
    // convert the relative path of filename (if it exists) to absolute path

    QStringList output_args;

    // the first argument is always path of the executable
    output_args.push_back(input_args.at(0));
    input_args.pop_front();

    if (input_args.size() > 0) {
        QString path = input_args.at(0);

        bool is_path_argument = true;

        if (path.size() > 2 && path.startsWith("--")) {
            is_path_argument = false;
        }

        if (is_path_argument) {
            std::wstring path_wstring = strip_uri(path.toStdWString());
            fs::path path_object(path_wstring);
            output_args.push_back(
                QString::fromStdWString(path_object.generic_wstring())
            );
            input_args.pop_front();
        }
    }
    for (int i = 0; i < input_args.size(); i++) {
        output_args.push_back(input_args.at(i));
    }

    return output_args;
}

void configure_paths() {
    Config& config = Config::instance();

    fs::path& default_config_path = config.default_config_path;
    fs::path& default_keys_path = config.default_keys_path;
    std::vector<fs::path>& user_config_paths = config.user_config_paths;
    std::vector<fs::path>& user_keys_paths = config.user_keys_paths;
    fs::path& database_file_path = config.database_file_path;
    fs::path& local_database_file_path = config.local_database_file_path;
    fs::path& global_database_file_path = config.global_database_file_path;
    fs::path& tutorial_path = config.tutorial_path;
    fs::path& last_opened_file_address_path =
        config.last_opened_file_address_path;
    fs::path& shader_path = config.shader_path;
    fs::path& auto_config_path = config.auto_config_path;

    fs::path parent_path(QCoreApplication::applicationDirPath().toStdWString());
    std::string exe_path =
        utf8_encode(QCoreApplication::applicationFilePath().toStdWString());

    shader_path = parent_path / "shaders";

#ifdef Q_OS_MACOS
    fs::path mac_home_path(QDir::homePath().toStdWString());
    fs::path mac_standard_config_path = mac_home_path / ".config" / "sioyek";
    user_keys_paths.push_back(mac_standard_config_path / "keys_user.config");
    user_config_paths.push_back(mac_standard_config_path / "prefs_user.config");
#endif

#ifdef Q_OS_LINUX
    QStringList all_config_paths =
        QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    for (int i = all_config_paths.size() - 1; i >= 0; i--) {
        user_config_paths.push_back(
            fs::path(all_config_paths.at(i).toStdWString()) /
            "prefs_user.config"
        );
        user_keys_paths.push_back(
            fs::path(all_config_paths.at(i).toStdWString()) / "keys_user.config"
        );
    }

#ifdef LINUX_STANDARD_PATHS
    fs::path home_path(QDir::homePath().toStdWString());
    fs::path standard_data_path = home_path / ".local" / "share" / "sioyek";
    fs::path standard_config_path = "/etc/sioyek";
    fs::path read_only_data_path = "/usr/share/sioyek";
    fs::create_directories(standard_data_path);

    default_config_path = standard_config_path / "prefs.config";
    default_keys_path = standard_config_path / "keys.config";

    database_file_path = standard_data_path / "test.db";
    local_database_file_path = standard_data_path / "local.db";
    global_database_file_path = standard_data_path / "shared.db";
    tutorial_path = read_only_data_path / "tutorial.pdf";
    last_opened_file_address_path =
        standard_data_path / "last_document_path.txt";
    shader_path = read_only_data_path / "shaders";
#else
    char* APPDIR = std::getenv("XDG_CONFIG_HOME");
    fs::path linux_home_path = QDir::homePath().toStdWString();

    if (!APPDIR) {
        APPDIR = std::getenv("HOME");
    }

    fs::path standard_data_path = utf8_decode(APPDIR);
    standard_data_path = standard_data_path / ".local" / "share" / "Sioyek";
    fs::create_directories(standard_data_path);

    default_config_path = parent_path / "prefs.config";
    user_config_paths.push_back(standard_data_path / "prefs_user.config");
    default_keys_path = parent_path / "keys.config";
    user_keys_paths.push_back(standard_data_path / "keys_user.config");
    database_file_path = standard_data_path / "test.db";
    local_database_file_path = standard_data_path / "local.db";
    global_d tutorial_path = standard_data_path / "tutorial.pdf";
    last_opened_file_address_path =
        standard_data_path / "last_document_path.txt";

    fs::path linux_standard_config_path =
        linux_home_path / ".config" / "sioyek";
    // user_keys_paths.push_back(mac_standard_config_path.slash(L"keys_user.config"));
    // user_config_paths.push_back(mac_standard_config_path.slash(L"prefs_user.config"));
    user_keys_paths.push_back(linux_standard_config_path / "keys_user.config");
    user_config_paths.push_back(
        linux_standard_config_path / "prefs_user.config"
    );

    if (!fs::exists(tutorial_path)) {
        copy_file(parent_path / "tutorial.pdf", tutorial_path);
    }
#endif
#else // windows
#ifdef NDEBUG
    // install_app(exe_path.c_str());
#endif
    fs::path standard_data_path(
        QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)
            .at(0)
            .toStdWString()
    );
    QStringList all_config_paths =
        QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);

    fs::create_directories(standard_data_path);

    default_config_path = parent_path / "prefs.config";
    default_keys_path = parent_path / "keys.config";
    tutorial_path = parent_path / "tutorial.pdf";

#ifdef NON_PORTABLE
    user_config_paths.push_back(standard_data_path / "prefs_user.config");
    user_keys_paths.push_back(standard_data_path / "keys_user.config");
    for (int i = all_config_paths.size() - 1; i > 0; i--) {
        user_config_paths.push_back(
            fs::path(all_config_paths.at(i).toStdWString()) /
            "prefs_user.config"
        );
        user_keys_paths.push_back(
            fs::path(all_config_paths.at(i).toStdWString()) / "keys_user.config"
        );
    }
    database_file_path = standard_data_path / "test.db";
    local_database_file_path = standard_data_path / "local.db";
    global_database_file_path = standard_data_path / "shared.db";
    last_opened_file_address_path =
        standard_data_path / "last_document_path.txt";
#else
    user_config_paths.push_back(parent_path / "prefs_user.config");
    user_keys_paths.push_back(parent_path / "keys_user.config");
    database_file_path = parent_path / "test.db";
    local_database_file_path = parent_path / "local.db";
    global_database_file_path = parent_path / "shared.db";
    last_opened_file_address_path = parent_path / "last_document_path.txt";
#endif

#endif
    auto_config_path = standard_data_path / "auto.config";
    // user_config_paths.insert(user_config_paths.begin(), auto_config_path);
}

#undef CHECK_DIR_EXIST
#undef CHECK_FILE_EXIST

std::mutex mupdf_mutexes[FZ_LOCK_MAX];

void lock_mutex(void* user, int lock) {
    std::mutex* mut = (std::mutex*)user;
    (mut + lock)->lock();
}

void unlock_mutex(void* user, int lock) {
    std::mutex* mut = (std::mutex*)user;
    (mut + lock)->unlock();
}

void add_paths_to_file_system_watcher(
    QFileSystemWatcher& watcher,
    const fs::path& default_path,
    const std::vector<fs::path>& user_paths
) {
    if (fs::exists(default_path)) {
        watcher.addPath(QString::fromStdWString(default_path.generic_wstring())
        );
    }

    for (const auto& user_path : user_paths) {
        if (fs::exists(user_path)) {
            watcher.addPath(QString::fromStdWString(user_path.generic_wstring())
            );
        }
    }
}

MainWidget* get_window_with_opened_file_path(const std::wstring& file_path) {
    if (!QFile::exists(QString::fromStdWString(file_path))) {
        return nullptr;
    }

    if (file_path.size() > 0) {
        for (auto window : windows) {
            // if (window->doc() && window->doc()->get_path() == file_path) {

            if (window->doc()) {

#ifdef Q_OS_WIN
                std::wstring path1 = window->doc()->get_path();
                std::wstring path2 = file_path;
#else
                std::string path1 = utf8_encode(window->doc()->get_path());
                std::string path2 = utf8_encode(file_path);
#endif
                if (std::filesystem::equivalent(path1, path2)) {
                    return window;
                }
            }
        }
    }
    return nullptr;
}

std::optional<std::wstring> get_last_opened_file_name() {
    std::string file_path_;
    std::ifstream last_state_file(
        Config::instance().last_opened_file_address_path
    );
    std::getline(last_state_file, file_path_);
    last_state_file.close();

    if (file_path_.size() > 0) {
        return utf8_decode(file_path_);
    } else {
        return {};
    }
}

void invalidate_render() {
    for (auto window : windows) {
        window->invalidate_render();
    }
}

MainWidget* handle_args(const QStringList& arguments) {
    Config& config = Config::instance();
    std::optional<int> page = -1;
    std::optional<float> x_loc, y_loc;
    std::optional<float> zoom_level;

    std::vector<std::wstring> aarguments;
    for (size_t i = 0; i < arguments.size(); i++) {
        aarguments.push_back(arguments.at(i).toStdWString());
    }

    // todo: handle out of bounds error

    QCommandLineParser* parser = get_command_line_parser();

    if (!parser->parse(arguments)) {
        std::wcout << parser->errorText().toStdWString() << L"\n";
        return nullptr;
    }

    std::wstring pdf_file_name = L"";

    if (parser->positionalArguments().size() > 0) {
        pdf_file_name = parser->positionalArguments().at(0).toStdWString();
    } else if (windows[0]->doc() == nullptr) {
        // when no file is specified, and no current file is open, use the
        // last opened file or tutorial
        pdf_file_name = get_last_opened_file_name().value_or(
            config.tutorial_path.generic_wstring()
        );
    }

    std::optional<std::wstring> latex_file_name = {};
    std::optional<int> latex_line = {};
    std::optional<int> latex_column = {};

    if (parser->isSet("forward-search-file")) {
        latex_file_name = parser->value("forward-search-file").toStdWString();
    }

    if (parser->isSet("forward-search-line")) {
        latex_line = parser->value("forward-search-line").toInt();
    }

    if (parser->isSet("forward-search-column")) {
        latex_column = parser->value("forward-search-column").toInt();
    }

    if (parser->isSet("page")) {
        int page_int = parser->value("page").toInt();
        // 1 is the index for the first page (not 0)
        if (page_int > 0)
            page_int--;
        page = page_int;
    }

    if (parser->isSet("zoom")) {
        zoom_level = parser->value("zoom").toFloat();
    }

    if (parser->isSet("xloc")) {
        x_loc = parser->value("xloc").toFloat();
    }

    if (parser->isSet("yloc")) {
        y_loc = parser->value("yloc").toFloat();
    }

    pdf_file_name = strip_uri(pdf_file_name);

    if ((pdf_file_name.size() > 0) &&
        (!QFile::exists(QString::fromStdWString(pdf_file_name)))) {
        return nullptr;
    }

    MainWidget* target_window = get_window_with_opened_file_path(pdf_file_name);

    bool should_create_new_window = false;
    if (pdf_file_name.size() > 0) {
        if (parser->isSet("new-window")) {
            should_create_new_window = true;
        }
        if (config.application.launch_new_window &&
            (target_window == nullptr) && (!parser->isSet("reuse-window"))) {
            should_create_new_window = true;
        }
        if (windows[0]->doc() == nullptr) {
            should_create_new_window = false;
        }
    } else {
        if (parser->isSet("new-window")) {
            should_create_new_window = true;
        }
    }

    if (should_create_new_window) {
        target_window = new MainWidget(windows[0]);
        // target_window->run_multiple_commands(STARTUP_COMMANDS);
        target_window->command_manager
            ->create_macro_command("", config.STARTUP_COMMANDS)
            ->run(*target_window);
        target_window->apply_window_params_for_one_window_mode(true);
        target_window->show();
        windows.push_back(target_window);
    }
    if (target_window == nullptr) {
        target_window = windows[0];
    }

    if (parser->isSet("inverse-search") && target_window) {
        target_window->set_inverse_search_command(
            parser->value("inverse-search").toStdWString()
        );
        target_window->raise();
        // target_window->activateWindow();
    }
    if (parser->isSet("execute-command")) {
        QString command_string = parser->value("execute-command");
        QString command_data = parser->value("execute-command-data");
        if (command_data.size() > 0) {
            command_string += QString::fromStdString("(") + command_data +
                              QString::fromStdString(")");
        }

        auto command = target_window->command_manager->create_macro_command(
            "", command_string.toStdWString()
        );
        command->run(*target_window);
    }

    if (parser->isSet("focus-text")) {
        QString text = parser->value("focus-text");
        int page = parser->value("focus-text-page").toInt();
        target_window->focus_text(page, text.toStdWString());
    }

    // if no file is specified, use the previous file
    if (pdf_file_name == L"" && (windows[0]->doc() != nullptr)) {
        if (target_window->doc()) {
            pdf_file_name = target_window->doc()->get_path();
        } else {
            if (windows.size() > 1) {
                if (windows[windows.size() - 2]->doc()) {
                    pdf_file_name =
                        windows[windows.size() - 2]->doc()->get_path();
                }
            } else {
                pdf_file_name = config.tutorial_path.generic_wstring();
            }
        }
    }

    if (page != -1) {
        if (target_window) {
            target_window->push_state();
            target_window->open_document_at_location(
                pdf_file_name, page.value_or(0), x_loc, y_loc, zoom_level
            );
        }
    } else if (latex_file_name) {
        if (target_window) {
            target_window->do_synctex_forward_search(
                pdf_file_name, latex_file_name.value(), latex_line.value_or(0),
                latex_column.value_or(0)
            );
        }
    } else {
        target_window->push_state();
        target_window->open_document(pdf_file_name);
    }

    invalidate_render();

    delete parser;
    return target_window;
}

void focus_on_widget(QWidget* widget) {
    widget->activateWindow();
    widget->setWindowState(
        widget->windowState() & ~Qt::WindowMinimized | Qt::WindowActive
    );
}

int main(int argc, char* args[]) {
    Config& config = Config::instance();

    if (has_arg(argc, args, "--version")) {
        std::cout << "sioyek " << config.application.version << "\n";
        return 0;
    }

    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    OpenWithApplication app(argc, args);

    QCommandLineParser* parser = get_command_line_parser();
    parser->process(app.arguments());

    config.configure_paths();
    config.verify_config_paths();

    ConfigManager config_manager(
        config.default_config_path, config.auto_config_path,
        config.user_config_paths
    );
    CommandManager* command_manager = new CommandManager(&config_manager);

    if (config.SHARED_DATABASE_PATH.size() > 0) {
        config.global_database_file_path = config.SHARED_DATABASE_PATH;
    }
    char* shared_database_path_arg =
        get_argv_value(argc, args, "--shared-database-path");
    if (shared_database_path_arg) {
        config.global_database_file_path =
            utf8_decode(std::string(shared_database_path_arg));
    }

    config.verify_paths();

    RunGuard guard("sioyek");

    if (!guard.isPrimary()) {
        QStringList sent_args = convert_arguments(app.arguments());
        guard.sendMessage(serialize_string_array(sent_args));
        return 0;
    }

    QCoreApplication::setApplicationName(
        QString::fromStdWString(config.application.name)
    );
    QCoreApplication::setApplicationVersion(
        QString::fromStdString(config.application.version)
    );

    QStringList positional_args = parser->positionalArguments();
    delete parser;

    DatabaseManager db_manager;
    if (fs::exists(config.local_database_file_path) &&
        fs::exists(config.global_database_file_path)) {
        db_manager.open(
            config.local_database_file_path.generic_wstring(),
            config.global_database_file_path.generic_wstring()
        );
    } else {
        db_manager.open(
            config.database_file_path.generic_wstring(),
            config.database_file_path.generic_wstring()
        );
    }
    db_manager.ensure_database_compatibility(
        config.local_database_file_path.generic_wstring(),
        config.global_database_file_path.generic_wstring()
    );

    fz_locks_context locks;
    locks.user = mupdf_mutexes;
    locks.lock = lock_mutex;
    locks.unlock = unlock_mutex;

    fz_context* mupdf_context =
        fz_new_context(nullptr, &locks, FZ_STORE_DEFAULT);

    if (!mupdf_context) {
        std::cerr << "could not create mupdf context" << std::endl;
        return -1;
    }
    bool fail = false;
    fz_try(mupdf_context) { fz_register_document_handlers(mupdf_context); }
    fz_catch(mupdf_context) {
        std::cerr << "could not register document handlers" << std::endl;
        fail = true;
    }

    if (fail) {
        return -1;
    }

    bool quit = false;

    InputHandler input_handler(
        config.default_keys_path, config.user_keys_paths, command_manager
    );

    std::vector<std::pair<std::wstring, std::wstring>> prev_path_hash_pairs;
    db_manager.get_prev_path_hash_pairs(prev_path_hash_pairs);

    CachedChecksummer checksummer(&prev_path_hash_pairs);

    DocumentManager document_manager(mupdf_context, &db_manager, &checksummer);

    QFileSystemWatcher pref_file_watcher;
    add_paths_to_file_system_watcher(
        pref_file_watcher, config.default_config_path, config.user_config_paths
    );

    QFileSystemWatcher key_file_watcher;
    add_paths_to_file_system_watcher(
        key_file_watcher, config.default_keys_path, config.user_keys_paths
    );

    MainWidget* main_widget = new MainWidget(
        mupdf_context, &db_manager, &document_manager, &config_manager,
        command_manager, &input_handler, &checksummer, &quit
    );
    windows.push_back(main_widget);

    QString startup_commands_list =
        QString::fromStdWString(config.STARTUP_COMMANDS);
    QStringList startup_commands = startup_commands_list.split(";");
    NewFileChecker new_file_checker(config.PAPERS_FOLDER_PATH, main_widget);

    if (guard.isPrimary()) {
        QObject::connect(
            &guard, &RunGuard::messageReceived,
            [](const QByteArray& message) {
                QStringList args = deserialize_string_array(message);
                bool nofocus = args.indexOf("--nofocus") != -1;
                MainWidget* target = handle_args(args);
                if (!nofocus) {
                    if (target) {
                        // target->activateWindow();
                        focus_on_widget(target);
                    } else if (windows.size() > 0) {
                        // windows[0]->activateWindow();
                        focus_on_widget(windows[0]);
                    }
                }
            }
        );
    }

    main_widget->topLevelWidget()->resize(500, 500);

    if (config.START_WITH_HELPER_WINDOW &&
        (config.HELPER_WINDOW_SIZE[0] > -1)) {
        main_widget->apply_window_params_for_two_window_mode();
    } else {
        main_widget->apply_window_params_for_one_window_mode();
    }

    main_widget->show();

    handle_args(app.arguments());
    main_widget->command_manager
        ->create_macro_command("", config.STARTUP_COMMANDS)
        ->run(*main_widget);
    // main_widget->run_multiple_commands(STARTUP_COMMANDS);

    // load input file from `QFileOpenEvent` for macOS drag and drop & "open
    // with"
    QObject::connect(
        &app, &OpenWithApplication::file_ready,
        [](const QString& file_name) {
            handle_args(
                QStringList()
                << QCoreApplication::applicationFilePath() << file_name
            );
        }
    );

    // live reload the config files
    QObject::connect(
        &pref_file_watcher, &QFileSystemWatcher::fileChanged,
        [&]() {
            config_manager.deserialize(
                config.default_config_path, config.auto_config_path,
                config.user_config_paths
            );

            ConfigFileChangeListener::notify_config_file_changed(&config_manager
            );
            main_widget->validate_render();
            add_paths_to_file_system_watcher(
                pref_file_watcher, config.default_config_path,
                config.user_config_paths
            );
        }
    );

    QObject::connect(
        &key_file_watcher, &QFileSystemWatcher::fileChanged,
        [&]() {
            input_handler.reload_config_files(
                config.default_keys_path, config.user_keys_paths
            );
            add_paths_to_file_system_watcher(
                key_file_watcher, config.default_keys_path,
                config.user_keys_paths
            );
        }
    );

    app.exec();

    quit = true;

    std::vector<MainWidget*> windows_to_delete;
    for (size_t i = 0; i < windows.size(); i++) {
        windows_to_delete.push_back(windows[i]);
    }
    for (size_t i = 0; i < windows_to_delete.size(); i++) {
        delete windows_to_delete[i];
    }

    return 0;
}
