#pragma once

#include "command.h"
#include "../main_widget.h"

class GotoTableOfContentsCommand : public Command {
  public:
    std::string get_name() const override { return "goto_toc"; }

  protected:
    void perform(MainWidget& widget) override { widget.handle_goto_toc(); }
};
