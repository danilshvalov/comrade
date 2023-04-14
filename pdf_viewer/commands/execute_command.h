#pragma once

#include "text_command.h"

class ExecuteCommand : public TextCommand {
  public:
    std::string get_name() const override { return "execute"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.execute_command(text.value());
    }
};
