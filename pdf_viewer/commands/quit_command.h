#pragma once

#include "command.h"
#include "main_widget.h"

class QuitCommand : public Command {
  public:
    std::string get_name() const override { return "quit"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_close_event();
        QApplication::quit();
    }
};
