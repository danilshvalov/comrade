#include "config.h"
#include "utils.h"

#include <cassert>
#include <map>
#include <ostream>
#include <qdir.h>

template <typename T>
void* generic_deserializer(std::istream& stream, void* res_) {
    T* res = static_cast<T*>(res_);
    stream >> *res;
    return res;
}

void int_serializer(void* int_pointer, std::ostream& stream) {
    stream << *(int*)int_pointer;
}

void bool_serializer(void* bool_pointer, std::ostream& stream) {
    stream << *(bool*)bool_pointer;
}

void string_serializer(void* string_pointer, std::ostream& stream) {
    stream << *(std::string*)string_pointer;
}

void* string_deserializer(std::istream& stream, void* res_) {
    assert(res_ != nullptr);
    // delete res_;

    std::string* res = static_cast<std::string*>(res_);
    res->clear();
    std::getline(stream, *res);
    while (iswspace((*res)[0])) {
        res->erase(res->begin());
    }
    return res;
}

template <int N, typename T>
void vec_n_serializer(void* vec_n_pointer, std::ostream& stream) {
    for (int i = 0; i < N; i++) {
        stream << *(((T*)vec_n_pointer) + i);
    }
}

template <int N, typename T>
void* vec_n_deserializer(std::istream& stream, void* res_) {
    assert(res_ != nullptr);
    T* res = (T*)res_;
    if (res == nullptr) {
        res = new T[N];
    }
    for (int i = 0; i < N; i++) {
        stream >> *(res + i);
    }

    return res;
}

template <int N>
void* colorn_deserializer(std::istream& stream, void* res_) {

    assert(res_ != nullptr);
    float* res = (float*)res_;
    if (res == nullptr) {
        res = new float[N];
    }
    while (std::isspace(stream.peek())) {
        stream.ignore();
    }
    if (stream.peek() == '#') {
        std::string rest;
        std::getline(stream, rest);
        hexademical_to_normalized_color(rest, res, N);
    } else {
        for (int i = 0; i < N; i++) {
            stream >> *(res + i);
        }
    }

    return res;
}

void float_serializer(void* float_pointer, std::ostream& stream) {
    stream << *(float*)float_pointer;
}

void* ConfigEntry::get_value() { return value; }

ConfigEntry* ConfigManager::get_mut_config_with_name(std::string config_name) {
    for (auto& it : configs) {
        if (it.name == config_name)
            return &it;
    }
    return nullptr;
}

bool ensure_between_0_and_1(const QStringList& parts) {
    bool ok = false;
    for (int i = 0; i < parts.size(); i++) {
        int val = parts.at(i).toFloat(&ok);
        if (!ok) {
            std::cout << "Error: invalid value for color component: "
                      << parts.at(i).toStdString() << "\n";
            return false;
        }
        if (val < 0 || val > 1) {
            std::cout << "Error: invalid value for color component (must be "
                         "between 0 and 1): "
                      << parts.at(i).toStdString() << "\n";
            return false;
        }
    }
    return true;
}

template <int N>
bool colorn_validator(const std::string& str) {
    QString qstr = QString::fromStdString(str);
    auto parts =
        qstr.trimmed().split(' ', Qt::SplitBehaviorFlags::SkipEmptyParts);
    if (parts.size() != N) {
        if (parts.size() == 1) {
            if (parts.at(0).at(0) == '#') {
                if (parts.at(0).size() == (1 + 2 * N)) {
                    return true;
                }
            }
        }
        std::cout << "Error: required 3 values for color, but got "
                  << parts.size() << "\n";
        return false;
    }
    if (!ensure_between_0_and_1(parts)) {
        return false;
    }
    return true;
}

bool bool_validator(const std::string& str) {
    QString qstr = QString::fromStdString(str);
    auto parts =
        qstr.trimmed().split(' ', Qt::SplitBehaviorFlags::SkipEmptyParts);
    std::string msg = "Bool values should be either 0 or 1, but got ";
    if (parts.size() != 1) {
        std::cout << msg << str << "\n";
        return false;
    }
    if (parts.at(0).trimmed().toStdString() == "0" ||
        parts.at(0).trimmed().toStdString() == "1") {
        return true;
    }
    std::cout << msg << str << "\n";
    return false;
}

