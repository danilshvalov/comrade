#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleVisualScrollCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_visual_scroll"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.toggle_visual_scroll_mode();
    }
};
