#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <QKeyEvent>
#include <qstring.h>
#include <qstringlist.h>

#include "input.h"
#include "main_widget.h"
#include "ui.h"
#include "commands/all_commands.h"

void print_tree_node(InputParseTreeNode node) {
    if (node.requires_text) {
        std::wcout << "text node" << std::endl;
        return;
    }
    if (node.requires_symbol) {
        std::wcout << "symbol node" << std::endl;
        return;
    }

    if (node.control_modifier) {
        std::wcout << "Ctrl+";
    }

    if (node.shift_modifier) {
        std::wcout << "Shift+";
    }

    if (node.alt_modifier) {
        std::wcout << "Alt+";
    }
    std::wcout << node.command << std::endl;
}

InputParseTreeNode parse_token(std::wstring token) {
    InputParseTreeNode res;

    if (token == L"sym") {
        res.requires_symbol = true;
        return res;
    }
    if (token == L"txt") {
        res.requires_text = true;
        return res;
    }

    std::vector<std::wstring> subcommands;
    split_key_string(token, L"-", subcommands);

    for (size_t i = 0; i < subcommands.size() - 1; i++) {
        if (subcommands[i] == L"C") {
            res.control_modifier = true;
        }

        if (subcommands[i] == L"S") {
            res.shift_modifier = true;
        }

        if (subcommands[i] == L"A") {
            res.alt_modifier = true;
        }
    }

    std::wstring command_string = subcommands[subcommands.size() - 1];
    if (command_string.size() == 1) {
        res.command = subcommands[subcommands.size() - 1][0];
    } else {

        if (int f_key = get_f_key(command_string)) {
            res.command = Qt::Key::Key_F1 - 1 + f_key;
        } else {

            std::map<std::wstring, Qt::Key> keymap_temp = {
                {L"up",        Qt::Key::Key_Up       },
                {L"down",      Qt::Key::Key_Down     },
                {L"left",      Qt::Key::Key_Left     },
                {L"right",     Qt::Key::Key_Right    },
                {L"backspace", Qt::Key::Key_Backspace},
                {L"space",     Qt::Key::Key_Space    },
                {L"pageup",    Qt::Key::Key_PageUp   },
                {L"pagedown",  Qt::Key::Key_PageDown },
                {L"home",      Qt::Key::Key_Home     },
                {L"end",       Qt::Key::Key_End      },
                {L"pagedown",  Qt::Key::Key_End      },
                {L"tab",       Qt::Key::Key_Tab      },
                {L"return",    Qt::Key::Key_Return   },
            };
            std::map<std::wstring, Qt::Key> keymap;

            for (auto item : keymap_temp) {
                keymap[item.first] = item.second;
                keymap[L"<" + item.first + L">"] = item.second;
            }

            res.command = keymap[command_string];
        }
    }

    return res;
}

void get_tokens(std::wstring line, std::vector<std::wstring>& tokens) {
    std::wstring stack;

    int stack_depth = 0;

    for (wchar_t c : line) {
        if (stack_depth && (c != '>') && (c != '<')) {
            stack.push_back(c);
        } else if (c == '>') {
            stack_depth--;
            if (stack_depth == 0) {
                tokens.push_back(stack);
                stack.clear();
            } else {
                stack.push_back(c);
            }
        } else if (c == '<') {
            if (stack_depth) {
                stack.push_back(c);
            }
            stack_depth++;
        } else {
            tokens.push_back(std::wstring(1, c));
        }
    }
}

