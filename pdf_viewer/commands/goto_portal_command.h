#pragma once

#include "command.h"
#include "main_widget.h"

class GotoPortalCommand : public Command {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "goto_link"; }

  protected:
    void perform(MainWidget& widget) override {
        std::optional<Portal> link =
            widget.main_document_view->find_closest_portal();
        if (link) {
            widget.open_document(link->dst);
        }
    }
};
