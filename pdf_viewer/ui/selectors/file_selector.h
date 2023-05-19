#pragma once

#include "ui/selectors/base_selector.h"
#include "utils.h"

#include <QStringListModel>
#include <QListView>
#include <QDir>
#include <QLineEdit>

class FileSelector : public BaseSelector {
  public:
    FileSelector(
        std::function<void(std::string)> on_done,
        QWidget* parent,
        QString last_path
    );

    QString get_view_stylesheet_type_name() override;

    virtual bool on_text_change(const QString& text) override;

    QStringList get_dir_contents(QString root, QString prefix);

    void on_select(const QModelIndex& index) override;

  private:
    QStringListModel* list_model = nullptr;
    std::function<void(std::string)> on_done = nullptr;
    QString last_root = "";
};
