#pragma once

#include "command.h"
#include "main_widget.h"

class DeleteHighlightCommand : public Command {
  public:
    std::string get_name() const override { return "delete_highlight"; }

  protected:
    void perform(MainWidget& widget) override {
        if (widget.selected_highlight_index != -1) {
            widget.main_document_view->delete_highlight_with_index(
                widget.selected_highlight_index
            );
            widget.selected_highlight_index = -1;
        }
        widget.validate_render();
    }
};
