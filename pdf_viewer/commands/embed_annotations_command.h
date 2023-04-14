#pragma once

#include "command.h"
#include "main_widget.h"
#include "ui.h"

class EmbedAnnotationsCommand : public Command {
  public:
    std::string get_name() const override { return "embed_annotations"; }

  protected:
    void perform(MainWidget& widget) override {
        std::wstring embedded_pdf_file_name = select_new_pdf_file_name();
        if (embedded_pdf_file_name.size() > 0) {
            widget.main_document_view->get_document()->embed_annotations(
                embedded_pdf_file_name
            );
        }
    }
};
