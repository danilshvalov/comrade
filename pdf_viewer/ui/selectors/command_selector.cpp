#include "ui/selectors/command_selector.h"
#include "rapidfuzz_amalgamated.hpp"
#include "utils.h"
#include "config.h"

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fts_fuzzy_match.h"
#undef FTS_FUZZY_MATCH_IMPLEMENTATION

#include <QTableView>
#include <QLineEdit>
#include <QHeaderView>

QList<QStandardItem*> CommandSelector::get_item(std::string command_name) {

    std::string command_key = "";

    if (key_map.find(command_name) != key_map.end()) {
        const std::vector<std::string>& command_keys = key_map[command_name];
        for (size_t i = 0; i < command_keys.size(); i++) {
            const std::string& ck = command_keys[i];
            if (i > 0) {
                command_key += " | ";
            }
            command_key += ck;
        }
    }
    QStandardItem* name_item =
        new QStandardItem(QString::fromStdString(command_name));
    QStandardItem* key_item =
        new QStandardItem(QString::fromStdString(command_key));
    key_item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    return (QList<QStandardItem*>() << name_item << key_item);
}

QStandardItemModel* CommandSelector::get_standard_item_model(
    std::vector<std::string> command_names
) {

    QStandardItemModel* res = new QStandardItemModel();

    for (size_t i = 0; i < command_names.size(); i++) {
        res->appendRow(get_item(command_names[i]));
    }
    return res;
}

QStandardItemModel* CommandSelector::get_standard_item_model(
    QStringList command_names
) {
    std::vector<std::string> std_command_names;

    for (int i = 0; i < command_names.size(); i++) {
        std_command_names.push_back(command_names.at(i).toStdString());
    }
    return get_standard_item_model(std_command_names);
}

QString CommandSelector::get_view_stylesheet_type_name() {
    return "QTableView";
}

void CommandSelector::on_select(const QModelIndex& index) {
    bool is_numeric = false;
    line_edit->text().toInt(&is_numeric);
    QString name = standard_item_model->data(index).toString();
    hide();
    parentWidget()->setFocus();
    if (!is_numeric) {
        (*on_done)(name.toStdString());
    } else {
        (*on_done)(line_edit->text().toStdString());
    }
}

CommandSelector::CommandSelector(
    std::function<void(std::string)>* on_done,
    QWidget* parent,
    QStringList elements,
    std::unordered_map<std::string, std::vector<std::string>> key_map
)
    : BaseSelector(nullptr, new QTableView(), parent),
      key_map(key_map),
      on_done(on_done) {
    string_elements = elements;
    standard_item_model = get_standard_item_model(string_elements);

    QTableView* table_view = dynamic_cast<QTableView*>(get_view());

    table_view->setSelectionMode(QAbstractItemView::SingleSelection);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_view->setModel(standard_item_model);

    table_view->horizontalHeader()->setStretchLastSection(true);
    table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_view->horizontalHeader()->hide();
    table_view->verticalHeader()->hide();
}

bool CommandSelector::on_text_change(const QString& text) {

    std::vector<std::string> matching_element_names;
    std::vector<int> scores;
    std::string search_text_string = text.toStdString();
    std::vector<std::pair<std::string, int>> match_score_pairs;

    for (int i = 0; i < string_elements.size(); i++) {
        std::string encoded = string_elements.at(i).toStdString();
        int score = static_cast<int>(
            rapidfuzz::fuzz::partial_ratio(search_text_string, encoded)
        );
        match_score_pairs.push_back(std::make_pair(encoded, score));
    }
    std::sort(
        match_score_pairs.begin(), match_score_pairs.end(),
        [](std::pair<std::string, int> lhs, std::pair<std::string, int> rhs) {
            return lhs.second > rhs.second;
        }
    );

    for (int i = 0; i < string_elements.size(); i++) {
        if (string_elements.at(i).startsWith(text)) {
            matching_element_names.push_back(string_elements.at(i).toStdString()
            );
        }
    }

    for (auto [command, score] : match_score_pairs) {
        if (score > 60 && (!QString::fromStdString(command).startsWith(text))) {
            matching_element_names.push_back(command);
        }
    }

    QStandardItemModel* new_standard_item_model =
        get_standard_item_model(matching_element_names);
    dynamic_cast<QTableView*>(get_view())->setModel(new_standard_item_model);
    delete standard_item_model;
    standard_item_model = new_standard_item_model;
    return true;
}
