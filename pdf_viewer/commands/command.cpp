#include "command.h"

#include "../main_widget.h"

std::optional<Requirement> Command::next_requirement(MainWidget& widget) {
    return {};
}

void Command::set_text_requirement(std::wstring value) {}

void Command::set_symbol_requirement(char value) {}

void Command::set_file_requirement(std::wstring value) {}

void Command::set_rect_requirement(fz_rect value) {}

bool Command::pushes_state() const { return false; }

bool Command::requires_document() const { return true; }

void Command::set_num_repeats(int nr) { num_repeats_ = nr; }

void Command::pre_perform(MainWidget& widget) {}

void Command::run(MainWidget& widget) {
    if (this->requires_document() &&
        !(widget.main_document_view_has_document())) {
        return;
    }
    perform(widget);
}

std::vector<char> Command::special_symbols() const { return {}; }

std::string Command::get_name() const { return ""; }
