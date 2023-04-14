#pragma once

#include "text_command.h"

class GotoPageWithPageNumberCommand : public TextCommand {
  public:
    std::string get_name() const override {
        return "goto_page_with_page_number";
    }

    std::string text_requirement_name() const override { return "Page Number"; }

    bool pushes_state() const override { return true; }

  protected:
    void perform(MainWidget& widget) override {
        std::wstring text_ = text.value();
        if (is_string_numeric(text_.c_str()) &&
            text_.size() < 6) { // make sure the page number is valid
            int dest = std::stoi(text_.c_str()) - 1;
            widget.main_document_view->goto_page(
                dest + widget.main_document_view->get_page_offset()
            );
        }
    }
};