InputParseTreeNode* parse_lines(
    InputParseTreeNode* root,
    const std::vector<std::wstring>& lines,
    const std::vector<std::vector<std::string>>& command_names,
    const std::vector<std::wstring>& command_file_names,
    const std::vector<int>& command_line_numbers
) {

    for (size_t j = 0; j < lines.size(); j++) {
        std::wstring line = lines[j];

        // for example convert "<a-<space>> to ["a", "space"]
        std::vector<std::wstring> tokens;
        get_tokens(line, tokens);

        InputParseTreeNode* parent_node = root;

        for (size_t i = 0; i < tokens.size(); i++) {
            InputParseTreeNode node = parse_token(tokens[i]);
            bool existing_node = false;
            for (InputParseTreeNode* child : parent_node->children) {
                if (child->is_same(&node)) {
                    parent_node = child;
                    existing_node = true;
                    break;
                }
            }
            if (!existing_node) {
                if ((tokens[i] != L"sym") && (tokens[i] != L"txt")) {
                    if (parent_node->is_final) {
                        std::wcout << L"Warning: key defined in "
                                   << command_file_names[j] << L":"
                                   << command_line_numbers[j] << L" for "
                                   << utf8_decode(command_names[j][0])
                                   << L" is unreachable, shadowed by final key "
                                      L"sequence "
                                      L"defined in "
                                   << parent_node->defining_file_path << L":"
                                   << parent_node->defining_file_line << L"\n";
                    }
                    auto new_node = new InputParseTreeNode(node);
                    new_node->defining_file_line = command_line_numbers[j];
                    new_node->defining_file_path = command_file_names[j];
                    parent_node->children.push_back(new_node);
                    parent_node =
                        parent_node->children[parent_node->children.size() - 1];
                } else {
                    if (tokens[i] == L"sym") {
                        parent_node->requires_symbol = true;
                        parent_node->is_final = true;
                    }

                    if (tokens[i] == L"txt") {
                        parent_node->requires_text = true;
                        parent_node->is_final = true;
                    }
                }
            } else if (((size_t)i == (tokens.size() - 1)) &&
                       (Config::instance().application.warn_about_user_key_override ||
                        (command_file_names[j].compare(parent_node->defining_file_path)) == 0)) {
                if ((parent_node->name.size() == 0) ||
                    parent_node->name[0].compare(command_names[j][0]) != 0) {

                    std::wcout << L"Warning: key defined in "
                               << parent_node->defining_file_path << L":"
                               << parent_node->defining_file_line
                               << L" overwritten by " << command_file_names[j]
                               << L":" << command_line_numbers[j];
                    if (parent_node->name.size() > 0) {
                        std::wcout << L". Overriding command: " << line
                                   << L": replacing "
                                   << utf8_decode(parent_node->name[0])
                                   << L" with "
                                   << utf8_decode(command_names[j][0]);
                    }
                    std::wcout << L"\n";
                }
            }
            if ((size_t)i == (tokens.size() - 1)) {
                parent_node->is_final = true;
                parent_node->name.clear();
                parent_node->defining_file_line = command_line_numbers[j];
                parent_node->defining_file_path = command_file_names[j];
                for (size_t k = 0; k < command_names[j].size(); k++) {
                    parent_node->name.push_back(command_names[j][k]);
                }
            } else {
                if (parent_node->is_final && (parent_node->name.size() > 0)) {
                    std::wcout
                        << L"Warning: unmapping "
                        << utf8_decode(parent_node->name[0]) << L" because of "
                        << utf8_decode(command_names[j][0]) << L" which uses "
                        << line << L"\n";
                }
                parent_node->is_final = false;
            }
        }
    }

    return root;
}

InputParseTreeNode* parse_lines(
    const std::vector<std::wstring>& lines,
    const std::vector<std::vector<std::string>>& command_names,
    const std::vector<std::wstring>& command_file_names,
    const std::vector<int>& command_line_numbers
) {
    // parse key configs into a trie where leaves are annotated with the name of
    // the command

    InputParseTreeNode* root = new InputParseTreeNode;
    root->is_root = true;

    parse_lines(
        root, lines, command_names, command_file_names, command_line_numbers
    );

    return root;
}

std::vector<std::string> parse_command_name(const std::wstring& command_names) {
    QStringList parts = QString::fromStdWString(command_names).split(';');
    std::vector<std::string> res;
    for (int i = 0; i < parts.size(); i++) {
        res.push_back(parts.at(i).toStdString());
    }
    return res;
}

void get_keys_file_lines(
    const fs::path& file_path,
    std::vector<std::vector<std::string>>& command_names,
    std::vector<std::wstring>& command_keys,
    std::vector<std::wstring>& command_files,
    std::vector<int>& command_line_numbers
) {

    std::ifstream infile(file_path);

    int line_number = 0;
    std::wstring default_path_name = file_path.generic_wstring();
    while (infile.good()) {
        line_number++;
        std::string line_;
        std::wstring line;
        std::getline(infile, line_);
        line = utf8_decode(line_);

        if (line.size() == 0 || line[0] == '#') {
            continue;
        }
        std::wstringstream ss(line);
        std::wstring command_name;
        std::wstring command_key;
        ss >> command_name >> command_key;
        // command_names.push_back(utf8_encode(command_name));
        command_names.push_back(parse_command_name(command_name));
        command_keys.push_back(command_key);
        command_files.push_back(default_path_name);
        command_line_numbers.push_back(line_number);
    }

    infile.close();
}

