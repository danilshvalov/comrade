#pragma once

#include "text_command.h"

class KeyboardOverviewCommand : public TextCommand {
  public:
    std::string get_name() const override { return "keyboard_overview"; }

    std::string text_requirement_name() const override { return "Label"; }

    void pre_perform(MainWidget& widget) override { widget.highlight_words(); }

  protected:
    void perform(MainWidget& widget) override {
        std::optional<fz_irect> rect_ =
            widget.get_tag_window_rect(text.value());
        if (rect_) {
            fz_irect rect = rect_.value();
            widget.overview_under_pos(
                {(rect.x0 + rect.x1) / 2, (rect.y0 + rect.y1) / 2}
            );
            widget.opengl_widget->set_should_highlight_words(false);
        }
    }
};
