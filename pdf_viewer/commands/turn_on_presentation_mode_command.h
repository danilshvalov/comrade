#pragma once

#include "command.h"
#include "main_widget.h"

class TurnOnPresentationModeCommand : public Command {
  public:
    std::string get_name() const override {
        return "turn_on_presentation_mode";
    }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.set_presentation_mode(true);
    }
};
