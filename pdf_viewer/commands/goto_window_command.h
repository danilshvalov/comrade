#pragma once

#include "command.h"
#include "main_widget.h"

class GotoWindowCommand : public Command {
  public:
    std::string get_name() const override { return "goto_window"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.handle_goto_window(); }
};
