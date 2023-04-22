#pragma once

#include "utils.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <optional>
#include <filesystem>

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

namespace fs = std::filesystem;

struct HighlightConfig {
    float foreground[3];
    float background[3];
};

class Config {
  public:
    static Config& instance() {
        static Config s;
        return s;
    }

    HighlightConfig document = {
        .foreground = {0.0f, 0.0f, 0.0f},
        .background = {1.0f, 1.0f, 1.0f},
    };

    HighlightConfig background = {
        .foreground = {1.0f,  1.0f,  1.0f },
        .background = {0.97f, 0.97f, 0.97f},
    };

    struct {
        struct {
            int font_size = -1;
            float background[3] = {0.0f, 0.0f, 0.0f};
            float foreground[3] = {1.0f, 1.0f, 1.0f};
        } status_bar;

        HighlightConfig selected = {
            .foreground = {0.0f, 0.0f, 0.0f},
            .background = {1.0f, 1.0f, 1.0f},
        };

        float UNSELECTED_SEARCH_HIGHLIGHT_COLOR[3] = {0.0f, 0.5f, 0.5f};

        float highlight_colors[26 * 3] = {
            0.5182963463943647,  0.052279561076773784, 0.42942409252886155,
            0.673198309637537,   0.14250443697242887,  0.1842972533900342,
            0.1259143196334698,  0.3472546716690144,   0.5268310086975159,
            0.44867634475259244, 0.36152631940627494,  0.18979733584113254,
            0.25561951195738114, 0.5203940586174391,   0.2239864294251798,
            0.46620566366115457, 0.34950449396122557,  0.18428984237761986,
            0.7766958121833649,  0.18529941752256116,  0.03800477029407397,
            0.14245148690647982, 0.27376105738246703,  0.5837874557110532,
            0.15069695522822338, 0.6757965126090706,   0.173506532162706,
            0.20214309005349734, 0.388109281902417,    0.40974762804408554,
            0.5282008406153603,  0.3604221142506678,   0.11137704513397183,
            0.11065494801726693, 0.43355028291683534,  0.4557947690658978,
            0.4623270941397442,  0.2575781303014751,   0.28009477555878065,
            0.13682260642246874, 0.843494092757017,    0.019683300820514175,
            0.3779898334061099,  0.10067511592122631,  0.5213350506726637,
            0.20252688176577896, 0.46636886381356,     0.33110425442066094,
            0.26429496078170356, 0.4214065241882322,   0.31429851503006423,
            0.2778665356071555,  0.31938061671537193,  0.40275284767747266,
            0.2859415758796114,  0.3778585576392479,   0.33619986648114064,
            0.06881479216543497, 0.49813975498043,     0.43304545285413504,
            0.5411102077276201,  0.050950286432382155, 0.4079395058399978,
            0.13956877643913856, 0.4133573589812949,   0.44707386457956655,
            0.5672781038824454,  0.026174925202518497, 0.4065469709150361,
            0.33594461744136966, 0.30463905854351836,  0.359416324015112,
            0.16837764593670387, 0.43225375356473283,  0.3993686004985632,
            0.21290269578043475, 0.5704883842115632,   0.21660892000800203,
        };
    } theme;

    std::string log_file_name = "sioyek_log.txt";
    std::ofstream LOG_FILE;

    std::wstring SEARCH_URLS[26];
    std::wstring EXECUTE_COMMANDS[26];
    std::wstring TEXT_HIGHLIGHT_URL = L"http://localhost:5000/";
    std::wstring MIDDLE_CLICK_SEARCH_ENGINE = L"s";
    std::wstring SHIFT_MIDDLE_CLICK_SEARCH_ENGINE = L"l";
    std::wstring PAPERS_FOLDER_PATH = L"";
    std::wstring STATUS_BAR_FORMAT =
        L"Page %{current_page} / "
        L"%{num_pages}%{chapter_name}%{search_results}%{search_progress}%{link_"
        L"status}%{waiting_for_symbol}%{indexing}%{preview_index}%{synctex}%{"
        L"drag}%"
        L"{presentation}%{visual_scroll}%{locked_scroll}%{highlight}%{closest_"
        L"bookmark}%{close_portal}%{rect_select}%{custom_message}";

    struct {
        std::wstring name = L"sioyek";
        std::string version = "2.0.0";
        int font_size = -1;
        bool debug = false;
        float gamma = 1.0f;
        bool load_tutorial_when_no_other_file = false;
        bool launch_new_instance = false;
        bool launch_new_window = false;
        bool draw_unrendered_pages = true;
        bool use_multiple_monitors = false;
        bool warn_about_user_key_override = true;
    } application;

