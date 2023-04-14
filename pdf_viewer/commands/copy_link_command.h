#pragma once

#include "text_command.h"

class CopyLinkCommand : public TextCommand {
  public:
    std::string get_name() const override { return "copy_link"; }

    std::string text_requirement_name() const override { return "Label"; }

    void pre_perform(MainWidget& widget) override {
        widget.opengl_widget->set_highlight_links(true, true);
    }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_open_link(text.value(), true);
    }
};
