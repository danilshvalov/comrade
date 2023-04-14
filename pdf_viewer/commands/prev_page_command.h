#pragma once

#include "command.h"
#include "main_widget.h"

class PreviousPageCommand : public Command {
  public:
    std::string get_name() const override { return "previous_page"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->move_pages(-1 - num_repeats_);
    }
};
