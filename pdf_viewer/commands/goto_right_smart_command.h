#pragma once

#include "command.h"
#include "main_widget.h"

class GotoRightSmartCommand : public Command {
  public:
    std::string get_name() const override { return "goto_right_smart"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->goto_right_smart();
    }
};