ConfigManager::ConfigManager(
    const fs::path& default_path,
    const fs::path& auto_path,
    const std::vector<fs::path>& user_paths
) {
    Config& config = Config::instance();

    user_config_paths = user_paths;
    auto ivec2_serializer = vec_n_serializer<2, int>;
    auto ivec2_deserializer = vec_n_deserializer<2, int>;
    auto fvec2_serializer = vec_n_serializer<2, float>;
    auto fvec2_deserializer = vec_n_deserializer<2, float>;
    auto vec3_serializer = vec_n_serializer<3, float>;
    auto vec4_serializer = vec_n_serializer<4, float>;
    auto vec4_deserializer = vec_n_deserializer<4, float>;
    auto float_deserializer = generic_deserializer<float>;
    auto int_deserializer = generic_deserializer<int>;
    auto bool_deserializer = generic_deserializer<bool>;
    auto color3_deserializer = colorn_deserializer<3>;
    auto color4_deserializer = colorn_deserializer<4>;
    auto color_3_validator = colorn_validator<3>;
    auto color_4_validator = colorn_validator<4>;

    configs.push_back(
        {"text_highlight_color", DEFAULT_TEXT_HIGHLIGHT_COLOR, vec3_serializer,
         color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"vertical_line_color", DEFAULT_VERTICAL_LINE_COLOR, vec4_serializer,
         color4_deserializer, color_4_validator}
    );
    configs.push_back(
        {"visual_mark_color", DEFAULT_VERTICAL_LINE_COLOR, vec4_serializer,
         color4_deserializer, color_4_validator}
    );
    configs.push_back(
        {"search_highlight_color", DEFAULT_SEARCH_HIGHLIGHT_COLOR,
         vec3_serializer, color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"unselected_search_highlight_color",
         config.theme.UNSELECTED_SEARCH_HIGHLIGHT_COLOR, vec3_serializer,
         color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"link_highlight_color", DEFAULT_LINK_HIGHLIGHT_COLOR, vec3_serializer,
         color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"synctex_highlight_color", DEFAULT_SYNCTEX_HIGHLIGHT_COLOR,
         vec3_serializer, color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"foreground_color", config.document.foreground, vec3_serializer,
         color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"background_color", config.document.background, vec3_serializer,
         color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"window_background_color", config.background.background,
         vec3_serializer, color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"custom_color_contrast", &config.CUSTOM_COLOR_CONTRAST,
         float_serializer, float_deserializer, nullptr}
    );
    configs.push_back(
        {"google_scholar_address", &config.GOOGLE_SCHOLAR_ADDRESS,
         string_serializer, string_deserializer, nullptr}
    );
    configs.push_back(
        {"item_list_prefix", &config.ITEM_LIST_PREFIX, string_serializer,
         string_deserializer, nullptr}
    );
    configs.push_back(
        {"inverse_search_command", &config.INVERSE_SEARCH_COMMAND,
         string_serializer, string_deserializer, nullptr}
    );
    configs.push_back(
        {"libgen_address", &config.LIBGEN_ADDRESS, string_serializer,
         string_deserializer, nullptr}
    );
    configs.push_back(
        {"zoom_inc_factor", &config.ZOOM_INC_FACTOR, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"vertical_move_amount", &config.VERTICAL_MOVE_AMOUNT, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"horizontal_move_amount", &config.HORIZONTAL_MOVE_AMOUNT,
         float_serializer, float_deserializer, nullptr}
    );
    configs.push_back(
        {"move_screen_percentage", &config.MOVE_SCREEN_PERCENTAGE,
         float_serializer, float_deserializer, nullptr}
    );
    configs.push_back(
        {"move_screen_ratio", &config.MOVE_SCREEN_PERCENTAGE, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"flat_toc", &config.FLAT_TABLE_OF_CONTENTS, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"should_use_multiple_monitors",
         &config.application.use_multiple_monitors, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"should_load_tutorial_when_no_other_file",
         &config.application.load_tutorial_when_no_other_file, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"should_launch_new_instance", &config.application.launch_new_instance,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"should_launch_new_window", &config.application.launch_new_window,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"should_draw_unrendered_pages",
         &config.application.draw_unrendered_pages, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"sort_bookmarks_by_location", &config.SORT_BOOKMARKS_BY_LOCATION,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"shared_database_path", &config.SHARED_DATABASE_PATH,
         string_serializer, string_deserializer, nullptr}
    );
    configs.push_back(
        {"hover_overview", &config.HOVER_OVERVIEW, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"visual_mark_next_page_fraction",
         &config.VISUAL_MARK_NEXT_PAGE_FRACTION, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"visual_mark_next_page_threshold",
         &config.VISUAL_MARK_NEXT_PAGE_THRESHOLD, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"ui_font", &config.UI_FONT_FACE_NAME, string_serializer,
         string_deserializer, nullptr}
    );
    configs.push_back(
        {"middle_click_search_engine", &config.MIDDLE_CLICK_SEARCH_ENGINE,
         string_serializer, string_deserializer, nullptr}
    );
    configs.push_back(
        {"shift_middle_click_search_engine",
         &config.SHIFT_MIDDLE_CLICK_SEARCH_ENGINE, string_serializer,
         string_deserializer, nullptr}
    );
    configs.push_back(
        {"startup_commands", &config.STARTUP_COMMANDS, string_serializer,
         string_deserializer, nullptr}
    );
    configs.push_back(
        {"font_size", &config.application.font_size, int_serializer,
         int_deserializer, nullptr}
    );
    configs.push_back(
        {"keyboard_select_font_size", &config.KEYBOARD_SELECT_FONT_SIZE,
         int_serializer, int_deserializer, nullptr}
    );
    configs.push_back(
        {"status_bar_font_size", &config.theme.status_bar.font_size,
         int_serializer, int_deserializer, nullptr}
    );
    configs.push_back(
        {"rerender_overview", &config.RERENDER_OVERVIEW, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"wheel_zoom_on_cursor", &config.WHEEL_ZOOM_ON_CURSOR, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"linear_filter", &config.LINEAR_TEXTURE_FILTERING, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"display_resolution_scale", &config.DISPLAY_RESOLUTION_SCALE,
         float_serializer, float_deserializer, nullptr}
    );
    configs.push_back(
        {"status_bar_color", config.theme.status_bar.background,
         vec3_serializer, color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"status_bar_text_color", config.theme.status_bar.foreground,
         vec3_serializer, color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"main_window_size", &config.MAIN_WINDOW_SIZE, ivec2_serializer,
         ivec2_deserializer, nullptr}
    );
    configs.push_back(
        {"helper_window_size", &config.HELPER_WINDOW_SIZE, ivec2_serializer,
         ivec2_deserializer, nullptr}
    );
    configs.push_back(
        {"main_window_move", &config.MAIN_WINDOW_MOVE, ivec2_serializer,
         ivec2_deserializer, nullptr}
    );
    configs.push_back(
        {"helper_window_move", &config.HELPER_WINDOW_MOVE, ivec2_serializer,
         ivec2_deserializer, nullptr}
    );
    configs.push_back(
        {"touchpad_sensitivity", &config.TOUCHPAD_SENSITIVITY, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"scrollview_sensitivity", &config.SCROLL_VIEW_SENSITIVITY,
         float_serializer, float_deserializer, nullptr}
    );
    configs.push_back(
        {"page_separator_width", &config.PAGE_SEPARATOR_WIDTH, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"page_separator_color", config.PAGE_SEPARATOR_COLOR, vec3_serializer,
         color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"single_main_window_size", &config.SINGLE_MAIN_WINDOW_SIZE,
         ivec2_serializer, ivec2_deserializer, nullptr}
    );
    configs.push_back(
        {"single_main_window_move", &config.SINGLE_MAIN_WINDOW_MOVE,
         ivec2_serializer, ivec2_deserializer, nullptr}
    );
    configs.push_back(
        {"fit_to_page_width_ratio", &config.FIT_TO_PAGE_WIDTH_RATIO,
         float_serializer, float_deserializer, nullptr}
    );
    configs.push_back(
        {"collapsed_toc", &config.SMALL_TOC, bool_serializer, bool_deserializer,
         bool_validator}
    );
    configs.push_back(
        {"ruler_mode", &config.RULER_MODE, bool_serializer, bool_deserializer,
         bool_validator}
    );
    configs.push_back(
        {"ruler_padding", &config.RULER_PADDING, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"ruler_x_padding", &config.RULER_X_PADDING, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"text_summary_url", &config.TEXT_HIGHLIGHT_URL, string_serializer,
         string_deserializer, nullptr}
    );
    configs.push_back(
        {"text_summary_should_refine",
         &config.TEXT_SUMMARY_HIGHLIGHT_SHOULD_REFINE, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"text_summary_should_fill", &config.TEXT_SUMMARY_HIGHLIGHT_SHOULD_FILL,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"text_summary_context_size", &config.TEXT_SUMMARY_CONTEXT_SIZE,
         int_serializer, int_deserializer, nullptr}
    );
    configs.push_back(
        {"use_heuristic_if_text_summary_not_available",
         &config.USE_HEURISTIC_IF_TEXT_SUMMARY_NOT_AVAILABLE, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"papers_folder_path", &config.PAPERS_FOLDER_PATH, string_serializer,
         string_deserializer, nullptr}
    );
    configs.push_back(
        {"enable_experimental_features", &config.ENABLE_EXPERIMENTAL_FEATURES,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"create_table_of_contents_if_not_exists",
         &config.CREATE_TABLE_OF_CONTENTS_IF_NOT_EXISTS, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"max_created_toc_size", &config.MAX_CREATED_TABLE_OF_CONTENTS_SIZE,
         int_serializer, int_deserializer, nullptr}
    );
    configs.push_back(
        {"force_custom_line_algorithm", &config.FORCE_CUSTOM_LINE_ALGORITHM,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"overview_size", &config.OVERVIEW_SIZE, fvec2_serializer,
         fvec2_deserializer, nullptr}
    );
    configs.push_back(
        {"overview_offset", &config.OVERVIEW_OFFSET, fvec2_serializer,
         fvec2_deserializer, nullptr}
    );
    configs.push_back(
        {"ignore_whitespace_in_presentation_mode",
         &config.IGNORE_WHITESPACE_IN_PRESENTATION_MODE, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"exact_highlight_select", &config.EXACT_HIGHLIGHT_SELECT,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"show_doc_path", &config.SHOW_DOC_PATH, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"fastread_opacity", &config.FASTREAD_OPACITY, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"should_warn_about_user_key_override",
         &config.application.warn_about_user_key_override, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"single_click_selects_words", &config.SINGLE_CLICK_SELECTS_WORDS,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"shift_click_command", &config.SHIFT_CLICK_COMMAND, string_serializer,
         string_deserializer, nullptr}
    );
    configs.push_back(
        {"control_click_command", &config.CONTROL_CLICK_COMMAND,
         string_serializer, string_deserializer, nullptr}
    );
    configs.push_back(
        {"shift_right_click_command", &config.SHIFT_RIGHT_CLICK_COMMAND,
         string_serializer, string_deserializer, nullptr}
    );
    configs.push_back(
        {"control_right_click_command", &config.CONTROL_RIGHT_CLICK_COMMAND,
         string_serializer, string_deserializer, nullptr}
    );
    configs.push_back(
        {"alt_click_command", &config.ALT_CLICK_COMMAND, string_serializer,
         string_deserializer, nullptr}
    );
    configs.push_back(
        {"alt_right_click_command", &config.ALT_RIGHT_CLICK_COMMAND,
         string_serializer, string_deserializer, nullptr}
    );
    configs.push_back(
        {"multiline_menus", &config.MULTILINE_MENUS, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"start_with_helper_window", &config.START_WITH_HELPER_WINDOW,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"prerender_next_page_presentation", &config.PRERENDER_NEXT_PAGE,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"emacs_mode_menus", &config.EMACS_MODE, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"highlight_middle_click", &config.HIGHLIGHT_MIDDLE_CLICK,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"hyperdrive_speed_factor", &config.HYPERDRIVE_SPEED_FACTOR,
         float_serializer, float_deserializer, nullptr}
    );
    configs.push_back(
        {"smooth_scroll_speed", &config.SMOOTH_SCROLL_SPEED, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"smooth_scroll_drag", &config.SMOOTH_SCROLL_DRAG, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"ignore_statusbar_in_presentation_mode",
         &config.IGNORE_STATUSBAR_IN_PRESENTATION_MODE, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"super_fast_search", &config.SUPER_FAST_SEARCH, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"show_closest_bookmark_in_statusbar",
         &config.SHOW_CLOSEST_BOOKMARK_IN_STATUSBAR, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"show_close_portal_in_statusbar",
         &config.SHOW_CLOSE_PORTAL_IN_STATUSBAR, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"prerendered_page_count", &config.PRERENDERED_PAGE_COUNT,
         int_serializer, int_deserializer, nullptr}
    );
    configs.push_back(
        {"case_sensitive_search", &config.CASE_SENSITIVE_SEARCH,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"show_document_name_in_statusbar",
         &config.SHOW_DOCUMENT_NAME_IN_STATUSBAR, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"ui_selected_background_color", config.theme.selected.background,
         vec3_serializer, color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"ui_selected_text_color", config.theme.selected.foreground,
         vec3_serializer, color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"ui_background_color", config.theme.status_bar.background,
         vec3_serializer, color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"ui_text_color", config.theme.status_bar.foreground, vec3_serializer,
         color3_deserializer, color_3_validator}
    );
    configs.push_back(
        {"numeric_tags", &config.NUMERIC_TAGS, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"highlight_links", &config.SHOULD_HIGHLIGHT_LINKS, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"should_highlight_unselected_search",
         &config.SHOULD_HIGHLIGHT_UNSELECTED_SEARCH, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"gamma", &config.application.gamma, float_serializer,
         float_deserializer, nullptr}
    );
    configs.push_back(
        {"debug", &config.application.debug, bool_serializer, bool_deserializer,
         bool_validator}
    );
    configs.push_back(
        {"highlight_delete_threshold", &config.HIGHLIGHT_DELETE_THRESHOLD,
         float_serializer, float_deserializer, nullptr}
    );
    configs.push_back(
        {"default_open_file_path", &config.DEFAULT_OPEN_FILE_PATH,
         string_serializer, string_deserializer, nullptr}
    );
    configs.push_back(
        {"status_bar_format", &config.STATUS_BAR_FORMAT, string_serializer,
         string_deserializer, nullptr}
    );
    configs.push_back(
        {"inverted_horizontal_scrolling", &config.INVERTED_HORIZONTAL_SCROLLING,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"toc_jump_align_top", &config.TOC_JUMP_ALIGN_TOP, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"keyboard_select_background_color",
         &config.KEYBOARD_SELECT_BACKGROUND_COLOR, vec4_serializer,
         vec4_deserializer, nullptr}
    );
    configs.push_back(
        {"keyboard_select_text_color", &config.KEYBOARD_SELECT_TEXT_COLOR,
         vec4_serializer, vec4_deserializer, nullptr}
    );
    configs.push_back(
        {"autocenter_visual_scroll", &config.AUTOCENTER_VISUAL_SCROLL,
         bool_serializer, bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"alphabetic_link_tags", &config.ALPHABETIC_LINK_TAGS, bool_serializer,
         bool_deserializer, bool_validator}
    );
    configs.push_back(
        {"vimtex_wsl_fix", &config.VIMTEX_WSL_FIX, bool_serializer,
         bool_deserializer, bool_validator}
    );

    std::string highlight_config_string = "highlight_color_a";
    std::string search_url_config_string = "search_url_a";
    std::string execute_command_config_string = "execute_command_a";

    for (char letter = 'a'; letter <= 'z'; letter++) {
        highlight_config_string[highlight_config_string.size() - 1] = letter;
        search_url_config_string[search_url_config_string.size() - 1] = letter;
        execute_command_config_string
            [execute_command_config_string.size() - 1] = letter;

        configs.push_back(
            {highlight_config_string,
             &config.theme.highlight_colors[(letter - 'a') * 3],
             vec3_serializer, color3_deserializer, color_3_validator}
        );
        configs.push_back(
            {search_url_config_string, &config.SEARCH_URLS[letter - 'a'],
             string_serializer, string_deserializer, nullptr}
        );
        configs.push_back(
            {execute_command_config_string,
             &config.EXECUTE_COMMANDS[letter - 'a'], string_serializer,
             string_deserializer, nullptr}
        );
    }

    deserialize(default_path, auto_path, user_paths);
}

