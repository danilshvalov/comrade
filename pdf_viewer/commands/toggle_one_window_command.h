#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleOneWindowCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_one_window"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.toggle_two_window_mode();
    }
};
