#pragma once

#include "command.h"
#include "main_widget.h"

class MoveVisualMarkUpCommand : public Command {
  public:
    std::string get_name() const override { return "move_visual_mark_up"; }

  protected:
    void perform(MainWidget& widget) override {
        int rp = num_repeats_ == 0 ? 1 : num_repeats_;
        widget.move_visual_mark_command(-rp);
    }
};
