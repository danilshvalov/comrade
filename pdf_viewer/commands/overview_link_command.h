#pragma once

#include "open_link_command.h"

class OverviewLinkCommand : public OpenLinkCommand {
  public:
    std::string get_name() const override { return "overview_link"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_overview_link(text.value());
    }
};
