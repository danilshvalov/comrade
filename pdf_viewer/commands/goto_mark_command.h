#pragma once

#include "symbol_command.h"

class GotoMarkCommand : public SymbolCommand {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "goto_mark"; }

    std::vector<char> special_symbols() const override {
        return {'`', '\'', '/'};
    }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        assert(this->symbol != 0);
        widget.goto_mark(this->symbol);
    }
};
