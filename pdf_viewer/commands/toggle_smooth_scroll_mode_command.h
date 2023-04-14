#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleSmoothScrollModeCommand : public Command {
  public:
    std::string get_name() const override {
        return "toggle_smooth_scroll_mode";
    }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_toggle_smooth_scroll_mode();
    }
};
