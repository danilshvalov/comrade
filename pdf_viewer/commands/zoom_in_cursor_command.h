#pragma once

#include "command.h"
#include "main_widget.h"

class ZoomInCursorCommand : public Command {
  public:
    std::string get_name() const override { return "zoom_in_cursor"; }

  protected:
    void perform(MainWidget& widget) override {
        QPoint mouse_pos = widget.mapFromGlobal(QCursor::pos());
        widget.main_document_view->zoom_in_cursor({mouse_pos.x(), mouse_pos.y()}
        );
        widget.last_smart_fit_page = {};
    }
};
