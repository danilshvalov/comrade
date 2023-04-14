#pragma once

#include "command.h"
#include "main_widget.h"

class ImportCommand : public Command {
  public:
    std::string get_name() const override { return "import"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        std::wstring import_file_name = select_json_file_name();
        widget.db_manager->import_json(import_file_name, widget.checksummer);
    }
};
