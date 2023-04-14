#pragma once

#include "command.h"
#include "main_widget.h"

class GotoEndCommand : public Command {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "goto_end"; }

  protected:
    void perform(MainWidget& main_widget) override {
        if (num_repeats_ > 0) {
            main_widget.main_document_view->goto_page(
                num_repeats_ - 1 +
                main_widget.main_document_view->get_page_offset()
            );
        } else {
            main_widget.main_document_view->goto_end();
        }
    }
};
