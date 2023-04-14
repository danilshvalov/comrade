#pragma once

#include "command.h"
#include "main_widget.h"

class NextPageCommand : public Command {
  public:
    std::string get_name() const override { return "next_page"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->move_pages(1 + num_repeats_);
    }
};
