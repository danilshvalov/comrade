#pragma once

#include "command.h"
#include "main_widget.h"

class OpenDocumentEmbeddedCommand : public Command {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override { return "open_document_embedded"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        widget.set_current_widget(new FileSelector(
            [&widget](std::string doc_path) {
                widget.validate_render();
                widget.open_document(doc_path);
            },
            &widget, ""
        ));
        widget.current_widget->show();
    }
};
