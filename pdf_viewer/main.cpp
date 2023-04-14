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

std::wstring APPLICATION_NAME = L"sioyek";
std::string LOG_FILE_NAME = "sioyek_log.txt";
std::ofstream LOG_FILE;
int FONT_SIZE = -1;
int STATUS_BAR_FONT_SIZE = -1;
std::string APPLICATION_VERSION = "2.0.0";
float BACKGROUND_COLOR[3] = {1.0f, 1.0f, 1.0f};
float DARK_MODE_BACKGROUND_COLOR[3] = {0.0f, 0.0f, 0.0f};
float CUSTOM_COLOR_MODE_EMPTY_BACKGROUND_COLOR[3] = {0.0f, 0.0f, 0.0f};
float CUSTOM_BACKGROUND_COLOR[3] = {1.0f, 1.0f, 1.0f};
float CUSTOM_TEXT_COLOR[3] = {0.0f, 0.0f, 0.0f};
float STATUS_BAR_COLOR[3] = {0.0f, 0.0f, 0.0f};
float STATUS_BAR_TEXT_COLOR[3] = {1.0f, 1.0f, 1.0f};
float UI_SELECTED_TEXT_COLOR[3] = {0.0f, 0.0f, 0.0f};
float UI_SELECTED_BACKGROUND_COLOR[3] = {1.0f, 1.0f, 1.0f};
float UNSELECTED_SEARCH_HIGHLIGHT_COLOR[3] = {0.0f, 0.5f, 0.5f};
float GAMMA = 1.0f;
bool DEBUG = false;

std::wstring SEARCH_URLS[26];
std::wstring EXECUTE_COMMANDS[26];
std::wstring TEXT_HIGHLIGHT_URL = L"http://localhost:5000/";
std::wstring MIDDLE_CLICK_SEARCH_ENGINE = L"s";
std::wstring SHIFT_MIDDLE_CLICK_SEARCH_ENGINE = L"l";
std::wstring PAPERS_FOLDER_PATH = L"";
std::wstring STATUS_BAR_FORMAT =
    L"Page %{current_page} / "
    L"%{num_pages}%{chapter_name}%{search_results}%{search_progress}%{link_"
    L"status}%{waiting_for_symbol}%{indexing}%{preview_index}%{synctex}%{drag}%"
    L"{presentation}%{visual_scroll}%{locked_scroll}%{highlight}%{closest_"
    L"bookmark}%{close_portal}%{rect_select}%{custom_message}";

float HIGHLIGHT_COLORS[26 * 3] = {
    0.5182963463943647,  0.052279561076773784, 0.42942409252886155,
    0.673198309637537,   0.14250443697242887,  0.1842972533900342,
    0.1259143196334698,  0.3472546716690144,   0.5268310086975159,
    0.44867634475259244, 0.36152631940627494,  0.18979733584113254,
    0.25561951195738114, 0.5203940586174391,   0.2239864294251798,
    0.46620566366115457, 0.34950449396122557,  0.18428984237761986,
    0.7766958121833649,  0.18529941752256116,  0.03800477029407397,
    0.14245148690647982, 0.27376105738246703,  0.5837874557110532,
    0.15069695522822338, 0.6757965126090706,   0.173506532162706,
    0.20214309005349734, 0.388109281902417,    0.40974762804408554,
    0.5282008406153603,  0.3604221142506678,   0.11137704513397183,
    0.11065494801726693, 0.43355028291683534,  0.4557947690658978,
    0.4623270941397442,  0.2575781303014751,   0.28009477555878065,
    0.13682260642246874, 0.843494092757017,    0.019683300820514175,
    0.3779898334061099,  0.10067511592122631,  0.5213350506726637,
    0.20252688176577896, 0.46636886381356,     0.33110425442066094,
    0.26429496078170356, 0.4214065241882322,   0.31429851503006423,
    0.2778665356071555,  0.31938061671537193,  0.40275284767747266,
    0.2859415758796114,  0.3778585576392479,   0.33619986648114064,
    0.06881479216543497, 0.49813975498043,     0.43304545285413504,
    0.5411102077276201,  0.050950286432382155, 0.4079395058399978,
    0.13956877643913856, 0.4133573589812949,   0.44707386457956655,
    0.5672781038824454,  0.026174925202518497, 0.4065469709150361,
    0.33594461744136966, 0.30463905854351836,  0.359416324015112,
    0.16837764593670387, 0.43225375356473283,  0.3993686004985632,
    0.21290269578043475, 0.5704883842115632,   0.21660892000800203,
};

