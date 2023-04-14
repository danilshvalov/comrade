#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleMouseDragModeCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_mouse_drag_mode"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.toggle_mouse_drag_mode();
    }
};
