#pragma once

#include "command.h"
#include "main_widget.h"

class PrefsUserCommand : public Command {
  public:
    std::string get_name() const override { return "prefs_user"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        std::optional<fs::path> pref_file_path =
            widget.config_manager->get_or_create_user_config_file();
        if (pref_file_path) {
            open_file(pref_file_path.value().generic_wstring());
        }
    }
};
