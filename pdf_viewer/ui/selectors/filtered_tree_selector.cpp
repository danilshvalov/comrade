#include "ui/selectors/filtered_tree_selector.h"
#include "config.h"

#include <QTreeView>
#include <QHeaderView>

QString FilteredTreeSelector::get_view_stylesheet_type_name() {
    return "QTreeView";
}

FilteredTreeSelector::FilteredTreeSelector(
    QStandardItemModel* item_model,
    std::function<void(const std::vector<int>&)> on_done,
    QWidget* parent,
    std::vector<int> selected_index
)
    : BaseSelector(item_model, new QTreeView(), parent),
      on_done(on_done) {
    auto index = QModelIndex();
    for (auto i : selected_index) {
        index = this->proxy_model->index(i, 0, index);
    }

    QTreeView* tree_view = dynamic_cast<QTreeView*>(this->get_view());
    if (Config::instance().SMALL_TOC) {
        tree_view->collapseAll();
        tree_view->expand(index);
    }

    tree_view->header()->setStretchLastSection(false);
    tree_view->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    tree_view->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tree_view->setCurrentIndex(index);
}

void FilteredTreeSelector::on_select(const QModelIndex& index) {
    this->hide();
    this->parentWidget()->setFocus();
    auto source_index = this->proxy_model->mapToSource(index);
    std::vector<int> indices;
    while (source_index != QModelIndex()) {
        indices.push_back(source_index.row());
        source_index = source_index.parent();
    }
    on_done(indices);
}
