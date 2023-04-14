#pragma once

#include "command.h"
#include "../main_widget.h"

class PrevStateCommand : public Command {
  public:
    std::string get_name() const override { return "prev_state"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.prev_state(); }
};
