#pragma once

#include "text_command.h"

class EnterPasswordCommand : public TextCommand {
  public:
    std::string get_name() const override { return "enter_password"; }

    std::string text_requirement_name() const override { return "Password"; }

  protected:
    void perform(MainWidget& widget) override {
        std::string password = text.value();
        widget.pdf_renderer->add_password(
            widget.main_document_view->get_document()->get_path(), password
        );
    }
};
