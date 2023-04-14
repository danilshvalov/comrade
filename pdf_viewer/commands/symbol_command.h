#pragma once

#include "command.h"
#include "../main_widget.h"

class SymbolCommand : public Command {
  public:
    virtual std::optional<Requirement> next_requirement(MainWidget& widget) {
        if (symbol == 0) {
            return Requirement{RequirementType::Symbol, "symbol"};
        } else {
            return {};
        }
    }

    virtual void set_symbol_requirement(char value) { this->symbol = value; }

  protected:
    char symbol = 0;
};
