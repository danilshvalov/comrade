#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleTittlebarCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_titlebar"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.toggle_titlebar(); }
};
