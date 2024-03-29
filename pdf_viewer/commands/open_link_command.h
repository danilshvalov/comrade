#pragma once

#include "text_command.h"
#include "config.h"

class OpenLinkCommand : public TextCommand {
  public:
    std::string text_requirement_name() const override { return "Label"; }

    std::optional<Requirement> next_requirement(MainWidget& widget) override {
        const Config& config = Config::instance();
        if (text.has_value()) {
            return {};
        } else if (widget.num_visible_links() < 26 && config.ALPHABETIC_LINK_TAGS) {
            return Requirement{RequirementType::Symbol, "Label"};
        } else if (widget.num_visible_links() < 10 && !config.ALPHABETIC_LINK_TAGS) {
            return Requirement{RequirementType::Symbol, "Label"};
        } else {
            return Requirement{RequirementType::Text, text_requirement_name()};
        }
    }

    std::string get_name() const override { return "open_link"; }

    void set_symbol_requirement(char value) override {
        std::string val;
        val.push_back(value);
        this->text = val;
    }

    void pre_perform(MainWidget& widget) override {
        widget.opengl_widget->set_highlight_links(true, true);
    }

  protected:
    void perform(MainWidget& widget) override {
        widget.handle_open_link(text.value());
    }
};
