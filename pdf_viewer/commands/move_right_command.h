#pragma once

#include "command.h"
#include "main_widget.h"

class MoveRightCommand : public Command {
  public:
    std::string get_name() const override { return "move_right"; }

  protected:
    void perform(MainWidget& widget) override {
        int rp = num_repeats_ == 0 ? 1 : num_repeats_;
        widget.handle_horizontal_move(rp);
    }
};
