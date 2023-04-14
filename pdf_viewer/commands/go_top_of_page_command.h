#pragma once

#include "command.h"
#include "main_widget.h"

class GotoTopOfPageCommand : public Command {
  public:
    std::string get_name() const override { return "goto_top_of_page"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->goto_top_of_page();
    }
};
