#pragma once

#include "command.h"
#include "main_widget.h"

class FitToPageWidthSmartCommand : public Command {
  public:
    std::string get_name() const override { return "fit_to_page_width_smart"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->fit_to_page_width(true);
        int current_page = widget.get_current_page_number();
        widget.last_smart_fit_page = current_page;
    }
};
