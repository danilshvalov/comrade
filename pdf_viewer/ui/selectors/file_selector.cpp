#include "ui/selectors/file_selector.h"
#include "rapidfuzz_amalgamated.hpp"
#include "config.h"

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fts_fuzzy_match.h"
#undef FTS_FUZZY_MATCH_IMPLEMENTATION

FileSelector::FileSelector(
    std::function<void(std::string)> on_done, QWidget* parent, QString last_path
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

    QListView* list_view = dynamic_cast<QListView*>(get_view());
    list_view->setModel(list_model);

    if (list_model->rowCount() > 0) {
        list_view->setCurrentIndex(list_model->index(0, 0));
    }
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

    QListView* list_view = dynamic_cast<QListView*>(get_view());
    list_view->setModel(new_list_model);

    delete list_model;
    list_model = new_list_model;

    if (list_model->rowCount() > 0) {
        list_view->setCurrentIndex(list_model->index(0, 0));
    }

    return true;
}

QStringList FileSelector::get_dir_contents(QString root, QString prefix) {

    root = expand_home_dir(root);
    QDir directory(root);
    QStringList res = directory.entryList({prefix + "*"});
    if (res.size() == 0) {
        std::string encoded_prefix = prefix.toStdString();
        QStringList all_directory_files = directory.entryList();
        std::vector<std::pair<QString, int>> file_scores;

        for (auto file : all_directory_files) {
            std::string encoded_file = file.toStdString();
            int score = static_cast<int>(
                rapidfuzz::fuzz::partial_ratio(encoded_prefix, encoded_file)
            );
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
        on_done(full_path.toStdString());
        hide();
        parentWidget()->setFocus();
    } else {
        line_edit->setText(full_path + sep);
    }
}
