#pragma once

#include "symbol_command.h"

class SetMarkCommand : public SymbolCommand {
  public:
    std::string get_name() const override { return "set_mark"; }

  protected:
    void perform(MainWidget& widget) override {
        assert(this->symbol != 0);
        widget.set_mark_in_current_location(this->symbol);
    }
};
