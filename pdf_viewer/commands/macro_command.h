#pragma once

#include "command.h"
#include "main_widget.h"
#include "lazy_command.h"

class MacroCommand : public Command {
  public:
    MacroCommand(
        CommandManager* manager, std::string name_, std::wstring commands_
    ) {
        command_manager = manager;
        name = name_;

        auto parts = QString::fromStdWString(commands_).split(';');
        for (int i = 0; i < parts.size(); i++) {
            if (parts.at(i).size() > 0) {
                commands.push_back(std::make_unique<LazyCommand>(
                    manager, parts.at(i).toStdWString()
                ));
            }
        }
    }

    std::string get_name() const override { return name; }

  protected:
    void perform(MainWidget& widget) override {
        for (std::unique_ptr<Command>& subcommand : commands) {
            subcommand->run(widget);
        }
    }

  private:
    std::vector<std::unique_ptr<Command>> commands;
    std::string name;
    CommandManager* command_manager;
};
