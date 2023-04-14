#pragma once

#include "command.h"
#include "main_widget.h"

class ZoomInCommand : public Command {
  public:
    std::string get_name() const override { return "zoom_in"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->zoom_in();
        widget.last_smart_fit_page = {};
    }
};
