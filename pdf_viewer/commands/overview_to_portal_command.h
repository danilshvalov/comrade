#pragma once

#include "command.h"
#include "main_widget.h"

class OverviewToPortalCommand : public Command {
  public:
    std::string get_name() const override { return "overview_to_portal"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_overview_to_portal();
    }
};
