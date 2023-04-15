#pragma once

#include "ui/selectors/base_selector.h"

#include <unordered_map>
#include <functional>

class CommandSelector : public BaseSelector {
  public:
    CommandSelector(
        std::function<void(std::string)>* on_done,
        QWidget* parent,
        QStringList elements,
        std::unordered_map<std::string, std::vector<std::string>> key_map
    );

    QString get_view_stylesheet_type_name() override;

    void on_select(const QModelIndex& index) override;

    virtual bool on_text_change(const QString& text) override;

  private:
    QList<QStandardItem*> get_item(std::string command_name);

    QStandardItemModel* get_standard_item_model(
        std::vector<std::string> command_names
    );

    QStandardItemModel* get_standard_item_model(QStringList command_names);

    QStringList string_elements;
    QStandardItemModel* standard_item_model = nullptr;
    std::unordered_map<std::string, std::vector<std::string>> key_map;
    std::function<void(std::string)>* on_done = nullptr;
};