void ConfigManager::deserialize_file(
    const fs::path& file_path, bool warn_if_not_exists
) {
    Config& config = Config::instance();
    std::string line;
    std::ifstream default_file(file_path);
    int line_number = 0;

    if (warn_if_not_exists && (!default_file.good())) {
        std::cout << "Error: Could not open config file " << file_path
                  << std::endl;
    }

    while (std::getline(default_file, line)) {
        line_number++;

        if (line.size() == 0 || line[0] == '#') {
            continue;
        }

        std::stringstream ss{line};
        std::string conf_name;
        ss >> conf_name;
        // special handling for new_command
        if (conf_name == "source") {
            std::string path;
            std::getline(ss, path);
            path = strip_string(path);
            if (path.size() > 0) {
                deserialize_file(path, true);
            }
        } else if ((conf_name == "new_command") || (conf_name == "new_macro")) {
            std::string config_value;
            std::getline(ss, config_value);
            config_value = strip_string(config_value);
            int space_index = config_value.find(" ");
            std::string new_command_name = config_value.substr(0, space_index);
            if (new_command_name[0] == '_') {
                std::string command_value = config_value.substr(
                    space_index + 1, config_value.size() - space_index - 1
                );
                if (conf_name == "new_command") {
                    config.ADDITIONAL_COMMANDS[new_command_name] =
                        command_value;
                }
                if (conf_name == "new_macro") {
                    config.ADDITIONAL_MACROS[new_command_name] = command_value;
                }
            }
        } else {

            ConfigEntry* conf = get_mut_config_with_name(conf_name);
            if (conf == nullptr) {
                std::cout << "Error: " << conf_name
                          << " is not a valid configuration name\n";
                continue;
            }

            std::string config_value;
            std::getline(ss, config_value);

            std::stringstream config_value_stream(config_value);

            if ((conf != nullptr) && (conf->validator != nullptr)) {
                if (!conf->validator(config_value)) {
                    std::cout << "Error in config file " << file_path
                              << " at line " << line_number << " : " << line
                              << "\n";
                    continue;
                }
            }

            if (conf) {
                auto deserialization_result =
                    conf->deserialize(config_value_stream, conf->value);
                if (deserialization_result != nullptr) {
                    conf->value = deserialization_result;
                } else {
                    std::cout << "Error in config file " << file_path
                              << " at line " << line_number << " : " << line
                              << "\n";
                }
            }
        }
    }
    default_file.close();
}

