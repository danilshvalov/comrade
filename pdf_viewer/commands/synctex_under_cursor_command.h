#pragma once

#include "command.h"
#include "main_widget.h"

class SynctexUnderCursorCommand : public Command {
  public:
    std::string get_name() const override { return "synctex_under_cursor"; }

  protected:
    void perform(MainWidget& widget) override {
        QPoint mouse_pos = widget.mapFromGlobal(QCursor::pos());
        widget.synctex_under_pos({mouse_pos.x(), mouse_pos.y()});
    }
};
