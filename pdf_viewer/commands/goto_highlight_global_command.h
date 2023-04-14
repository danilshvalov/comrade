#pragma once

#include "command.h"
#include "../main_widget.h"

class GotoHighlightGlobalCommand : public Command {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "goto_highlight_g"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_goto_highlight_global();
    }
};
