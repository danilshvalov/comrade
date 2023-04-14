#pragma once

#include "command.h"
#include "main_widget.h"

class ToggleStatusbarCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_statusbar"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.toggle_statusbar(); }
};
