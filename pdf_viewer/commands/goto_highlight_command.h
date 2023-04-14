#pragma once

#include "command.h"
#include "../main_widget.h"

class GotoHighlightCommand : public Command {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "goto_highlight"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_goto_highlight();
    }
};
