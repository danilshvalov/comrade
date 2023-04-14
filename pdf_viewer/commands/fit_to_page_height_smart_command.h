#pragma once

#include "command.h"
#include "main_widget.h"

class FitToPageHeightSmartCommand : public Command {
  public:
    std::string get_name() const override { return "fit_to_page_height_smart"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->fit_to_page_height(true);
    }
};
