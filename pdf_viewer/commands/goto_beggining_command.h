#pragma once

#include "command.h"
#include "main_widget.h"

class GotoBeginningCommand : public Command {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "goto_beginning"; }

  protected:
    void perform(MainWidget& main_widget) override {
        DocumentView* view = main_widget.main_document_view;
        if (num_repeats_) {
            view->goto_page(num_repeats_ - 1 + view->get_page_offset());
        } else {
            view->set_offset_y(0.0f);
        }
    }
};
