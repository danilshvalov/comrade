#pragma once

#include "command.h"
#include "main_widget.h"

class GotoOverviewCommand : public Command {
  public:
    std::string get_name() const override { return "goto_overview"; }

  protected:
    void perform(MainWidget& widget) override { widget.goto_overview(); }
};
