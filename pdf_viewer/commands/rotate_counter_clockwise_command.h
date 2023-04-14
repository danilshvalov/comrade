#pragma once

#include "command.h"
#include "main_widget.h"

class RotateCounterClockwiseCommand : public Command {
  public:
    std::string get_name() const override { return "rotate_counterclockwise"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->rotate();
        widget.opengl_widget->rotate_counterclockwise();
    }
};
