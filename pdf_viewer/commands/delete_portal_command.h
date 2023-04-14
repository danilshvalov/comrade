#pragma once

#include "command.h"
#include "main_widget.h"

class DeletePortalCommand : public Command {
  public:
    std::string get_name() const override { return "delete_portal"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->delete_closest_portal();
        widget.validate_render();
    }
};
