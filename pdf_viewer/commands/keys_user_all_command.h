#pragma once

#include "command.h"
#include "main_widget.h"

class KeysUserAllCommand : public Command {
  public:
    std::string get_name() const override { return "keys_user_all"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.handle_keys_user_all(); }
};
