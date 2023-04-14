#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleSelectHighlightCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_select_highlight"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.is_select_highlight_mode = !widget.is_select_highlight_mode;
    }
};
