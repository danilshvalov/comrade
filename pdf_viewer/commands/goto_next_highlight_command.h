#pragma once

#include "command.h"
#include "main_widget.h"

class GotoNextHighlightCommand : public Command {
  public:
    std::string get_name() const override { return "goto_next_highlight"; }

  protected:
    void perform(MainWidget& widget) override {
        auto next_highlight =
            widget.main_document_view->get_document()->get_next_highlight(
                widget.main_document_view->get_offset_y()
            );
        if (next_highlight.has_value()) {
            widget.long_jump_to_destination(
                next_highlight.value().selection_begin.y
            );
        }
    }
};
