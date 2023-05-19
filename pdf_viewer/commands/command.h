#pragma once

#include <string>
#include <optional>

// TODO: maybe remove
#include "../utils.h"

class MainWidget;

enum class RequirementType { Text, Symbol, File, Rect };

struct Requirement {
    RequirementType type;
    std::string name;
};

class Command {
  public:
    virtual std::optional<Requirement> next_requirement(MainWidget& widget);

    virtual void set_text_requirement(std::string value);
    virtual void set_symbol_requirement(char value);
    virtual void set_file_requirement(std::string value);
    virtual void set_rect_requirement(fz_rect value);
    virtual void set_num_repeats(int nr);
    virtual std::vector<char> special_symbols() const;
    virtual void pre_perform(MainWidget& widget);
    virtual bool pushes_state() const;
    virtual bool requires_document() const;

    virtual void run(MainWidget& widget);
    virtual std::string get_name() const;

    virtual ~Command() = default;

  protected:
    virtual void perform(MainWidget& widget) = 0;

    int num_repeats_ = 1;
};
