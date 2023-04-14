#pragma once

#include "text_command.h"

class GotoBookmarkCommand : public Command {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "goto_bookmark"; }

  protected:
    void perform(MainWidget& widget) override { widget.handle_goto_bookmark(); }
};
