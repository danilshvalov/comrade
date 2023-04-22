#pragma once

#include "config.h"
#include "commands/command.h"

class CommandManager {
  public:
    CommandManager(ConfigManager* config_manager);
    std::unique_ptr<Command> get_command_with_name(std::string name) const;
    std::unique_ptr<Command> create_macro_command(
        std::string name, std::string macro_string
    );
    QStringList get_all_command_names() const;

  private:
    using CommandCreator = std::function<std::unique_ptr<Command>()>;
    std::map<std::string, CommandCreator> new_commands;
};
