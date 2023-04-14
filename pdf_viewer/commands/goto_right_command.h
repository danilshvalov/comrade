#pragma once

#include "command.h"
#include "main_widget.h"

class GotoRightCommand : public Command {
  public:
    std::string get_name() const override { return "goto_right"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->goto_right();
    }
};
