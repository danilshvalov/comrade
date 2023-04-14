#pragma once

#include "text_command.h"

class KeyboardSelectCommand : public TextCommand {
  public:
    std::string get_name() const override { return "keyboard_select"; }

    std::string text_requirement_name() const override { return "Labels"; }

    void pre_perform(MainWidget& widget) override { widget.highlight_words(); }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_keyboard_select(text.value());
    }
};
