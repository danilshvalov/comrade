#pragma once

#include "command.h"
#include "main_widget.h"

class ReloadCommand : public Command {
  public:
    std::string get_name() const override { return "relaod"; }

  protected:
    void perform(MainWidget& widget) override { widget.reload(); }
};
