#include "ui/selectors/file_selector.h"
#include "rapidfuzz_amalgamated.hpp"

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fts_fuzzy_match.h"
#undef FTS_FUZZY_MATCH_IMPLEMENTATION

extern bool FUZZY_SEARCHING;

FileSelector::FileSelector(
    std::function<void(std::wstring)> on_done,
    QWidget* parent,
    QString last_path
)
    : BaseSelector(nullptr, new QListView(), parent),
      on_done(on_done) {

    QString root_path;

    if (last_path.size() > 0) {
        QString file_name;
        split_root_file(last_path, root_path, file_name);
        root_path += QDir::separator();
    }

    list_model = new QStringListModel(get_dir_contents(root_path, ""));
    last_root = root_path;
    line_edit->setText(last_root);

    dynamic_cast<QListView*>(get_view())->setModel(list_model);
}

QString FileSelector::get_view_stylesheet_type_name() { return "QListView"; }

bool FileSelector::on_text_change(const QString& text) {
    QString root_path;
    QString partial_name;
    split_root_file(text, root_path, partial_name);

    last_root = root_path;
    if (last_root.size() > 0) {
        if (last_root.at(last_root.size() - 1) == QDir::separator()) {
            last_root.chop(1);
        }
    }

    QStringList match_list = get_dir_contents(root_path, partial_name);
    QStringListModel* new_list_model = new QStringListModel(match_list);
    dynamic_cast<QListView*>(get_view())->setModel(new_list_model);
    delete list_model;
    list_model = new_list_model;
    return true;
}

QStringList FileSelector::get_dir_contents(QString root, QString prefix) {

    root = expand_home_dir(root);
    QDir directory(root);
    QStringList res = directory.entryList({prefix + "*"});
    if (res.size() == 0) {
        std::string encoded_prefix = utf8_encode(prefix.toStdWString());
        QStringList all_directory_files = directory.entryList();
        std::vector<std::pair<QString, int>> file_scores;

        for (auto file : all_directory_files) {
            std::string encoded_file = utf8_encode(file.toStdWString());
            int score = 0;
            if (FUZZY_SEARCHING) {
                score = static_cast<int>(
                    rapidfuzz::fuzz::partial_ratio(encoded_prefix, encoded_file)
                );
            } else {
                fts::fuzzy_match(
                    encoded_prefix.c_str(), encoded_file.c_str(), score
                );
            }
            file_scores.push_back(std::make_pair(file, score));
        }
        std::sort(
            file_scores.begin(), file_scores.end(),
            [](std::pair<QString, int> lhs, std::pair<QString, int> rhs) {
                return lhs.second > rhs.second;
            }
        );
        for (auto [file, score] : file_scores) {
            if (score > 0) {
                res.push_back(file);
            }
        }
    }
    return res;
}

void FileSelector::on_select(const QModelIndex& index) {
    QString name = list_model->data(index).toString();
    QChar sep = QDir::separator();
    QString full_path = expand_home_dir(
        (last_root.size() > 0) ? (last_root + sep + name) : name
    );

    if (QFileInfo(full_path).isFile()) {
        on_done(full_path.toStdWString());
        hide();
        parentWidget()->setFocus();
    } else {
        line_edit->setText(full_path + sep);
    }
}
