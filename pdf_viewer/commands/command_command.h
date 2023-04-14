#pragma once

#include "command.h"
#include "../main_widget.h"

class CommandCommand : public Command {
  public:
    std::string get_name() const { return "command"; }

    bool requires_document() const { return false; }

  protected:
    void perform(MainWidget& widget) {
        QStringList command_names =
            widget.command_manager->get_all_command_names();
        widget.set_current_widget(new CommandSelector(
            &widget.on_command_done, &widget, command_names,
            widget.input_handler->get_command_key_mappings()
        ));
        widget.current_widget->show();
    }
};
