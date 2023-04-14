#pragma once

#include "command.h"
#include "document_view.h"
#include "main_widget.h"

class GotoPrevHighlightOfTypeCommand : public Command {
  public:
    std::string get_name() const override {
        return "goto_prev_highlight_of_type";
    }

  protected:
    void perform(MainWidget& widget) override {
        DocumentView* view = widget.main_document_view;
        auto prev_highlight = view->get_document()->get_prev_highlight(
            view->get_offset_y(), widget.select_highlight_type
        );
        if (prev_highlight) {
            widget.long_jump_to_destination(prev_highlight->selection_begin.y);
        }
    }
};
