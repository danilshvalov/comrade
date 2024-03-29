#pragma once

#include "command.h"
#include "main_widget.h"

class OpenDocumentEmbeddedFromCurrentPathCommand : public Command {
  public:
    bool pushes_state() const override { return true; }

    std::string get_name() const override {
        return "open_document_embedded_from_current_path";
    }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        std::string last_file_name =
            widget.get_current_file_name().value_or("");

        widget.set_current_widget(new FileSelector(
            [&widget](std::string doc_path) {
                widget.validate_render();
                widget.open_document(doc_path);
            },
            &widget, QString::fromStdString(last_file_name)
        ));
        widget.current_widget->show();
    }
};
