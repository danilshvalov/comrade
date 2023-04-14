#pragma once

#include "command.h"
#include "../main_widget.h"

class NextStateCommand : public Command {
  public:
    std::string get_name() const override { return "next_state"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.next_state(); }
};
