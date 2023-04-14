#pragma once

#include "command.h"
#include "main_widget.h"

class TurnOnSynctexCommand : public Command {
  public:
    std::string get_name() const override { return "turn_on_synctex"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override { widget.set_synctex_mode(true); }
};
