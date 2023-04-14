#pragma once

#include "command.h"
#include "../main_widget.h"

class OpenDocumentCommand : public Command {
  public:
    std::string get_name() const override { return "open_document"; }

    bool requires_document() const override { return false; }

    bool pushes_state() const override { return true; }

    std::optional<Requirement> next_requirement(MainWidget& widget) override {
        if (file_name.size() == 0) {
            return Requirement{RequirementType::File, "File"};
        } else {
            return {};
        }
    }

    void set_file_requirement(std::wstring value) override {
        file_name = value;
    }

  protected:
    void perform(MainWidget& widget) override {
        widget.open_document(file_name);
    }

    std::wstring file_name;
};
