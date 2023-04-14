#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleDarkModeCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_dark_mode"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.opengl_widget->toggle_dark_mode();
        widget.helper_opengl_widget->toggle_dark_mode();
    }
};
