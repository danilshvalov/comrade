#pragma once

#include "command.h"
#include "../main_widget.h"

class PrevItemCommand : public Command {
  public:
    std::string get_name() const override { return "previous_item"; }

  protected:
    void perform(MainWidget& widget) override {
        if (num_repeats_ == 0)
            num_repeats_++;
        widget.opengl_widget->goto_search_result(-num_repeats_);
    }
};
