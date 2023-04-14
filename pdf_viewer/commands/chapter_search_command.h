#pragma once

#include "text_command.h"

class ChapterSearchCommand : public TextCommand {
  public:
    void pre_perform(MainWidget& widget) override {
        std::optional<std::pair<int, int>> chapter_range =
            widget.main_document_view->get_current_page_range();
        if (chapter_range) {
            std::stringstream search_range_string;
            search_range_string << "<" << chapter_range.value().first << ","
                                << chapter_range.value().second << ">";
            widget.text_command_line_edit->setText(
                search_range_string.str().c_str() +
                widget.text_command_line_edit->text()
            );
        }
    }

    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "chapter_search"; }

    std::string text_requirement_name() const override { return "Search Term"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.perform_search(this->text.value(), false);
    }
};
