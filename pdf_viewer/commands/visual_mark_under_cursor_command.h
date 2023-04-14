#pragma once

#include "command.h"
#include "main_widget.h"

class VisualMarkUnderCursorCommand : public Command {
  public:
    std::string get_name() const override { return "visual_mark_under_cursor"; }

  protected:
    void perform(MainWidget& widget) override {
        QPoint mouse_pos = widget.mapFromGlobal(QCursor::pos());
        widget.visual_mark_under_pos({mouse_pos.x(), mouse_pos.y()});
    }
};
