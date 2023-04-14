#pragma once

#include "symbol_command.h"

class SetSelectHighlightTypeCommand : public SymbolCommand {
  public:
    std::string get_name() const override {
        return "set_select_highlight_type";
    }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.select_highlight_type = symbol;
    }
};
