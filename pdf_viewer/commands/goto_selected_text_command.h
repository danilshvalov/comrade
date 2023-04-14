#pragma once

#include "command.h"
#include "main_widget.h"

class GotoSelectedTextCommand : public Command {
  public:
    std::string get_name() const override { return "goto_selected_text"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.long_jump_to_destination(widget.selection_begin.y);
    }
};
