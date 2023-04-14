#pragma once

#include "command.h"
#include "main_widget.h"

class NextPreviewCommand : public Command {
  public:
    std::string get_name() const override { return "next_preview"; }

  protected:
    void perform(MainWidget& widget) override {
        if (!widget.smart_view_candidates.empty()) {
            widget.index_into_candidates = (widget.index_into_candidates + 1) %
                                           widget.smart_view_candidates.size();
            widget.set_overview_position(
                widget.smart_view_candidates[widget.index_into_candidates].page,
                widget.smart_view_candidates[widget.index_into_candidates].y
            );
        }
    }
};
