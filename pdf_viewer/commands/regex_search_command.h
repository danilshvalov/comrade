#pragma once

#include "text_command.h"

class RegexSearchCommand : public TextCommand {
  public:
    std::string get_name() const override { return "regex_search"; }

    bool pushes_state() const override { return true; }

    std::string text_requirement_name() const override { return "regex"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.perform_search(this->text.value(), true);
    }
};
