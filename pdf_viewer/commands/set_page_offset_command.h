#pragma once

#include "text_command.h"

class SetPageOffsetCommand : public TextCommand {
  public:
    std::string get_name() const override { return "set_page_offset"; }

  protected:
    void perform(MainWidget& widget) override {
        if (is_string_numeric(text.value().c_str()) &&
            text.value().size() < 6) { // make sure the page number is valid
            widget.main_document_view->set_page_offset(
                std::stoi(text.value().c_str())
            );
        }
    }
};
