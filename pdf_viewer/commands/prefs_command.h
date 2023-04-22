#pragma once

#include "command.h"
#include "utils.h"
#include "config.h"

class PrefsCommand : public Command {
  public:
    std::string get_name() const override { return "prefs"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        open_file(Config::instance().default_config_path.generic_string());
    }
};