void ConfigManager::deserialize(
    const fs::path& default_file_path,
    const fs::path& auto_path,
    const std::vector<fs::path>& user_file_paths
) {
    Config::instance().ADDITIONAL_COMMANDS.clear();

    deserialize_file(default_file_path);
    deserialize_file(auto_path);

    for (const auto& user_file_path : user_file_paths) {
        deserialize_file(user_file_path);
    }
}

std::optional<fs::path> ConfigManager::get_or_create_user_config_file() {
    if (user_config_paths.size() == 0) {
        return {};
    }

    for (int i = user_config_paths.size() - 1; i >= 0; i--) {
        if (fs::exists(user_config_paths[i])) {
            return user_config_paths[i];
        }
    }
    fs::create_directories(user_config_paths.back().parent_path());
    create_file_if_not_exists(user_config_paths.back().generic_string());
    return user_config_paths.back();
}

std::vector<fs::path> ConfigManager::get_all_user_config_files() {
    std::vector<fs::path> res;
    for (int i = user_config_paths.size() - 1; i >= 0; i--) {
        if (fs::exists(user_config_paths[i])) {
            res.push_back(user_config_paths[i]);
        }
    }
    return res;
}

std::vector<ConfigEntry> ConfigManager::get_configs() { return configs; }

void ConfigManager::deserialize_config(
    std::string config_name, std::string config_value
) {

    std::stringstream config_value_stream(config_value);
    ConfigEntry* conf = get_mut_config_with_name(config_name);
    auto deserialization_result =
        conf->deserialize(config_value_stream, conf->value);
    if (deserialization_result != nullptr) {
        conf->value = deserialization_result;
    }
}

