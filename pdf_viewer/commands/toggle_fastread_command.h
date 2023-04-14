#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleFastreadCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_fastread"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.opengl_widget->toggle_fastread_mode();
    }
};
