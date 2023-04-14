#pragma once

#include "text_command.h"

class AddBookmarkCommand : public TextCommand {
  public:
    std::string get_name() const override { return "add_bookmark"; }

    std::string text_requirement_name() const override {
        return "Bookmark Text";
    }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->add_bookmark(text.value());
    }
};
