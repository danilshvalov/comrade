#pragma once

#include "command.h"
#include "main_widget.h"

class TogglePresentationModeCommand : public Command {
  public:
    std::string get_name() const override { return "toggle_presentation_mode"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.toggle_presentation_mode();
    }
};
