#pragma once

#include "command.h"
#include "utils.h"

extern fs::path default_config_path;

class PrefsCommand : public Command {
  public:
    std::string get_name() const override { return "prefs"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        open_file(default_config_path.generic_wstring());
    }
};
