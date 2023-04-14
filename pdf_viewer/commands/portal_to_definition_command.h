#pragma once

#include "command.h"
#include "main_widget.h"

class PortalToDefinitionCommand : public Command {
  public:
    std::string get_name() const override { return "portal_to_definition"; }

  protected:
    void perform(MainWidget& widget) override { widget.portal_to_definition(); }
};
