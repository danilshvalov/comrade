#pragma once

#include "ui/selectors/base_selector.h"

class FilteredTreeSelector : public BaseSelector {
  public:
    FilteredTreeSelector(
        QStandardItemModel* item_model,
        std::function<void(const std::vector<int>&)> on_done,
        QWidget* parent,
        std::vector<int> selected_index
    );

    QString get_view_stylesheet_type_name() override;

    void on_select(const QModelIndex& index) override;

  private:
    std::function<void(const std::vector<int>&)> on_done;
};
