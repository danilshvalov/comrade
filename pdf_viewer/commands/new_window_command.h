#pragma once

#include "command.h"
#include "main_widget.h"

class NewWindowCommand : public Command {
  public:
    std::string get_name() const override { return "new_window"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.handle_new_window(); }
};
