#pragma once

#include "ui/selectors/base_selector.h"

#include <QListView>
#include <QStringListModel>

template <typename T>
class FilteredWindowSelector : public BaseSelector {
  public:
    FilteredWindowSelector(
        std::vector<std::wstring> std_string_list,
        std::vector<T> values,
        std::function<void(T*)> on_done,
        QWidget* parent,
        std::function<void(T*)> on_delete_function = nullptr
    )
        : BaseSelector(nullptr, new QListView(), parent),
          values(values),
          on_done(on_done),
          on_delete_function(on_delete_function) {
        QVector<QString> q_string_list;
        for (const auto& s : std_string_list) {
            q_string_list.push_back(QString::fromStdWString(s));
        }
        QStringList string_list = QStringList::fromVector(q_string_list);

        string_list_model = new QStringListModel(string_list);
        this->proxy_model->setSourceModel(string_list_model);
    }

    QString get_view_stylesheet_type_name() override { return "QListView"; }

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
    std::function<void(T*)> on_done = nullptr;
    std::function<void(T*)> on_delete_function = nullptr;
};
