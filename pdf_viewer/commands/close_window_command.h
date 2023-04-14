#pragma once

#include "command.h"
#include "main_widget.h"

class CloseWindowCommand : public Command {
  public:
    std::string get_name() const override { return "close_window"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.close(); }
};
