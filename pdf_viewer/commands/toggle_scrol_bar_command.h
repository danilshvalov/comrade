#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleScrollbarCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_scrollbar"; }

  protected:
    void perform(MainWidget& widget) override { widget.toggle_scrollbar(); }
};
