#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleCustomColorMode : public Command {
  public:
    std::string get_name() const override { return "toggle_custom_color"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.opengl_widget->toggle_custom_color_mode();
        widget.helper_opengl_widget->toggle_custom_color_mode();
    }
};
