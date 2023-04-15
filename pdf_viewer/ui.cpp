#include "ui.h"

#include <QFileDialog>

extern std::wstring DEFAULT_OPEN_FILE_PATH;

std::wstring select_command_file_name(std::string command_name) {
    if (command_name == "open_document") {
        return select_document_file_name();
    } else if (command_name == "source_config") {
        return select_any_file_name();
    } else {
        return select_any_file_name();
    }
}

std::wstring select_document_file_name() {
    if (DEFAULT_OPEN_FILE_PATH.size() == 0) {

        QString file_name = QFileDialog::getOpenFileName(
            nullptr, "Select Document", "", "Documents (*.pdf *.epub *.cbz)"
        );
        return file_name.toStdWString();
    } else {

        QFileDialog fd = QFileDialog(
            nullptr, "Select Document", "", "Documents (*.pdf *.epub *.cbz)"
        );
        fd.setDirectory(QString::fromStdWString(DEFAULT_OPEN_FILE_PATH));
        if (fd.exec()) {

            QString file_name = fd.selectedFiles().first();
            return file_name.toStdWString();
        } else {
            return L"";
        }
    }
}

std::wstring select_json_file_name() {
    QString file_name = QFileDialog::getOpenFileName(
        nullptr, "Select Document", "", "Documents (*.json )"
    );
    return file_name.toStdWString();
}

std::wstring select_any_file_name() {
    QString file_name =
        QFileDialog::getOpenFileName(nullptr, "Select File", "", "Any (*)");
    return file_name.toStdWString();
}

std::wstring select_new_json_file_name() {
    QString file_name = QFileDialog::getSaveFileName(
        nullptr, "Select Document", "", "Documents (*.json )"
    );
    return file_name.toStdWString();
}

std::wstring select_new_pdf_file_name() {
    QString file_name = QFileDialog::getSaveFileName(
        nullptr, "Select Document", "", "Documents (*.pdf )"
    );
    return file_name.toStdWString();
}

std::vector<ConfigFileChangeListener*>
    ConfigFileChangeListener::registered_listeners;

ConfigFileChangeListener::ConfigFileChangeListener() {
    registered_listeners.push_back(this);
}

ConfigFileChangeListener::~ConfigFileChangeListener() {
    registered_listeners.erase(std::find(
        registered_listeners.begin(), registered_listeners.end(), this
    ));
}

void ConfigFileChangeListener::notify_config_file_changed(
    ConfigManager* new_config_manager
) {
    for (auto* it : ConfigFileChangeListener::registered_listeners) {
        it->on_config_file_changed(new_config_manager);
    }
}
