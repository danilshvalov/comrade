#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleTypingModeCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_typing_mode"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_toggle_typing_mode();
    }
};
