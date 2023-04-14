#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleFullscreenCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_fullscreen"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.toggle_fullscreen(); }
};
