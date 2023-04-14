#pragma once

#include "command.h"
#include "main_widget.h"

class NextItemCommand : public Command {
    void perform(MainWidget& widget) override {
        if (num_repeats_ == 0) {
            num_repeats_++;
        }
        widget.opengl_widget->goto_search_result(num_repeats_);
    }

    std::string get_name() const override { return "next_item"; }
};
