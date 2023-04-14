#pragma once

#include "text_command.h"

class FocusTextCommand : public TextCommand {
  public:
    std::string get_name() const override { return "focus_text"; }

    std::string text_requirement_name() const override {
        return "Text to focus";
    }

  protected:
    void perform(MainWidget& widget) override {
        std::wstring text_ = text.value();
        widget.handle_focus_text(text_);
    }
};
