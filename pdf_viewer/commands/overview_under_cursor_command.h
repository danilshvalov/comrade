#pragma once

#include "command.h"
#include "main_widget.h"

class OverviewUnderCursorCommand : public Command {
  public:
    std::string get_name() const override { return "overview_under_cursor"; }

  protected:
    void perform(MainWidget& widget) override {
        QPoint mouse_pos = widget.mapFromGlobal(QCursor::pos());
        widget.overview_under_pos({mouse_pos.x(), mouse_pos.y()});
    }
};
