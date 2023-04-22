#pragma once

#include "config.h"
#include "ui/selectors/base_selector.h"

#include <QTableView>
#include <QHeaderView>
#include <QStringListModel>

template <typename T>
class FilteredTableWindowSelector : public BaseSelector {
  public:
    FilteredTableWindowSelector(
        std::vector<std::wstring> std_string_list,
        std::vector<std::wstring> std_string_list_right,
        std::vector<T> values,
        int selected_index,
        std::function<void(T*)> on_done,
        QWidget* parent,
        std::function<void(T*)> on_delete_function = nullptr
    )
        : BaseSelector(nullptr, new QTableView(), parent),
          values(values),
          on_done(on_done),
          on_delete_function(on_delete_function) {
        item_strings = std_string_list;
        QVector<QString> q_string_list;
        for (const auto& s : std_string_list) {
            q_string_list.push_back(QString::fromStdWString(s));
        }

        QStandardItemModel* model = new QStandardItemModel();

        for (size_t i = 0; i < std_string_list.size(); i++) {
            QStandardItem* name_item =
                new QStandardItem(QString::fromStdWString(std_string_list[i]));
            QStandardItem* key_item = new QStandardItem(
                QString::fromStdWString(std_string_list_right[i])
            );
            key_item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
            model->appendRow(QList<QStandardItem*>() << name_item << key_item);
        }

        this->proxy_model->setSourceModel(model);

        QTableView* table_view = dynamic_cast<QTableView*>(this->get_view());

        if (selected_index != -1) {
            table_view->selectionModel()->setCurrentIndex(
                model->index(selected_index, 0),
                QItemSelectionModel::Rows | QItemSelectionModel::SelectCurrent
            );
        }

        table_view->setSelectionMode(QAbstractItemView::SingleSelection);
        table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
        table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

        if (std_string_list.size() > 0) {
            table_view->horizontalHeader()->setStretchLastSection(false);
            table_view->horizontalHeader()->setSectionResizeMode(
                0, QHeaderView::Stretch
            );
            table_view->horizontalHeader()->setSectionResizeMode(
                1, QHeaderView::ResizeToContents
            );
        }

        table_view->horizontalHeader()->hide();
        table_view->verticalHeader()->hide();

        if (Config::instance().MULTILINE_MENUS) {
            table_view->setWordWrap(true);
            table_view->verticalHeader()->setSectionResizeMode(
                QHeaderView::ResizeToContents
            );
        }

        if (selected_index != -1) {
            table_view->scrollTo(
                this->proxy_model->mapFromSource(table_view->currentIndex()),
                QAbstractItemView::EnsureVisible
            );
        }
    }

    QString get_view_stylesheet_type_name() override { return "QTableView"; }

    std::wstring get_selected_text() override {
        auto index = this->get_selected_index();

        if (index) {

            auto source_index = this->proxy_model->mapToSource(index.value());
            return item_strings[source_index.row()];
        }

        return L"";
    }

    virtual void on_delete(
        const QModelIndex& source_index, const QModelIndex& selected_index
    ) override {
        if (on_delete_function) {
            on_delete_function(&values[source_index.row()]);
            this->proxy_model->removeRow(selected_index.row());
            values.erase(values.begin() + source_index.row());
        }
    }

    void on_select(const QModelIndex& index) override {
        this->hide();
        this->parentWidget()->setFocus();
        auto source_index = this->proxy_model->mapToSource(index);
        on_done(&values[source_index.row()]);
    }

  private:
    QStringListModel* string_list_model = nullptr;
    std::vector<T> values;
    std::vector<std::wstring> item_strings;
    std::function<void(T*)> on_done = nullptr;
    std::function<void(T*)> on_delete_function = nullptr;
};
