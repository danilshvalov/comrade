#pragma once

#include "command.h"
#include "main_widget.h"

class OpenLastDocumentCommand : public Command {
  public:
    std::string get_name() const override { return "open_last_document"; }

    bool requires_document() const override { return false; }

  protected:
    void perform(MainWidget& widget) override {
        auto last_opened_file = widget.get_last_opened_file_checksum();
        if (last_opened_file) {
            widget.open_document_with_hash(last_opened_file.value());
        }
    }
};