float DARK_MODE_CONTRAST = 0.8f;
float ZOOM_INC_FACTOR = 1.2f;
float VERTICAL_MOVE_AMOUNT = 1.0f;
float HORIZONTAL_MOVE_AMOUNT = 1.0f;
float MOVE_SCREEN_PERCENTAGE = 0.8f;
const unsigned int CACHE_INVALID_MILIES = 1000;
const int PERSIST_MILIES = 1000 * 60;
const int PAGE_PADDINGS = 0;
const int MAX_PENDING_REQUESTS = 31;
bool FLAT_TABLE_OF_CONTENTS = false;
bool SHOULD_USE_MULTIPLE_MONITORS = false;
bool SHOULD_CHECK_FOR_LATEST_VERSION_ON_STARTUP = true;
bool DEFAULT_DARK_MODE = false;
bool SORT_BOOKMARKS_BY_LOCATION = false;
std::wstring LIBGEN_ADDRESS = L"";
std::wstring GOOGLE_SCHOLAR_ADDRESS = L"";
std::wstring INVERSE_SEARCH_COMMAND = L"";
std::wstring SHARED_DATABASE_PATH = L"";
std::wstring UI_FONT_FACE_NAME = L"";
std::wstring DEFAULT_OPEN_FILE_PATH = L"";
bool SHOULD_LOAD_TUTORIAL_WHEN_NO_OTHER_FILE = false;
bool SHOULD_LAUNCH_NEW_INSTANCE = false;
bool SHOULD_LAUNCH_NEW_WINDOW = false;
bool SHOULD_DRAW_UNRENDERED_PAGES = true;
bool HOVER_OVERVIEW = false;
bool RERENDER_OVERVIEW = false;
bool LINEAR_TEXTURE_FILTERING = false;
bool RULER_MODE = false;
bool SMALL_TOC = false;
bool WHEEL_ZOOM_ON_CURSOR = false;
bool TEXT_SUMMARY_HIGHLIGHT_SHOULD_REFINE = true;
bool TEXT_SUMMARY_HIGHLIGHT_SHOULD_FILL = true;
bool USE_HEURISTIC_IF_TEXT_SUMMARY_NOT_AVAILABLE = false;
int TEXT_SUMMARY_CONTEXT_SIZE = 49;
float VISUAL_MARK_NEXT_PAGE_FRACTION = 0.25f;
float VISUAL_MARK_NEXT_PAGE_THRESHOLD = 0.1f;
float RULER_PADDING = 0.0f;
float RULER_X_PADDING = 0.0f;
std::wstring ITEM_LIST_PREFIX = L">";
std::wstring STARTUP_COMMANDS = L"";
float SMALL_PIXMAP_SCALE = 0.75f;
float DISPLAY_RESOLUTION_SCALE = -1;
float FIT_TO_PAGE_WIDTH_RATIO = 1;
int MAIN_WINDOW_SIZE[2] = {-1, -1};
int HELPER_WINDOW_SIZE[2] = {-1, -1};
int MAIN_WINDOW_MOVE[2] = {-1, -1};
int HELPER_WINDOW_MOVE[2] = {-1, -1};
float TOUCHPAD_SENSITIVITY = 1.0f;
int SINGLE_MAIN_WINDOW_SIZE[2] = {-1, -1};
int SINGLE_MAIN_WINDOW_MOVE[2] = {-1, -1};
bool ENABLE_EXPERIMENTAL_FEATURES = false;
bool CREATE_TABLE_OF_CONTENTS_IF_NOT_EXISTS = true;
int MAX_CREATED_TABLE_OF_CONTENTS_SIZE = 5000;
bool FORCE_CUSTOM_LINE_ALGORITHM = false;
float OVERVIEW_SIZE[2] = {0.8f, 0.4f};
float OVERVIEW_OFFSET[2] = {0.0f, 0.0f};
bool IGNORE_WHITESPACE_IN_PRESENTATION_MODE = false;
bool EXACT_HIGHLIGHT_SELECT = false;
bool SHOW_DOC_PATH = false;
float FASTREAD_OPACITY = 0.5f;
bool SHOULD_WARN_ABOUT_USER_KEY_OVERRIDE = true;
bool SINGLE_CLICK_SELECTS_WORDS = false;
bool USE_LEGACY_KEYBINDS = true;
bool MULTILINE_MENUS = false;
bool START_WITH_HELPER_WINDOW = false;
std::map<std::wstring, std::wstring> ADDITIONAL_COMMANDS;
std::map<std::wstring, std::wstring> ADDITIONAL_MACROS;
bool PRERENDER_NEXT_PAGE = false;
bool EMACS_MODE = false;
bool HIGHLIGHT_MIDDLE_CLICK = false;
float HYPERDRIVE_SPEED_FACTOR = 10.0f;
float SMOOTH_SCROLL_SPEED = 3.0f;
float SMOOTH_SCROLL_DRAG = 3000.0f;
int PRERENDERED_PAGE_COUNT = 0;

