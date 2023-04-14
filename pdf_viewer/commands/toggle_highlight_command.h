#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleHighlightCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_highlight"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.opengl_widget->toggle_highlight_links();
    }
};
