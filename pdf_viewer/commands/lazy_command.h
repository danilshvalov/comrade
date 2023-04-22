#pragma once

#include "command.h"
#include "command_manager.h"
#include "noop_command.h"

class LazyCommand : public Command {
  public:
    LazyCommand(CommandManager* manager, std::string command_text) {
        command_manager = manager;
        parse_command_text(command_text);
    }

    void set_text_requirement(std::string value) override {
        get_command().set_text_requirement(value);
    }

    void set_symbol_requirement(char value) override {
        get_command().set_symbol_requirement(value);
    }

    void set_file_requirement(std::string value) override {
        get_command().set_file_requirement(value);
    }

    void set_rect_requirement(fz_rect value) override {
        get_command().set_rect_requirement(value);
    }

    void set_num_repeats(int nr) override { get_command().set_num_repeats(nr); }

    std::vector<char> special_symbols() const override {
        return get_command().special_symbols();
    }

    void pre_perform(MainWidget& widget) override {
        get_command().pre_perform(widget);
    }

    bool pushes_state() const override { return get_command().pushes_state(); }

    bool requires_document() const override {
        return get_command().requires_document();
    }

    std::string get_name() const override { return get_command().get_name(); }

  protected:
    void perform(MainWidget& w) override { get_command().run(w); }

  private:
    void parse_command_text(std::string command_text) {
        int index = command_text.find('(');
        if (index != -1) {
            command_name = command_text.substr(0, index);
            command_params =
                command_text.substr(index + 1, command_text.size() - index - 2);
        } else {
            command_name = command_text;
        }
    }

    const Command& get_command() const {
        if (actual_command) {
            return *actual_command;
        }

        actual_command = command_manager->get_command_with_name(command_name);
        if (!actual_command) {
            return noop;
        }

        if (auto req = actual_command->next_requirement(*widget)) {
            switch (req->type) {
                case RequirementType::Text:
                    actual_command->set_text_requirement(command_params);
                    break;
                case RequirementType::File:
                    actual_command->set_file_requirement(command_params);
                    break;
                case RequirementType::Symbol: {
                    if (!command_params.empty()) {
                        actual_command->set_symbol_requirement(
                            static_cast<char>(command_params.front())
                        );
                    }
                    break;
                }
                default:
                    break;
            }
        }
        return *actual_command;
    }

    // TODO: remove copy pasta
    Command& get_command() {
        if (actual_command) {
            return *actual_command;
        }

        actual_command = command_manager->get_command_with_name(command_name);
        if (!actual_command) {
            return noop;
        }

        if (auto req = actual_command->next_requirement(*widget)) {
            switch (req->type) {
                case RequirementType::Text:
                    actual_command->set_text_requirement(command_params);
                    break;
                case RequirementType::File:
                    actual_command->set_file_requirement(command_params);
                    break;
                case RequirementType::Symbol: {
                    if (!command_params.empty()) {
                        actual_command->set_symbol_requirement(
                            static_cast<char>(command_params.front())
                        );
                    }
                    break;
                }
                default:
                    break;
            }
        }
        return *actual_command;
    }

    CommandManager* command_manager;
    MainWidget* widget;
    std::string command_name;
    std::string command_params;
    mutable std::unique_ptr<Command> actual_command = nullptr;
    NoopCommand noop;
};
