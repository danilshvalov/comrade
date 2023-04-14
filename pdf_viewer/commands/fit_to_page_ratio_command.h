#pragma once

#include "command.h"
#include "main_widget.h"

class FitToPageWidthRatioCommand : public Command {
  public:
    std::string get_name() const override { return "fit_to_page_width_ratio"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->fit_to_page_width(false, true);
        widget.last_smart_fit_page = {};
    }
};
