#pragma once

#include "command.h"
#include "main_widget.h"

class CloseVisualMarkCommand : public Command {
  public:
    std::string get_name() const override { return "close_visual_mark"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.opengl_widget->set_should_draw_vertical_line(false);
    }
};
