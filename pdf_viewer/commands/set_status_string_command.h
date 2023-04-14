#pragma once

#include "text_command.h"

class SetStatusStringCommand : public TextCommand {
  public:
    std::string get_name() const override { return "set_status_string"; }

    std::string text_requirement_name() const override {
        return "Status String";
    }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.set_status_message(text.value());
    }
};
