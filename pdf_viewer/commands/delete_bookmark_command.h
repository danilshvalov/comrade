#pragma once

#include "command.h"
#include "main_widget.h"

class DeleteBookmarkCommand : public Command {
  public:
    std::string get_name() const override { return "delete_bookmark"; }

  protected:
    void perform(MainWidget& widget) override {
        widget.main_document_view->delete_closest_bookmark();
        widget.validate_render();
    }
};
