#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleShowLastCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_show_last_command"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.should_show_last_command = !widget.should_show_last_command;
    }
};
