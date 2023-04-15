#include "ui/selectors/base_selector.h"
#include "utils.h"
#include "rapidfuzz_amalgamated.hpp"

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fts_fuzzy_match.h"
#undef FTS_FUZZY_MATCH_IMPLEMENTATION

#include <QVBoxLayout>
#include <QTreeView>
#include <QHeaderView>
#include <QLineEdit>
#include <QKeyEvent>
#include <QCoreApplication>

extern bool EMACS_MODE;
extern bool FUZZY_SEARCHING;
extern int FONT_SIZE;

BaseSelector::BaseSelector(
    QStandardItemModel* item_model,
    QAbstractItemView* abstract_item_view,
    QWidget* parent
)
    : QWidget(parent),
      abstract_item_view(abstract_item_view),
      proxy_model(new MySortFilterProxyModel()) {

    proxy_model->setFilterCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);

    if (item_model) {
        proxy_model->setSourceModel(item_model);
    }

    resize(300, 800);
    QVBoxLayout* layout = new QVBoxLayout;
    setLayout(layout);

    line_edit = new QLineEdit();
    abstract_item_view->setModel(proxy_model);
    abstract_item_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QTreeView* tree_view = dynamic_cast<QTreeView*>(abstract_item_view);
    if (tree_view) {
        tree_view->expandAll();
        tree_view->setHeaderHidden(true);
        tree_view->resizeColumnToContents(0);
        tree_view->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    }
    if (proxy_model) {
        proxy_model->setRecursiveFilteringEnabled(true);
    }

    layout->addWidget(line_edit);
    layout->addWidget(abstract_item_view);

    line_edit->installEventFilter(this);
    line_edit->setFocus();

    QObject::connect(
        abstract_item_view, &QAbstractItemView::activated,
        [&](const QModelIndex& index) { on_select(index); }
    );

    QObject::connect(
        line_edit, &QLineEdit::textChanged,
        [&](const QString& text) {
            if (!on_text_change(text)) {
                // generic text change handling when we don't explicitly
                // handle text change events
                // proxy_model->setFilterFixedString(text);
                proxy_model->setFilterCustom(text);
                QTreeView* t_view = dynamic_cast<QTreeView*>(get_view());
                if (t_view) {
                    t_view->expandAll();
                }
            }
        }
    );
}

bool BaseSelector::eventFilter(QObject* obj, QEvent* event) {
    if (obj == line_edit) {
        if (event->type() == QEvent::KeyRelease) {
            QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
            if (key_event->key() == Qt::Key_Delete) {
                handle_delete();
            }
        }
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
            bool is_control_pressed =
                key_event->modifiers().testFlag(Qt::ControlModifier) ||
                key_event->modifiers().testFlag(Qt::MetaModifier);
            bool is_alt_pressed =
                key_event->modifiers().testFlag(Qt::AltModifier);

            if (key_event->key() == Qt::Key_Down ||
                key_event->key() == Qt::Key_Up ||
                key_event->key() == Qt::Key_Left ||
                key_event->key() == Qt::Key_Right) {
                QKeyEvent* newEvent = key_event->clone();
                QCoreApplication::postEvent(get_view(), newEvent);
                // QCoreApplication::postEvent(tree_view, key_event);
                return true;
            }
            if (key_event->key() == Qt::Key_Tab) {
                QKeyEvent* new_key_event = new QKeyEvent(
                    key_event->type(), Qt::Key_Down, key_event->modifiers()
                );
                QCoreApplication::postEvent(get_view(), new_key_event);
                return true;
            }
            if (EMACS_MODE) {
                if (((key_event->key() == Qt::Key_V)) && is_control_pressed) {
                    QKeyEvent* new_key_event = new QKeyEvent(
                        key_event->type(), Qt::Key_Up, key_event->modifiers()
                    );
                    QCoreApplication::postEvent(get_view(), new_key_event);
                    return true;
                }
                if (((key_event->key() == Qt::Key_V)) && is_alt_pressed) {
                    QKeyEvent* new_key_event = new QKeyEvent(
                        key_event->type(), Qt::Key_Down, key_event->modifiers()
                    );
                    QCoreApplication::postEvent(get_view(), new_key_event);
                    return true;
                }
            }
            if (((key_event->key() == Qt::Key_N) ||
                 (key_event->key() == Qt::Key_J)) &&
                is_control_pressed) {
                QKeyEvent* new_key_event = new QKeyEvent(
                    key_event->type(), Qt::Key_Down, key_event->modifiers()
                );
                QCoreApplication::postEvent(get_view(), new_key_event);
                return true;
            }
            if (((key_event->key() == Qt::Key_P) ||
                 (key_event->key() == Qt::Key_K)) &&
                is_control_pressed) {
                QKeyEvent* new_key_event = new QKeyEvent(
                    key_event->type(), Qt::Key_Up, key_event->modifiers()
                );
                QCoreApplication::postEvent(get_view(), new_key_event);
                return true;
            }
            if ((key_event->key() == Qt::Key_J) && is_alt_pressed) {
                QKeyEvent* new_key_event = new QKeyEvent(
                    key_event->type(), Qt::Key_End,
                    Qt::KeyboardModifier::NoModifier
                );
                QCoreApplication::postEvent(get_view(), new_key_event);
                return true;
            }
            if ((key_event->key() == Qt::Key_K) && is_alt_pressed) {
                QKeyEvent* new_key_event = new QKeyEvent(
                    key_event->type(), Qt::Key_Home,
                    Qt::KeyboardModifier::NoModifier
                );
                QCoreApplication::postEvent(get_view(), new_key_event);
                return true;
            }
            if ((key_event->key() == Qt::Key_PageDown)) {
                QKeyEvent* new_key_event = new QKeyEvent(
                    key_event->type(), Qt::Key_PageDown, key_event->modifiers()
                );
                QCoreApplication::postEvent(get_view(), new_key_event);
                return true;
            }
            if ((key_event->key() == Qt::Key_PageUp)) {
                QKeyEvent* new_key_event = new QKeyEvent(
                    key_event->type(), Qt::Key_PageUp, key_event->modifiers()
                );
                QCoreApplication::postEvent(get_view(), new_key_event);
                return true;
            }
            if (key_event->key() == Qt::Key_Backtab) {
                QKeyEvent* new_key_event = new QKeyEvent(
                    key_event->type(), Qt::Key_Up, key_event->modifiers()
                );
                QCoreApplication::postEvent(get_view(), new_key_event);
                return true;
            }
            if (((key_event->key() == Qt::Key_C) && is_control_pressed)) {
                std::wstring text = get_selected_text();
                if (text.size() > 0) {
                    copy_to_clipboard(text);
                }
                return true;
            }
            if (key_event->key() == Qt::Key_Return ||
                key_event->key() == Qt::Key_Enter) {
                std::optional<QModelIndex> selected_index =
                    get_selected_index();
                if (selected_index) {
                    on_select(selected_index.value());
                } else {
                    on_return_no_select(line_edit->text());
                }
                return true;
            }
        }
    }
    return false;
}

