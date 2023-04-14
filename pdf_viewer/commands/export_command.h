#pragma once

#include "command.h"
#include "main_widget.h"

class ExportCommand : public Command {
  public:
    std::string get_name() const override { return "export"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        std::wstring export_file_name = select_new_json_file_name();
        widget.db_manager->export_json(export_file_name, widget.checksummer);
    }
};
