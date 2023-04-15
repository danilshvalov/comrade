#pragma once

#include "ui/selectors/base_selector.h"
#include "ui/selectors/command_selector.h"
#include "ui/selectors/file_selector.h"
#include "ui/selectors/filtered_tree_selector.h"
#include "ui/selectors/filtered_window_selector.h"
#include "ui/selectors/filtered_table_window_selector.h"
#include "config.h"

class ConfigFileChangeListener {
    static std::vector<ConfigFileChangeListener*> registered_listeners;

  public:
    ConfigFileChangeListener();
    ~ConfigFileChangeListener();
    virtual void on_config_file_changed(ConfigManager* new_config_manager) = 0;
    static void notify_config_file_changed(ConfigManager* new_config_manager);
};

std::wstring select_document_file_name();
std::wstring select_json_file_name();
std::wstring select_any_file_name();
std::wstring select_command_file_name(std::string command_name);
std::wstring select_new_json_file_name();
std::wstring select_new_pdf_file_name();