float PAGE_SEPARATOR_WIDTH = 0.0f;
float PAGE_SEPARATOR_COLOR[3] = {0.9f, 0.9f, 0.9f};
bool IGNORE_STATUSBAR_IN_PRESENTATION_MODE = false;
bool SUPER_FAST_SEARCH = false;
bool SHOW_CLOSEST_BOOKMARK_IN_STATUSBAR = false;
bool SHOW_CLOSE_PORTAL_IN_STATUSBAR = false;
bool CASE_SENSITIVE_SEARCH = true;
bool SHOW_DOCUMENT_NAME_IN_STATUSBAR = false;
bool NUMERIC_TAGS = false;
bool SHOULD_HIGHLIGHT_LINKS = false;
bool SHOULD_HIGHLIGHT_UNSELECTED_SEARCH = false;
int KEYBOARD_SELECT_FONT_SIZE = 20;
bool FUZZY_SEARCHING = false;
bool INVERTED_HORIZONTAL_SCROLLING = false;
bool TOC_JUMP_ALIGN_TOP = false;
float CUSTOM_COLOR_CONTRAST = 0.5f;
float HIGHLIGHT_DELETE_THRESHOLD = 0.01f;
float SCROLL_VIEW_SENSITIVITY = 1.0f;
float KEYBOARD_SELECT_BACKGROUND_COLOR[] = {0.9f, 0.75f, 0.0f, 1.0f};
float KEYBOARD_SELECT_TEXT_COLOR[] = {0.0f, 0.0f, 0.5f, 1.0f};
bool AUTOCENTER_VISUAL_SCROLL = false;
bool ALPHABETIC_LINK_TAGS = false;
bool VIMTEX_WSL_FIX = false;

fs::path default_config_path(L"");
fs::path default_keys_path(L"");
std::vector<fs::path> user_config_paths = {};
std::vector<fs::path> user_keys_paths = {};
fs::path database_file_path(L"");
fs::path local_database_file_path(L"");
fs::path global_database_file_path(L"");
fs::path tutorial_path(L"");
fs::path last_opened_file_address_path(L"");
fs::path shader_path(L"");
fs::path auto_config_path(L"");

std::wstring SHIFT_CLICK_COMMAND = L"overview_under_cursor";
std::wstring CONTROL_CLICK_COMMAND = L"smart_jump_under_cursor";
std::wstring SHIFT_RIGHT_CLICK_COMMAND = L"";
std::wstring CONTROL_RIGHT_CLICK_COMMAND = L"";
std::wstring ALT_CLICK_COMMAND = L"";
std::wstring ALT_RIGHT_CLICK_COMMAND = L"";

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
    global_database_file_path = standard_data_path / "shared.db";
    tutorial_path = standard_data_path / "tutorial.pdf";
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

void verify_config_paths() {
    // TODO: remove macros
#define CHECK_DIR_EXIST(path)                                                  \
    do {                                                                       \
        if (!fs::exists(path))                                                 \
            std::wcout << L"Error: " << #path << ": " << path                  \
                       << L" doesn't exist!\n";                                \
    } while (false)
#define CHECK_FILE_EXIST(path)                                                 \
    do {                                                                       \
        if (!fs::exists(path))                                                 \
            std::wcout << L"Error: " << #path << ": " << path                  \
                       << L" doesn't exist!\n";                                \
    } while (false)

    std::wcout << L"default_config_path: " << default_config_path << L"\n";
    CHECK_FILE_EXIST(default_config_path);
    std::wcout << L"default_keys_path: " << default_keys_path << L"\n";
    CHECK_FILE_EXIST(default_keys_path);
    for (size_t i = 0; i < user_config_paths.size(); i++) {
        std::wcout << L"user_config_path: [ " << i << " ] "
                   << user_config_paths[i] << L"\n";
    }
    for (size_t i = 0; i < user_keys_paths.size(); i++) {
        std::wcout << L"user_keys_path: [ " << i << " ] " << user_keys_paths[i]
                   << L"\n";
    }
}

