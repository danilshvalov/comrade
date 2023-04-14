#pragma once

#include "text_command.h"

class SearchCommand : public TextCommand {
  public:
    std::string get_name() const override { return "search"; }

    bool pushes_state() const override { return true; }

    std::string text_requirement_name() const override { return "Search Term"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.perform_search(this->text.value(), false);
    }
};
