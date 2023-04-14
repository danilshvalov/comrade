#pragma once

#include "command.h"
#include "main_widget.h"

class OverviewDefinitionCommand : public Command {
  public:
    std::string get_name() const override { return "overview_definition"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.overview_to_definition();
    }
};
