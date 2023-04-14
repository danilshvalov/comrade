#pragma once

#include "text_command.h"

class GotoBookmarkGlobalCommand : public Command {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "goto_bookmark_g"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_goto_bookmark_global();
    }
};
