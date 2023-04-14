#pragma once

#include "command.h"

class NoopCommand : public Command {
  public:
    std::string get_name() const override { return "noop"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {}
};
