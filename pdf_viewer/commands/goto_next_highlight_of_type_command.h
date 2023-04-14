#pragma once

#include "command.h"
#include "document_view.h"
#include "main_widget.h"

class GotoNextHighlightOfTypeCommand : public Command {
  public:
    std::string get_name() const override {
        return "goto_next_highlight_of_type";
    }

  protected:
    void perform(MainWidget& widget) override {
        DocumentView* view = widget.main_document_view;
        auto next_highlight = view->get_document()->get_next_highlight(
            view->get_offset_y(), widget.select_highlight_type
        );
        if (next_highlight) {
            widget.long_jump_to_destination(next_highlight->selection_begin.y);
        }
    }
};
