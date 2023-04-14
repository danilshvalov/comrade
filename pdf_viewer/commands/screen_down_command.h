#pragma once

#include "command.h"
#include "main_widget.h"

class ScreenDownCommand : public Command {
  public:
    std::string get_name() const override { return "screen_down"; }

  protected:
    void perform(MainWidget& widget) override {
        int rp = num_repeats_ == 0 ? 1 : num_repeats_;
        widget.handle_move_screen(rp);
    }
};
