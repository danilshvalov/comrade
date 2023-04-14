#pragma once

#include "symbol_command.h"

class AddHighlightCommand : public SymbolCommand {
  public:
    std::string get_name() const override { return "add_highlight"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_add_highlight(symbol);
    }
};
