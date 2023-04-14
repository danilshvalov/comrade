#pragma once

#include "open_link_command.h"

class PortalToLinkCommand : public OpenLinkCommand {
  public:
    std::string get_name() const override { return "portal_to_link"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_portal_to_link(text.value());
    }
};
