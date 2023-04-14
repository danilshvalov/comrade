#pragma once

#include "command.h"
#include "main_widget.h"

class PortalToOverviewCommand : public Command {
  public:
    std::string get_name() const override { return "portal_to_overview"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_portal_to_overview();
    }
};
