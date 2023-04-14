#pragma once

#include "command.h"
#include "main_widget.h"

class OpenPrevDocCommand : public Command {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "open_prev_doc"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.handle_open_prev_doc(); }
};
