#pragma once

#include "symbol_command.h"

class ExternalSearchCommand : public SymbolCommand {
  public:
    std::string get_name() const override { return "external_search"; }

  protected:
    void perform(MainWidget& widget) override {
        const Config& config = Config::instance();
        if ((symbol >= 'a') && (symbol <= 'z')) {
            if (config.SEARCH_URLS[symbol - 'a'].size() > 0) {
                search_custom_engine(
                    widget.selected_text, config.SEARCH_URLS[symbol - 'a']
                );
            } else {
                std::wcout << L"No search engine defined for symbol " << symbol
                           << std::endl;
            }
        }
    }
};
