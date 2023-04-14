#pragma once

#include "command.h"
#include "main_widget.h"

class FitToPageHeightCommand : public Command {
  public:
    std::string get_name() const override { return "fit_to_page_height"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->fit_to_page_height();
        widget.last_smart_fit_page = {};
    }
};
