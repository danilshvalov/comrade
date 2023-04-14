#pragma once

#include "symbol_command.h"

extern std::wstring SEARCH_URLS[26];

class ExternalSearchCommand : public SymbolCommand {
  public:
    std::string get_name() const override { return "external_search"; }

  protected:
    void perform(MainWidget& widget) override {
        if ((symbol >= 'a') && (symbol <= 'z')) {
            if (SEARCH_URLS[symbol - 'a'].size() > 0) {
                search_custom_engine(
                    widget.selected_text, SEARCH_URLS[symbol - 'a']
                );
            } else {
                std::wcout << L"No search engine defined for symbol " << symbol
                           << std::endl;
            }
        }
    }
};
