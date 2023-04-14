#pragma once

#include "command.h"
#include "main_widget.h"

class GotoLeftSmartCommand : public Command {
  public:
    std::string get_name() const override { return "goto_left_smart"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->goto_left_smart();
    }
};
