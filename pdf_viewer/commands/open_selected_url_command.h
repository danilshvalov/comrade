#pragma once

#include "command.h"
#include "main_widget.h"

class OpenSelectedUrlCommand : public Command {
  public:
    std::string get_name() const override { return "open_selected_url"; }

  protected:
    void perform(MainWidget& widget) override {
        open_web_url(widget.selected_text.c_str());
    }
};
