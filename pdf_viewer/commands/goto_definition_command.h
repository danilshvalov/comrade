#pragma once

#include "command.h"
#include "main_widget.h"

class GotoDefinitionCommand : public Command {
  public:
    std::string get_name() const override { return "goto_definition"; }

    bool pushes_state() const override { return true; }

  protected:
    void perform(MainWidget& widget) override {
        if (widget.main_document_view->goto_definition()) {
            widget.opengl_widget->set_should_draw_vertical_line(false);
        }
    }
};
