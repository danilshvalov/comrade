#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleHorizontalLockCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_horizontal_lock"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.horizontal_scroll_locked = !widget.horizontal_scroll_locked;
    }
};
