#pragma once

#include <QWidget>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QSortFilterProxyModel>

class MySortFilterProxyModel : public QSortFilterProxyModel {
    QString filterString;

  public:
    MySortFilterProxyModel();
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;
    void setFilterCustom(QString filterString);
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
};

class BaseSelector : public QWidget {
  public:
    std::optional<QModelIndex> get_selected_index();

    virtual std::wstring get_selected_text() { return L""; }

    bool eventFilter(QObject* obj, QEvent* event) override;

    void handle_delete();

    virtual void on_config_file_changed();

    void resizeEvent(QResizeEvent* resize_event) override;

  protected:
    BaseSelector(
        QStandardItemModel* item_model,
        QAbstractItemView* abstract_item_view,
        QWidget* parent
    );

    virtual QAbstractItemView* get_view();

    virtual void on_select(const QModelIndex& value) = 0;

    virtual void on_delete(
        const QModelIndex& source_index, const QModelIndex& selected_index
    );

    virtual void on_return_no_select(const QString& text);

    // should return true when we want to manually handle text change events
    virtual bool on_text_change(const QString& text);

    virtual QString get_view_stylesheet_type_name() = 0;

    QLineEdit* line_edit = nullptr;
    QAbstractItemView* abstract_item_view = nullptr;
    MySortFilterProxyModel* proxy_model = nullptr;
};
