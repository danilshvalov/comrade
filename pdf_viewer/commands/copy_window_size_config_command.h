#pragma once

#include "command.h"
#include "main_widget.h"

class CopyWindowSizeConfigCommand : public Command {
  public:
    std::string get_name() const override { return "copy_window_size_config"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        copy_to_clipboard(widget.get_window_configuration_string());
    }
};
