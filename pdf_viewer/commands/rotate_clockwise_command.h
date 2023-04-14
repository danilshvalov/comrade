#pragma once

#include "command.h"
#include "main_widget.h"

class RotateClockwiseCommand : public Command {
  public:
    std::string get_name() const override { return "rotate_clockwise"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->rotate();
        widget.opengl_widget->rotate_clockwise();
    }
};