void verify_paths() {
    std::wcout << L"database_file_path: " << database_file_path << L"\n";
    std::wcout << L"local_database_file_path: " << local_database_file_path
               << L"\n";
    std::wcout << L"global_database_file_path: " << global_database_file_path
               << L"\n";
    std::wcout << L"tutorial_path: " << tutorial_path << L"\n";
    std::wcout << L"last_opened_file_address_path: "
               << last_opened_file_address_path << L"\n";
    std::wcout << L"shader_path: " << shader_path << L"\n";
    CHECK_DIR_EXIST(shader_path);
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
    std::ifstream last_state_file(last_opened_file_address_path);
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
        pdf_file_name =
            get_last_opened_file_name().value_or(tutorial_path.generic_wstring()
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
        if (SHOULD_LAUNCH_NEW_WINDOW && (target_window == nullptr) &&
            (!parser->isSet("reuse-window"))) {
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
            ->create_macro_command("", STARTUP_COMMANDS)
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
                pdf_file_name = tutorial_path.generic_wstring();
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

    if (has_arg(argc, args, "--version")) {
        std::cout << "sioyek " << APPLICATION_VERSION << "\n";
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

    configure_paths();
    verify_config_paths();

    ConfigManager config_manager(
        default_config_path, auto_config_path, user_config_paths
    );
    CommandManager* command_manager = new CommandManager(&config_manager);

    if (SHARED_DATABASE_PATH.size() > 0) {
        global_database_file_path = SHARED_DATABASE_PATH;
    }
    char* shared_database_path_arg =
        get_argv_value(argc, args, "--shared-database-path");
    if (shared_database_path_arg) {
        global_database_file_path =
            utf8_decode(std::string(shared_database_path_arg));
    }

    verify_paths();

    RunGuard guard("sioyek");

    if (!guard.isPrimary()) {
        QStringList sent_args = convert_arguments(app.arguments());
        guard.sendMessage(serialize_string_array(sent_args));
        return 0;
    }

    QCoreApplication::setApplicationName(
        QString::fromStdWString(APPLICATION_NAME)
    );
    QCoreApplication::setApplicationVersion(
        QString::fromStdString(APPLICATION_VERSION)
    );

    QStringList positional_args = parser->positionalArguments();
    delete parser;

    DatabaseManager db_manager;
    if (fs::exists(local_database_file_path) &&
        fs::exists(global_database_file_path)) {
        db_manager.open(
            local_database_file_path.generic_wstring(),
            global_database_file_path.generic_wstring()
        );
    } else {
        db_manager.open(
            database_file_path.generic_wstring(),
            database_file_path.generic_wstring()
        );
    }
    db_manager.ensure_database_compatibility(
        local_database_file_path.generic_wstring(),
        global_database_file_path.generic_wstring()
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
        default_keys_path, user_keys_paths, command_manager
    );

    std::vector<std::pair<std::wstring, std::wstring>> prev_path_hash_pairs;
    db_manager.get_prev_path_hash_pairs(prev_path_hash_pairs);

    CachedChecksummer checksummer(&prev_path_hash_pairs);

    DocumentManager document_manager(mupdf_context, &db_manager, &checksummer);

    QFileSystemWatcher pref_file_watcher;
    add_paths_to_file_system_watcher(
        pref_file_watcher, default_config_path, user_config_paths
    );

    QFileSystemWatcher key_file_watcher;
    add_paths_to_file_system_watcher(
        key_file_watcher, default_keys_path, user_keys_paths
    );

    MainWidget* main_widget = new MainWidget(
        mupdf_context, &db_manager, &document_manager, &config_manager,
        command_manager, &input_handler, &checksummer, &quit
    );
    windows.push_back(main_widget);

    if (DEFAULT_DARK_MODE) {
        main_widget->toggle_dark_mode();
    }

    QString startup_commands_list = QString::fromStdWString(STARTUP_COMMANDS);
    QStringList startup_commands = startup_commands_list.split(";");
    NewFileChecker new_file_checker(PAPERS_FOLDER_PATH, main_widget);

    if (guard.isPrimary()) {
        QObject::connect(
            &guard, &RunGuard::messageReceived,
            [&main_widget](const QByteArray& message) {
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

    if (START_WITH_HELPER_WINDOW && (HELPER_WINDOW_SIZE[0] > -1)) {
        main_widget->apply_window_params_for_two_window_mode();
    } else {
        main_widget->apply_window_params_for_one_window_mode();
    }

    main_widget->show();

    handle_args(app.arguments());
    main_widget->command_manager->create_macro_command("", STARTUP_COMMANDS)
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
                default_config_path, auto_config_path, user_config_paths
            );

            ConfigFileChangeListener::notify_config_file_changed(&config_manager
            );
            main_widget->validate_render();
            add_paths_to_file_system_watcher(
                pref_file_watcher, default_config_path, user_config_paths
            );
        }
    );

    QObject::connect(
        &key_file_watcher, &QFileSystemWatcher::fileChanged,
        [&]() {
            input_handler.reload_config_files(
                default_keys_path, user_keys_paths
            );
            add_paths_to_file_system_watcher(
                key_file_watcher, default_keys_path, user_keys_paths
            );
        }
    );

    if (SHOULD_CHECK_FOR_LATEST_VERSION_ON_STARTUP) {
        check_for_updates(main_widget, APPLICATION_VERSION);
    }

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
