#pragma once

#include "command.h"
#include "utils.h"
#include "config.h"

class KeysCommand : public Command {
  public:
    std::string get_name() const override { return "keys"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        open_file(Config::instance().default_keys_path.generic_string());
    }
};
