#pragma once

#include "command.h"
#include "main_widget.h"

class ClearStatusStringCommand : public Command {
  public:
    std::string get_name() const override { return "clear_status_string"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.set_status_message(""); }
};
