#include "command_manager.h"
#include "commands/all_commands.h"

extern std::map<std::wstring, std::wstring> ADDITIONAL_MACROS;
extern std::map<std::wstring, std::wstring> ADDITIONAL_COMMANDS;

template <typename T>
auto make_command_creator() {
    return []() { return std::make_unique<T>(); };
}

CommandManager::CommandManager(ConfigManager* config_manager) {
    new_commands["goto_beginning"] =
        make_command_creator<GotoBeginningCommand>();
    new_commands["goto_end"] = make_command_creator<GotoEndCommand>();
    new_commands["goto_definition"] =
        make_command_creator<GotoDefinitionCommand>();
    new_commands["overview_definition"] =
        make_command_creator<OverviewDefinitionCommand>();
    new_commands["portal_to_definition"] =
        make_command_creator<PortalToDefinitionCommand>();
    new_commands["next_item"] = make_command_creator<NextItemCommand>();
    new_commands["previous_item"] = make_command_creator<PrevItemCommand>();
    new_commands["set_mark"] = make_command_creator<SetMarkCommand>();
    new_commands["goto_mark"] = make_command_creator<GotoMarkCommand>();
    new_commands["goto_page_with_page_number"] =
        make_command_creator<GotoPageWithPageNumberCommand>();
    new_commands["search"] = make_command_creator<SearchCommand>();
    new_commands["regex_search"] = make_command_creator<RegexSearchCommand>();
    new_commands["chapter_search"] =
        make_command_creator<ChapterSearchCommand>();
    new_commands["move_down"] = make_command_creator<MoveDownCommand>();
    new_commands["move_up"] = make_command_creator<MoveUpCommand>();
    new_commands["move_left"] = make_command_creator<MoveLeftCommand>();
    new_commands["move_right"] = make_command_creator<MoveRightCommand>();
    new_commands["zoom_in"] = make_command_creator<ZoomInCommand>();
    new_commands["zoom_out"] = make_command_creator<ZoomOutCommand>();
    new_commands["fit_to_page_width"] =
        make_command_creator<FitToPageWidthCommand>();
    new_commands["fit_to_page_height"] =
        make_command_creator<FitToPageHeightCommand>();
    new_commands["fit_to_page_height_smart"] =
        make_command_creator<FitToPageHeightSmartCommand>();
    new_commands["fit_to_page_width_smart"] =
        make_command_creator<FitToPageWidthSmartCommand>();
    new_commands["next_page"] = make_command_creator<NextPageCommand>();
    new_commands["previous_page"] = make_command_creator<PreviousPageCommand>();
    new_commands["open_document"] = make_command_creator<OpenDocumentCommand>();
    new_commands["add_bookmark"] = make_command_creator<AddBookmarkCommand>();
    new_commands["add_highlight"] = make_command_creator<AddHighlightCommand>();
    new_commands["goto_toc"] =
        make_command_creator<GotoTableOfContentsCommand>();
    new_commands["goto_highlight"] =
        make_command_creator<GotoHighlightCommand>();
    new_commands["goto_bookmark"] = make_command_creator<GotoBookmarkCommand>();
    new_commands["goto_bookmark_g"] =
        make_command_creator<GotoBookmarkGlobalCommand>();
    new_commands["goto_highlight_g"] =
        make_command_creator<GotoHighlightGlobalCommand>();
    new_commands["link"] = make_command_creator<PortalCommand>();
    new_commands["portal"] = make_command_creator<PortalCommand>();
    new_commands["next_state"] = make_command_creator<NextStateCommand>();
    new_commands["prev_state"] = make_command_creator<PrevStateCommand>();
    new_commands["delete_link"] = make_command_creator<DeletePortalCommand>();
    new_commands["delete_portal"] = make_command_creator<DeletePortalCommand>();
    new_commands["delete_bookmark"] =
        make_command_creator<DeleteBookmarkCommand>();
    new_commands["delete_highlight"] =
        make_command_creator<DeleteHighlightCommand>();
    new_commands["goto_link"] = make_command_creator<GotoPortalCommand>();
    new_commands["goto_portal"] = make_command_creator<GotoPortalCommand>();
    new_commands["edit_link"] = make_command_creator<EditPortalCommand>();
    new_commands["edit_portal"] = make_command_creator<EditPortalCommand>();
    new_commands["open_prev_doc"] = make_command_creator<OpenPrevDocCommand>();
    new_commands["open_document_embedded"] =
        make_command_creator<OpenDocumentEmbeddedCommand>();
    new_commands["open_document_embedded_from_current_path"] =
        make_command_creator<OpenDocumentEmbeddedFromCurrentPathCommand>();
    new_commands["copy"] = make_command_creator<CopyCommand>();
    new_commands["toggle_fullscreen"] =
        make_command_creator<ToggleFullscreenCommand>();
    new_commands["toggle_one_window"] =
        make_command_creator<ToggleOneWindowCommand>();
    new_commands["toggle_highlight"] =
        make_command_creator<ToggleHighlightCommand>();
    new_commands["toggle_synctex"] =
        make_command_creator<ToggleSynctexCommand>();
    new_commands["turn_on_synctex"] =
        make_command_creator<TurnOnSynctexCommand>();
    new_commands["toggle_show_last_command"] =
        make_command_creator<ToggleShowLastCommand>();
    new_commands["command"] = make_command_creator<CommandCommand>();
    new_commands["external_search"] =
        make_command_creator<ExternalSearchCommand>();
    new_commands["open_selected_url"] =
        make_command_creator<OpenSelectedUrlCommand>();
    new_commands["screen_down"] = make_command_creator<ScreenDownCommand>();
    new_commands["screen_up"] = make_command_creator<ScreenUpCommand>();
    new_commands["next_chapter"] = make_command_creator<NextChapterCommand>();
    new_commands["prev_chapter"] = make_command_creator<PrevChapterCommand>();
    new_commands["toggle_dark_mode"] =
        make_command_creator<ToggleDarkModeCommand>();
    new_commands["toggle_presentation_mode"] =
        make_command_creator<TogglePresentationModeCommand>();
    new_commands["turn_on_presentation_mode"] =
        make_command_creator<TurnOnPresentationModeCommand>();
    new_commands["toggle_mouse_drag_mode"] =
        make_command_creator<ToggleMouseDragModeCommand>();
    new_commands["close_window"] = make_command_creator<CloseWindowCommand>();
    new_commands["quit"] = make_command_creator<QuitCommand>();
    new_commands["escape"] = make_command_creator<EscapeCommand>();
    new_commands["q"] = make_command_creator<QuitCommand>();
    new_commands["open_link"] = make_command_creator<OpenLinkCommand>();
    new_commands["overview_link"] = make_command_creator<OverviewLinkCommand>();
    new_commands["portal_to_link"] =
        make_command_creator<PortalToLinkCommand>();
    new_commands["copy_link"] = make_command_creator<CopyLinkCommand>();
    new_commands["keyboard_select"] =
        make_command_creator<KeyboardSelectCommand>();
    new_commands["keyboard_smart_jump"] =
        make_command_creator<KeyboardSmartjumpCommand>();
    new_commands["keyboard_overview"] =
        make_command_creator<KeyboardOverviewCommand>();
    new_commands["keys"] = make_command_creator<KeysCommand>();
    new_commands["keys_user"] = make_command_creator<KeysUserCommand>();
    new_commands["prefs"] = make_command_creator<PrefsCommand>();
    new_commands["prefs_user"] = make_command_creator<PrefsUserCommand>();
    new_commands["move_visual_mark_down"] =
        make_command_creator<MoveVisualMarkDownCommand>();
    new_commands["move_visual_mark_up"] =
        make_command_creator<MoveVisualMarkUpCommand>();
    new_commands["toggle_custom_color"] =
        make_command_creator<ToggleCustomColorMode>();
    new_commands["set_select_highlight_type"] =
        make_command_creator<SetSelectHighlightTypeCommand>();
    new_commands["toggle_window_configuration"] =
        make_command_creator<ToggleWindowConfigurationCommand>();
    new_commands["prefs_user_all"] =
        make_command_creator<PrefsUserAllCommand>();
    new_commands["keys_user_all"] = make_command_creator<KeysUserAllCommand>();
    new_commands["fit_to_page_width_ratio"] =
        make_command_creator<FitToPageWidthRatioCommand>();
    new_commands["smart_jump_under_cursor"] =
        make_command_creator<SmartJumpUnderCursorCommand>();
    new_commands["overview_under_cursor"] =
        make_command_creator<OverviewUnderCursorCommand>();
    new_commands["close_overview"] =
        make_command_creator<CloseOverviewCommand>();
    new_commands["visual_mark_under_cursor"] =
        make_command_creator<VisualMarkUnderCursorCommand>();
    new_commands["close_visual_mark"] =
        make_command_creator<CloseVisualMarkCommand>();
    new_commands["zoom_in_cursor"] =
        make_command_creator<ZoomInCursorCommand>();
    new_commands["zoom_out_cursor"] =
        make_command_creator<ZoomOutCursorCommand>();
    new_commands["goto_left"] = make_command_creator<GotoLeftCommand>();
    new_commands["goto_left_smart"] =
        make_command_creator<GotoLeftSmartCommand>();
    new_commands["goto_right"] = make_command_creator<GotoRightCommand>();
    new_commands["goto_right_smart"] =
        make_command_creator<GotoRightSmartCommand>();
    new_commands["rotate_clockwise"] =
        make_command_creator<RotateClockwiseCommand>();
    new_commands["rotate_counterclockwise"] =
        make_command_creator<RotateCounterClockwiseCommand>();
    new_commands["goto_next_highlight"] =
        make_command_creator<GotoNextHighlightCommand>();
    new_commands["goto_prev_highlight"] =
        make_command_creator<GotoPrevHighlightCommand>();
    new_commands["goto_next_highlight_of_type"] =
        make_command_creator<GotoNextHighlightOfTypeCommand>();
    new_commands["goto_prev_highlight_of_type"] =
        make_command_creator<GotoPrevHighlightOfTypeCommand>();
    new_commands["add_highlight_with_current_type"] =
        make_command_creator<AddHighlightWithCurrentTypeCommand>();
    new_commands["enter_password"] =
        make_command_creator<EnterPasswordCommand>();
    new_commands["toggle_fastread"] =
        make_command_creator<ToggleFastreadCommand>();
    new_commands["goto_top_of_page"] =
        make_command_creator<GotoTopOfPageCommand>();
    new_commands["goto_bottom_of_page"] =
        make_command_creator<GotoBottomOfPageCommand>();
    new_commands["new_window"] = make_command_creator<NewWindowCommand>();
    new_commands["reload"] = make_command_creator<ReloadCommand>();
    new_commands["reload_config"] = make_command_creator<ReloadConfigCommand>();
    new_commands["synctex_under_cursor"] =
        make_command_creator<SynctexUnderCursorCommand>();
    new_commands["set_status_string"] =
        make_command_creator<SetStatusStringCommand>();
    new_commands["clear_status_string"] =
        make_command_creator<ClearStatusStringCommand>();
    new_commands["toggle_titlebar"] =
        make_command_creator<ToggleTittlebarCommand>();
    new_commands["next_preview"] = make_command_creator<NextPreviewCommand>();
    new_commands["previous_preview"] =
        make_command_creator<PrevPreviewCommand>();
    new_commands["goto_overview"] = make_command_creator<GotoOverviewCommand>();
    new_commands["portal_to_overview"] =
        make_command_creator<PortalToOverviewCommand>();
    new_commands["goto_selected_text"] =
        make_command_creator<GotoSelectedTextCommand>();
    new_commands["focus_text"] = make_command_creator<FocusTextCommand>();
    new_commands["goto_window"] = make_command_creator<GotoWindowCommand>();
    new_commands["toggle_smooth_scroll_mode"] =
        make_command_creator<ToggleSmoothScrollModeCommand>();
    new_commands["goto_begining"] =
        make_command_creator<GotoBeginningCommand>();
    new_commands["toggle_scrollbar"] =
        make_command_creator<ToggleScrollbarCommand>();
    new_commands["overview_to_portal"] =
        make_command_creator<OverviewToPortalCommand>();
    new_commands["select_rect"] = make_command_creator<SelectRectCommand>();
    new_commands["toggle_typing_mode"] =
        make_command_creator<ToggleTypingModeCommand>();
    new_commands["overview_next_item"] =
        make_command_creator<OverviewNextItemCommand>();
    new_commands["overview_prev_item"] =
        make_command_creator<OverviewPrevItemCommand>();
    new_commands["delete_highlight_under_cursor"] =
        make_command_creator<DeleteHighlightUnderCursorCommand>();
    new_commands["noop"] = make_command_creator<NoopCommand>();
    new_commands["import"] = make_command_creator<ImportCommand>();
    new_commands["export"] = make_command_creator<ExportCommand>();
    new_commands["enter_visual_mark_mode"] =
        make_command_creator<EnterVisualMarkModeCommand>();
    new_commands["set_page_offset"] =
        make_command_creator<SetPageOffsetCommand>();
    new_commands["toggle_visual_scroll"] =
        make_command_creator<ToggleVisualScrollCommand>();
    new_commands["toggle_horizontal_scroll_lock"] =
        make_command_creator<ToggleHorizontalLockCommand>();
    new_commands["execute"] = make_command_creator<ExecuteCommand>();
    new_commands["embed_annotations"] =
        make_command_creator<EmbedAnnotationsCommand>();
    new_commands["copy_window_size_config"] =
        make_command_creator<CopyWindowSizeConfigCommand>();
    new_commands["toggle_select_highlight"] =
        make_command_creator<ToggleSelectHighlightCommand>();
    new_commands["open_last_document"] =
        make_command_creator<OpenLastDocumentCommand>();
    new_commands["toggle_statusbar"] =
        make_command_creator<ToggleStatusbarCommand>();

    for (auto [command_name_, command_value] : ADDITIONAL_COMMANDS) {
        std::string command_name = utf8_encode(command_name_);
        std::wstring local_command_value = command_value;
        new_commands[command_name] = [command_name, local_command_value]() {
            return std::make_unique<CustomCommand>(
                command_name, local_command_value
            );
        };
    }

    for (auto [command_name_, macro_value] : ADDITIONAL_MACROS) {
        std::string command_name = utf8_encode(command_name_);
        std::wstring local_macro_value = macro_value;
        new_commands[command_name] = [command_name, local_macro_value, this]() {
            return std::make_unique<MacroCommand>(
                this, command_name, local_macro_value
            );
        };
    }

    std::vector<Config> configs = config_manager->get_configs();

    for (auto conf : configs) {
        std::string confname = utf8_encode(conf.name);
        std::string config_set_command_name = "setconfig_" + confname;
        new_commands[config_set_command_name] = [confname]() {
            return std::make_unique<ConfigCommand>(confname);
        };
    }
}

std::unique_ptr<Command> CommandManager::get_command_with_name(std::string name
) const {
    if (new_commands.find(name) != new_commands.end()) {
        return new_commands.at(name)();
    }
    return nullptr;
}

QStringList CommandManager::get_all_command_names() const {
    QStringList res;
    for (const auto& com : new_commands) {
        res.push_back(QString::fromStdString(com.first));
    }
    return res;
}

std::unique_ptr<Command> CommandManager::create_macro_command(
    std::string name, std::wstring macro_string
) {
    return std::make_unique<MacroCommand>(this, name, macro_string);
}