InputParseTreeNode* parse_key_config_files(
    const fs::path& default_path, const std::vector<fs::path>& user_paths
) {

    std::wifstream default_infile(default_path);

    std::vector<std::vector<std::string>> command_names;
    std::vector<std::wstring> command_keys;
    std::vector<std::wstring> command_files;
    std::vector<int> command_line_numbers;

    get_keys_file_lines(
        default_path, command_names, command_keys, command_files,
        command_line_numbers
    );
    for (auto upath : user_paths) {
        get_keys_file_lines(
            upath, command_names, command_keys, command_files,
            command_line_numbers
        );
    }

    return parse_lines(
        command_keys, command_names, command_files, command_line_numbers
    );
}

InputHandler::InputHandler(
    const fs::path& default_path,
    const std::vector<fs::path>& user_paths,
    CommandManager* cm
) {
    command_manager = cm;
    user_key_paths = user_paths;
    reload_config_files(default_path, user_paths);
}

void InputHandler::reload_config_files(
    const fs::path& default_config_path,
    const std::vector<fs::path>& user_config_paths
) {
    delete_current_parse_tree(root);
    root = parse_key_config_files(default_config_path, user_config_paths);
    current_node = root;
}

bool is_digit(int key) {
    return key >= Qt::Key::Key_0 && key <= Qt::Key::Key_9;
}

std::vector<std::unique_ptr<Command>> InputHandler::handle_key(
    QKeyEvent* key_event,
    bool shift_pressed,
    bool control_pressed,
    bool alt_pressed,
    int* num_repeats
) {
    std::vector<std::unique_ptr<Command>> res;

    int key = 0;
    std::vector<QString> special_texts = {"\b", "\t", " ", "\r", "\n"};
    if (((key_event->key() >= 'A') && (key_event->key() <= 'Z')) ||
        ((key_event->text().size() > 0) &&
         (std::find(
              special_texts.begin(), special_texts.end(), key_event->text()
          ) == special_texts.end()))) {
        if (!control_pressed && !alt_pressed) {
            // shift is already handled in the returned text
            shift_pressed = false;
            std::wstring text = key_event->text().toStdWString();
            key = key_event->text().toStdWString()[0];
        } else {
            key = key_event->key();
            if (key >= 'A' && key <= 'Z') {
                key = key - 'A' + 'a';
            }
        }
    } else {
        key = key_event->key();

        if (key == Qt::Key::Key_Backtab) {
            key = Qt::Key::Key_Tab;
        }
    }

    if (current_node == root && is_digit(key)) {
        if (!(key == '0' && (number_stack.size() == 0)) && (!control_pressed) &&
            (!shift_pressed) && (!alt_pressed)) {
            number_stack.push_back('0' + key - Qt::Key::Key_0);
            return {};
        }
    }

    for (InputParseTreeNode* child : current_node->children) {
        // if (child->command == key && child->shift_modifier == shift_pressed
        // && child->control_modifier == control_pressed){
        if (child->matches(key, shift_pressed, control_pressed, alt_pressed)) {
            if (child->is_final == true) {
                current_node = root;
                // cout << child->name << endl;
                *num_repeats = 0;
                if (number_stack.size() > 0) {
                    *num_repeats = atoi(number_stack.c_str());
                    number_stack.clear();
                }

                // return command_manager.get_command_with_name(child->name);
                for (size_t i = 0; i < child->name.size(); i++) {
                    res.push_back(
                        command_manager->get_command_with_name(child->name[i])
                    );
                }
                return res;
            } else {
                current_node = child;
                return {};
            }
        }
    }
    std::wcout << "Warning: invalid command (key:" << (char)key
               << "); resetting to root" << std::endl;
    number_stack.clear();
    current_node = root;
    return {};
}

void InputHandler::delete_current_parse_tree(InputParseTreeNode* node_to_delete
) {
    bool is_root = false;

    if (node_to_delete != nullptr) {
        is_root = node_to_delete->is_root;

        for (size_t i = 0; i < node_to_delete->children.size(); i++) {
            delete_current_parse_tree(node_to_delete->children[i]);
        }
        delete node_to_delete;
    }

    if (is_root) {
        root = nullptr;
    }
}

bool InputParseTreeNode::is_same(const InputParseTreeNode* other) {
    return (command == other->command) &&
           (shift_modifier == other->shift_modifier) &&
           (control_modifier == other->control_modifier) &&
           (alt_modifier == other->alt_modifier) &&
           (requires_symbol == other->requires_symbol) &&
           (requires_text == other->requires_text);
}

