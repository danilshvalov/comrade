#pragma once

#include "command.h"
#include "main_widget.h"

class PrefsUserAllCommand : public Command {
  public:
    std::string get_name() const override { return "prefs_user_all"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_prefs_user_all();
    }
};
