#pragma once

#include "command.h"
#include "main_widget.h"

class CustomCommand : public Command {

  public:
    CustomCommand(std::string name_, std::string command_)
        : name(std::move(name_)),
          raw_command(std::move(command_)) {}

    std::optional<Requirement> next_requirement(MainWidget& widget) override {
        if (command_requires_rect(raw_command) && (!command_rect.has_value())) {
            Requirement req = {RequirementType::Rect, "Command Rect"};
            return req;
        }
        if (command_requires_text(raw_command) && (!command_text.has_value())) {
            Requirement req = {RequirementType::Text, "Command Text"};
            return req;
        }
        return {};
    }

    void set_rect_requirement(fz_rect rect) override { command_rect = rect; }

    void set_text_requirement(std::string txt) override { command_text = txt; }

    std::string get_name() const override { return name; }

  protected:
    void perform(MainWidget& widget) override {
        widget.execute_command(raw_command, command_text.value_or(""));
    }

  private:
    std::string name;
    std::string raw_command;
    std::optional<fz_rect> command_rect;
    std::optional<std::string> command_text;
};