bool InputParseTreeNode::matches(int key, bool shift, bool ctrl, bool alt) {
    return (key == this->command) && (shift == this->shift_modifier) &&
           (ctrl == this->control_modifier) && (alt == this->alt_modifier);
}

std::optional<fs::path> InputHandler::get_or_create_user_keys_path() {
    if (user_key_paths.size() == 0) {
        return {};
    }

    for (int i = user_key_paths.size() - 1; i >= 0; i--) {
        if (fs::exists(user_key_paths[i])) {
            return user_key_paths[i];
        }
    }

    fs::create_directories(user_key_paths.back().parent_path());
    create_file_if_not_exists(user_key_paths.back().generic_wstring());
    return user_key_paths.back();
}

std::unordered_map<std::string, std::vector<std::string>> InputHandler::
    get_command_key_mappings() const {
    std::unordered_map<std::string, std::vector<std::string>> res;
    std::vector<InputParseTreeNode*> prefix;
    add_command_key_mappings(root, res, prefix);
    return res;
}

void InputHandler::add_command_key_mappings(
    InputParseTreeNode* thisroot,
    std::unordered_map<std::string, std::vector<std::string>>& map,
    std::vector<InputParseTreeNode*> prefix
) const {

    if (thisroot->is_final) {
        if (thisroot->name.size() == 1) {
            map[thisroot->name[0]].push_back(
                get_key_string_from_tree_node_sequence(prefix)
            );
        } else if (thisroot->name.size() > 1) {
            for (const auto& name : thisroot->name) {
                map[name].push_back(
                    "{" + get_key_string_from_tree_node_sequence(prefix) + "}"
                );
            }
        }
    } else {
        for (size_t i = 0; i < thisroot->children.size(); i++) {
            prefix.push_back(thisroot->children[i]);
            add_command_key_mappings(thisroot->children[i], map, prefix);
            prefix.pop_back();
        }
    }
}

std::string InputHandler::get_key_name_from_key_code(int key_code) const {
    std::string result;
    std::map<int, std::string> keymap = {
        {Qt::Key::Key_Up,        "up"       },
        {Qt::Key::Key_Down,      "down"     },
        {Qt::Key::Key_Left,      "left"     },
        {Qt::Key::Key_Right,     "right"    },
        {Qt::Key::Key_Backspace, "backspace"},
        {Qt::Key::Key_Space,     "space"    },
        {Qt::Key::Key_PageUp,    "pageup"   },
        {Qt::Key::Key_PageDown,  "pagedown" },
        {Qt::Key::Key_Home,      "home"     },
        {Qt::Key::Key_End,       "end"      },
        {Qt::Key::Key_Tab,       "tab"      },
        {Qt::Key::Key_Backtab,   "tab"      },
    };

    // if (((key_code <= 'z') && (key_code >= 'a')) || ((key_code <= 'Z') &&
    // (key_code >= 'A'))) {
    if (key_code < 127) {
        result.push_back(key_code);
        return result;
    } else if (keymap.find(key_code) != keymap.end()) {
        return "<" + keymap[key_code] + ">";
    } else if ((key_code >= Qt::Key::Key_F1) && (key_code <= Qt::Key::Key_F35)) {
        int f_number = 1 + (key_code - Qt::Key::Key_F1);
        return "<f" + QString::number(f_number).toStdString() + ">";
    } else {
        return "UNK";
    }
}

std::string InputHandler::get_key_string_from_tree_node_sequence(
    const std::vector<InputParseTreeNode*> seq
) const {
    std::string res;
    for (size_t i = 0; i < seq.size(); i++) {
        if (seq[i]->alt_modifier || seq[i]->shift_modifier ||
            seq[i]->control_modifier) {
            res += "<";
        }
        std::string current_key_command_name =
            get_key_name_from_key_code(seq[i]->command);

        if (seq[i]->alt_modifier) {
            res += "A-";
        }
        if (seq[i]->control_modifier) {
            res += "C-";
        }
        if (seq[i]->shift_modifier) {
            res += "S-";
        }
        res += current_key_command_name;
        if (seq[i]->alt_modifier || seq[i]->shift_modifier ||
            seq[i]->control_modifier) {
            res += ">";
        }
    }
    return res;
}

std::vector<fs::path> InputHandler::get_all_user_keys_paths() {
    std::vector<fs::path> res;

    for (int i = user_key_paths.size() - 1; i >= 0; i--) {
        if (fs::exists(user_key_paths[i])) {
            res.push_back(user_key_paths[i]);
        }
    }

    return res;
}