    float ZOOM_INC_FACTOR = 1.2f;
    float VERTICAL_MOVE_AMOUNT = 1.0f;
    float HORIZONTAL_MOVE_AMOUNT = 1.0f;
    float MOVE_SCREEN_PERCENTAGE = 0.8f;
    const unsigned int CACHE_INVALID_MILIES = 1000;
    const int PERSIST_MILIES = 1000 * 60;
    const int PAGE_PADDINGS = 0;
    const int MAX_PENDING_REQUESTS = 31;
    bool FLAT_TABLE_OF_CONTENTS = false;
    bool SORT_BOOKMARKS_BY_LOCATION = false;
    std::wstring LIBGEN_ADDRESS = L"";
    std::wstring GOOGLE_SCHOLAR_ADDRESS = L"";
    std::wstring INVERSE_SEARCH_COMMAND = L"";
    std::wstring SHARED_DATABASE_PATH = L"";
    std::wstring UI_FONT_FACE_NAME = L"";
    std::wstring DEFAULT_OPEN_FILE_PATH = L"";
    bool HOVER_OVERVIEW = false;
    bool RERENDER_OVERVIEW = false;
    bool LINEAR_TEXTURE_FILTERING = false;
    bool RULER_MODE = false;
    bool SMALL_TOC = false;
    bool WHEEL_ZOOM_ON_CURSOR = false;
    bool TEXT_SUMMARY_HIGHLIGHT_SHOULD_REFINE = true;
    bool TEXT_SUMMARY_HIGHLIGHT_SHOULD_FILL = true;
    bool USE_HEURISTIC_IF_TEXT_SUMMARY_NOT_AVAILABLE = false;
    int TEXT_SUMMARY_CONTEXT_SIZE = 49;
    float VISUAL_MARK_NEXT_PAGE_FRACTION = 0.25f;
    float VISUAL_MARK_NEXT_PAGE_THRESHOLD = 0.1f;
    float RULER_PADDING = 0.0f;
    float RULER_X_PADDING = 0.0f;
    std::wstring ITEM_LIST_PREFIX = L">";
    std::wstring STARTUP_COMMANDS = L"";
    float SMALL_PIXMAP_SCALE = 0.75f;
    float DISPLAY_RESOLUTION_SCALE = -1;
    float FIT_TO_PAGE_WIDTH_RATIO = 1;
    int MAIN_WINDOW_SIZE[2] = {-1, -1};
    int HELPER_WINDOW_SIZE[2] = {-1, -1};
    int MAIN_WINDOW_MOVE[2] = {-1, -1};
    int HELPER_WINDOW_MOVE[2] = {-1, -1};
    float TOUCHPAD_SENSITIVITY = 1.0f;
    int SINGLE_MAIN_WINDOW_SIZE[2] = {-1, -1};
    int SINGLE_MAIN_WINDOW_MOVE[2] = {-1, -1};
    bool ENABLE_EXPERIMENTAL_FEATURES = false;
    bool CREATE_TABLE_OF_CONTENTS_IF_NOT_EXISTS = true;
    int MAX_CREATED_TABLE_OF_CONTENTS_SIZE = 5000;
    bool FORCE_CUSTOM_LINE_ALGORITHM = false;
    float OVERVIEW_SIZE[2] = {0.8f, 0.4f};
    float OVERVIEW_OFFSET[2] = {0.0f, 0.0f};
    bool IGNORE_WHITESPACE_IN_PRESENTATION_MODE = false;
    bool EXACT_HIGHLIGHT_SELECT = false;
    bool SHOW_DOC_PATH = false;
    float FASTREAD_OPACITY = 0.5f;
    bool SINGLE_CLICK_SELECTS_WORDS = false;
    bool MULTILINE_MENUS = false;
    bool START_WITH_HELPER_WINDOW = false;
    std::map<std::wstring, std::wstring> ADDITIONAL_COMMANDS;
    std::map<std::wstring, std::wstring> ADDITIONAL_MACROS;
    bool PRERENDER_NEXT_PAGE = false;
    bool EMACS_MODE = false;
    bool HIGHLIGHT_MIDDLE_CLICK = false;
    float HYPERDRIVE_SPEED_FACTOR = 10.0f;
    float SMOOTH_SCROLL_SPEED = 3.0f;
    float SMOOTH_SCROLL_DRAG = 3000.0f;
    int PRERENDERED_PAGE_COUNT = 0;

    struct {

    } search;

