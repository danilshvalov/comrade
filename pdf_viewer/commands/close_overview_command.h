#pragma once

#include "command.h"
#include "main_widget.h"

class CloseOverviewCommand : public Command {
  public:
    std::string get_name() const override { return "close_overview"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.opengl_widget->set_overview_page({});
    }
};
