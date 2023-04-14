#pragma once

#include "command.h"
#include "main_widget.h"

class ReloadConfigCommand : public Command {
  public:
    std::string get_name() const override { return "reload_config"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.on_config_file_changed(widget.config_manager);
    }
};