void Config::configure_paths() {
    fs::path parent_path(QCoreApplication::applicationDirPath().toStdString());
    std::string exe_path =
        QCoreApplication::applicationFilePath().toStdString();

    shader_path = parent_path / "shaders";

#ifdef Q_OS_MACOS
    fs::path mac_home_path(QDir::homePath().toStdString());
    fs::path mac_standard_config_path = mac_home_path / ".config" / "sioyek";
    user_keys_paths.push_back(mac_standard_config_path / "keys_user.config");
    user_config_paths.push_back(mac_standard_config_path / "prefs_user.config");
#endif

#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    QStringList all_config_paths =
        QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    for (int i = all_config_paths.size() - 1; i >= 0; i--) {
        user_config_paths.push_back(
            fs::path(all_config_paths.at(i).toStdString()) / "prefs_user.config"
        );
        user_keys_paths.push_back(
            fs::path(all_config_paths.at(i).toStdString()) / "keys_user.config"
        );
    }

#ifdef LINUX_STANDARD_PATHS
    fs::path home_path(QDir::homePath().toStdString());
    fs::path standard_data_path = home_path / ".local" / "share" / "sioyek";
    fs::path standard_config_path = "/etc/sioyek";
    fs::path read_only_data_path = "/usr/share/sioyek";
    fs::create_directories(standard_data_path);

    default_config_path = standard_config_path / "prefs.config";
    default_keys_path = standard_config_path / "keys.config";

    database_file_path = standard_data_path / "test.db";
    local_database_file_path = standard_data_path / "local.db";
    global_database_file_path = standard_data_path / "shared.db";
    tutorial_path = read_only_data_path / "tutorial.pdf";
    last_opened_file_address_path =
        standard_data_path / "last_document_path.txt";
    shader_path = read_only_data_path / "shaders";
#else
    char* APPDIR = std::getenv("XDG_CONFIG_HOME");
    fs::path linux_home_path = QDir::homePath().toStdString();

    if (!APPDIR) {
        APPDIR = std::getenv("HOME");
    }

    fs::path standard_data_path = APPDIR;
    standard_data_path = standard_data_path / ".local" / "share" / "Sioyek";
    fs::create_directories(standard_data_path);

    default_config_path = parent_path / "prefs.config";
    user_config_paths.push_back(standard_data_path / "prefs_user.config");
    default_keys_path = parent_path / "keys.config";
    user_keys_paths.push_back(standard_data_path / "keys_user.config");
    database_file_path = standard_data_path / "test.db";
    local_database_file_path = standard_data_path / "local.db";
    global_database_file_path = standard_data_path / "shared.db";
    tutorial_path = standard_data_path / "tutorial.pdf";
    last_opened_file_address_path =
        standard_data_path / "last_document_path.txt";

    fs::path linux_standard_config_path =
        linux_home_path / ".config" / "sioyek";
    // user_keys_paths.push_back(mac_standard_config_path.slash("keys_user.config"));
    // user_config_paths.push_back(mac_standard_config_path.slash("prefs_user.config"));
    user_keys_paths.push_back(linux_standard_config_path / "keys_user.config");
    user_config_paths.push_back(
        linux_standard_config_path / "prefs_user.config"
    );

    if (!fs::exists(tutorial_path)) {
        copy_file(parent_path / "tutorial.pdf", tutorial_path);
    }
#endif
#else // windows
#ifdef NDEBUG
    // install_app(exe_path.c_str());
#endif
    fs::path standard_data_path(
        QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)
            .at(0)
            .toStdString()
    );
    QStringList all_config_paths =
        QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);

    fs::create_directories(standard_data_path);

    default_config_path = parent_path / "prefs.config";
    default_keys_path = parent_path / "keys.config";
    tutorial_path = parent_path / "tutorial.pdf";