    float PAGE_SEPARATOR_WIDTH = 0.0f;
    float PAGE_SEPARATOR_COLOR[3] = {0.9f, 0.9f, 0.9f};
    bool IGNORE_STATUSBAR_IN_PRESENTATION_MODE = false;
    bool SUPER_FAST_SEARCH = false;
    bool SHOW_CLOSEST_BOOKMARK_IN_STATUSBAR = false;
    bool SHOW_CLOSE_PORTAL_IN_STATUSBAR = false;
    bool CASE_SENSITIVE_SEARCH = true;
    bool SHOW_DOCUMENT_NAME_IN_STATUSBAR = false;
    bool NUMERIC_TAGS = false;
    bool SHOULD_HIGHLIGHT_LINKS = false;
    bool SHOULD_HIGHLIGHT_UNSELECTED_SEARCH = false;
    int KEYBOARD_SELECT_FONT_SIZE = 20;
    bool INVERTED_HORIZONTAL_SCROLLING = false;
    bool TOC_JUMP_ALIGN_TOP = false;
    float CUSTOM_COLOR_CONTRAST = 0.5f;
    float HIGHLIGHT_DELETE_THRESHOLD = 0.01f;
    float SCROLL_VIEW_SENSITIVITY = 1.0f;
    float KEYBOARD_SELECT_BACKGROUND_COLOR[4] = {0.9f, 0.75f, 0.0f, 1.0f};
    float KEYBOARD_SELECT_TEXT_COLOR[4] = {0.0f, 0.0f, 0.5f, 1.0f};
    bool AUTOCENTER_VISUAL_SCROLL = false;
    bool ALPHABETIC_LINK_TAGS = false;
    bool VIMTEX_WSL_FIX = false;

    fs::path default_config_path;
    fs::path default_keys_path;
    std::vector<fs::path> user_config_paths;
    std::vector<fs::path> user_keys_paths;
    fs::path database_file_path;
    fs::path local_database_file_path;
    fs::path global_database_file_path;
    fs::path tutorial_path;
    fs::path last_opened_file_address_path;
    fs::path shader_path;
    fs::path auto_config_path;

    std::wstring SHIFT_CLICK_COMMAND = L"overview_under_cursor";
    std::wstring CONTROL_CLICK_COMMAND = L"smart_jump_under_cursor";
    std::wstring SHIFT_RIGHT_CLICK_COMMAND = L"";
    std::wstring CONTROL_RIGHT_CLICK_COMMAND = L"";
    std::wstring ALT_CLICK_COMMAND = L"";
    std::wstring ALT_RIGHT_CLICK_COMMAND = L"";

    void configure_paths();

    void verify_config_paths();

    void verify_paths();

  private:
    Config() {}

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    ~Config() {}
};

struct ConfigEntry {
    std::wstring name;
    void* value = nullptr;
    void (*serialize)(void*, std::wstringstream&) = nullptr;
    void* (*deserialize)(std::wstringstream&, void* res) = nullptr;
    bool (*validator)(const std::wstring& value);

    void* get_value();
};

class ConfigManager {
    std::vector<ConfigEntry> configs;

    ConfigEntry* get_mut_config_with_name(std::wstring config_name);
    float DEFAULT_TEXT_HIGHLIGHT_COLOR[3];
    float DEFAULT_VERTICAL_LINE_COLOR[4];
    float DEFAULT_SEARCH_HIGHLIGHT_COLOR[3];
    float DEFAULT_LINK_HIGHLIGHT_COLOR[3];
    float DEFAULT_SYNCTEX_HIGHLIGHT_COLOR[3];

    std::vector<fs::path> user_config_paths;

  public:
    ConfigManager(
        const fs::path& default_path,
        const fs::path& auto_path,
        const std::vector<fs::path>& user_paths
    );
    // void serialize(std::wofstream& file);
    void deserialize(
        const fs::path& default_file_path,
        const fs::path& auto_path,
        const std::vector<fs::path>& user_file_paths
    );
    void deserialize_file(
        const fs::path& file_path, bool warn_if_not_exists = false
    );

    template <typename T>
    const T* get_config(std::wstring name) {

        void* raw_pointer = get_mut_config_with_name(name)->get_value();

        // todo: Provide a default value for all configs, so that all the
        // nullchecks here and in the places where `get_config` is called can be
        // removed.
        if (raw_pointer == nullptr)
            return nullptr;
        return (T*)raw_pointer;
    }

    std::optional<fs::path> get_or_create_user_config_file();
    std::vector<fs::path> get_all_user_config_files();
    std::vector<ConfigEntry> get_configs();
    void deserialize_config(std::string config_name, std::wstring config_value);
};
