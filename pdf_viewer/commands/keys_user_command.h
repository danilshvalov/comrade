#pragma once

#include "command.h"
#include "main_widget.h"

class KeysUserCommand : public Command {
  public:
    std::string get_name() const override { return "keys_user"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        std::optional<fs::path> key_file_path =
            widget.input_handler->get_or_create_user_keys_path();
        if (key_file_path) {
            open_file(key_file_path.value().generic_wstring());
        }
    }
};