#ifdef NON_PORTABLE
    user_config_paths.push_back(standard_data_path / "prefs_user.config");
    user_keys_paths.push_back(standard_data_path / "keys_user.config");
    for (int i = all_config_paths.size() - 1; i > 0; i--) {
        user_config_paths.push_back(
            fs::path(all_config_paths.at(i).toStdString()) / "prefs_user.config"
        );
        user_keys_paths.push_back(
            fs::path(all_config_paths.at(i).toStdString()) / "keys_user.config"
        );
    }
    database_file_path = standard_data_path / "test.db";
    local_database_file_path = standard_data_path / "local.db";
    global_database_file_path = standard_data_path / "shared.db";
    last_opened_file_address_path =
        standard_data_path / "last_document_path.txt";
#else
    user_config_paths.push_back(parent_path / "prefs_user.config");
    user_keys_paths.push_back(parent_path / "keys_user.config");
    database_file_path = parent_path / "test.db";
    local_database_file_path = parent_path / "local.db";
    global_database_file_path = parent_path / "shared.db";
    last_opened_file_address_path = parent_path / "last_document_path.txt";
#endif

#endif
    auto_config_path = standard_data_path / "auto.config";
    // user_config_paths.insert(user_config_paths.begin(),
    // auto_config_path);
}

