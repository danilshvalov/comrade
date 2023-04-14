#pragma once

#include "command.h"
#include "main_widget.h"

class EnterVisualMarkModeCommand : public Command {
  public:
    std::string get_name() const override { return "enter_visual_mark_mode"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.visual_mark_under_pos({widget.width() / 2, widget.height() / 2});
    }
};
