#pragma once

#include "command.h"
#include "main_widget.h"

class NextChapterCommand : public Command {
  public:
    std::string get_name() const override { return "next_chapter"; }

  protected:
    void perform(MainWidget& widget) override {
        int rp = num_repeats_ == 0 ? 1 : num_repeats_;
        widget.main_document_view->goto_chapter(rp);
    }
};
