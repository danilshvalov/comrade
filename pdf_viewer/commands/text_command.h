#pragma once

#include "command.h"
#include "../main_widget.h"

class TextCommand : public Command {
  public:
    virtual std::string text_requirement_name() const { return "text"; }

    std::optional<Requirement> next_requirement(MainWidget& widget) override {
        if (text.has_value()) {
            return {};
        } else {
            return Requirement{RequirementType::Text, text_requirement_name()};
        }
    }

    void set_text_requirement(std::string value) override {
        this->text = value;
    }

  protected:
    std::optional<std::string> text = {};
};
