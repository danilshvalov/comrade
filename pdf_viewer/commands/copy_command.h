#pragma once

#include "command.h"
#include "main_widget.h"
#include "utils.h"

class CopyCommand : public Command {
  public:
    std::string get_name() const override { return "copy"; }

  protected:
    void perform(MainWidget& widget) override {
        copy_to_clipboard(widget.selected_text);
    }
};