void check_path_exists(const fs::path& path) {
    if (!fs::exists(path)) {
        std::cout << "Error: " << path << ": " << path << " doesn't exist!"
                  << std::endl;
    }
}

void Config::verify_config_paths() {
    std::cout << "default_config_path: " << default_config_path << "\n";
    check_path_exists(default_config_path);
    std::cout << "default_keys_path: " << default_keys_path << "\n";
    check_path_exists(default_keys_path);
    for (size_t i = 0; i < user_config_paths.size(); i++) {
        std::cout << "user_config_path: [ " << i << " ] "
                  << user_config_paths[i] << "\n";
    }
    for (size_t i = 0; i < user_keys_paths.size(); i++) {
        std::cout << "user_keys_path: [ " << i << " ] " << user_keys_paths[i]
                  << "\n";
    }
}

void Config::verify_paths() {
    std::cout << "database_file_path: " << database_file_path << "\n";
    std::cout << "local_database_file_path: " << local_database_file_path
              << "\n";
    std::cout << "global_database_file_path: " << global_database_file_path
              << "\n";
    std::cout << "tutorial_path: " << tutorial_path << "\n";
    std::cout << "last_opened_file_address_path: "
              << last_opened_file_address_path << "\n";
    std::cout << "shader_path: " << shader_path << "\n";
    check_path_exists(shader_path);
}
