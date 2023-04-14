#pragma once

#include "command.h"
#include "document_view.h"
#include "main_widget.h"

class AddHighlightWithCurrentTypeCommand : public Command {
  public:
    std::string get_name() const override {
        return "add_highlight_with_current_type";
    }

  protected:
    void perform(MainWidget& widget) override {
        DocumentView* view = widget.main_document_view;
        if (!view->selected_character_rects.empty()) {
            view->add_highlight(
                widget.selection_begin, widget.selection_end,
                widget.select_highlight_type
            );
            view->selected_character_rects.clear();
            widget.selected_text.clear();
        }
    }
};
