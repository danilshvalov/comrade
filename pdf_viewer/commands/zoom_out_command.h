#pragma once

#include "command.h"
#include "main_widget.h"

class ZoomOutCommand : public Command {
  public:
    std::string get_name() const override { return "zoom_out"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->zoom_out();
        widget.last_smart_fit_page = {};
    }
};
