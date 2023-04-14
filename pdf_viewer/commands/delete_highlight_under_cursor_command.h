#pragma once

#include "command.h"
#include "main_widget.h"

class DeleteHighlightUnderCursorCommand : public Command {
  public:
    std::string get_name() const override {
        return "delete_highlight_under_cursor";
    }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_delete_highlight_under_cursor();
    }
};