void BaseSelector::handle_delete() {
    QModelIndexList selected_index_list =
        get_view()->selectionModel()->selectedIndexes();
    if (selected_index_list.size() > 0) {
        QModelIndex selected_index = selected_index_list.at(0);
        if (proxy_model->hasIndex(
                selected_index.row(), selected_index.column()
            )) {
            QModelIndex source_index = proxy_model->mapToSource(selected_index);
            on_delete(source_index, selected_index);
        }
    }
}

void BaseSelector::on_config_file_changed() {
    QString font_size_stylesheet = "";
    if (FONT_SIZE > 0) {
        font_size_stylesheet = QString("font-size: %1px").arg(FONT_SIZE);
    }

    std::wstring ss =
        (get_status_stylesheet(true) + font_size_stylesheet).toStdWString();
    setStyleSheet(get_status_stylesheet(true) + font_size_stylesheet);
    get_view()->setStyleSheet(
        get_view_stylesheet_type_name() + "::item::selected{" +
        get_selected_stylesheet() + "}"
    );
}

void BaseSelector::resizeEvent(QResizeEvent* resize_event) {
    QWidget::resizeEvent(resize_event);
    int parent_width = parentWidget()->width();
    int parent_height = parentWidget()->height();
    setFixedSize(parent_width * 0.9f, parent_height * 0.3);
    move(parent_width * 0.05f, 0);
    on_config_file_changed();
}

std::optional<QModelIndex> BaseSelector::get_selected_index() {
    QModelIndexList selected_index_list =
        get_view()->selectionModel()->selectedIndexes();

    if (selected_index_list.size() > 0) {
        QModelIndex selected_index = selected_index_list.at(0);
        return selected_index;
    }
    return {};
}

QAbstractItemView* BaseSelector::get_view() { return abstract_item_view; }

void BaseSelector::on_delete(
    const QModelIndex& source_index, const QModelIndex& selected_index
) {}

void BaseSelector::on_return_no_select(const QString& text) {
    if (get_view()->model()->hasIndex(0, 0)) {
        on_select(get_view()->model()->index(0, 0));
    }
}

bool BaseSelector::on_text_change(const QString& text) { return false; }

bool MySortFilterProxyModel::filterAcceptsRow(
    int source_row, const QModelIndex& source_parent
) const {
    if (FUZZY_SEARCHING) {

        QModelIndex source_index = sourceModel()->index(
            source_row, this->filterKeyColumn(), source_parent
        );
        if (source_index.isValid()) {
            // check current index itself :

            QString key =
                sourceModel()->data(source_index, filterRole()).toString();
            if (filterString.size() == 0)
                return true;
            std::wstring s1 = filterString.toStdWString();
            std::wstring s2 = key.toStdWString();
            int score =
                static_cast<int>(rapidfuzz::fuzz::partial_ratio(s1, s2));

            return score > 50;
        } else {
            return false;
        }
    } else {
        return QSortFilterProxyModel::filterAcceptsRow(
            source_row, source_parent
        );
    }
}

void MySortFilterProxyModel::setFilterCustom(QString filterString) {
    if (FUZZY_SEARCHING) {
        this->filterString = filterString;
        this->setFilterFixedString(filterString);
        sort(0);
    } else {
        this->setFilterFixedString(filterString);
    }
}

bool MySortFilterProxyModel::lessThan(
    const QModelIndex& left, const QModelIndex& right
) const {
    if (FUZZY_SEARCHING) {

        QString leftData = sourceModel()->data(left).toString();
        QString rightData = sourceModel()->data(right).toString();

        int left_score = static_cast<int>(rapidfuzz::fuzz::partial_ratio(
            filterString.toStdWString(), leftData.toStdWString()
        ));
        int right_score = static_cast<int>(rapidfuzz::fuzz::partial_ratio(
            filterString.toStdWString(), rightData.toStdWString()
        ));
        return left_score > right_score;
    } else {
        return QSortFilterProxyModel::lessThan(left, right);
    }
}

MySortFilterProxyModel::MySortFilterProxyModel() {
    if (FUZZY_SEARCHING) {
        setDynamicSortFilter(true);
    }
}
