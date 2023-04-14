#pragma once

#include "command.h"
#include "../main_widget.h"

class PortalCommand : public Command {
  public:
    std::string get_name() const override { return "portal"; }

  protected:
    void perform(MainWidget& widget) override { widget.handle_portal(); }
};
