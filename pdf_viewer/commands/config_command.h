#pragma once

#include "text_command.h"

class ConfigCommand : public TextCommand {
  public:
    ConfigCommand(std::string config_name_)
        : config_name(std::move(config_name_)) {}

    std::string get_name() const override { return "setconfig_" + config_name; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.config_manager->deserialize_config(config_name, text.value());
    }

  private:
    std::string config_name;
};
