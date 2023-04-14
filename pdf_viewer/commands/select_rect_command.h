#pragma once

#include "command.h"
#include "main_widget.h"

class SelectRectCommand : public Command {
  public:
    std::string get_name() const override { return "select_rect"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.set_rect_select_mode(true);
    }
};
