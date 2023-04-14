#pragma once

#include "command.h"
#include "../main_widget.h"

class ToggleWindowConfigurationCommand : public Command {
  public:
    std::string get_name() const override {
        return "toggle_window_configuration";
    }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.toggle_window_configuration();
    }
};
