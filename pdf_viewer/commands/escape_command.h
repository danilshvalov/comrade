#pragma once

#include "command.h"
#include "main_widget.h"

class EscapeCommand : public Command {
  public:
    std::string get_name() const override { return "escape"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.handle_escape(); }
};
