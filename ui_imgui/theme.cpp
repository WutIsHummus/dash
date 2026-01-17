#include "theme.h"

namespace ui {

void ApplyTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Spacing
    style.WindowPadding = ImVec2(Spacing::WindowPadding, Spacing::WindowPadding);
    style.FramePadding = ImVec2(Spacing::FramePadding, Spacing::FramePadding);
    style.ItemSpacing = ImVec2(Spacing::ItemSpacing, Spacing::ItemSpacing);
    style.ItemInnerSpacing = ImVec2(Spacing::ItemInnerSpacing, Spacing::ItemInnerSpacing);
    style.CellPadding = ImVec2(6.0f, 4.0f);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 12.0f;
    style.GrabMinSize = 10.0f;
    
    // Rounding
    style.WindowRounding = Rounding::Window;
    style.ChildRounding = Rounding::Card;
    style.FrameRounding = Rounding::Frame;
    style.PopupRounding = Rounding::Popup;
    style.ScrollbarRounding = Rounding::ScrollBar;
    style.GrabRounding = Rounding::Frame;
    style.TabRounding = Rounding::Tab;
    
    // Borders - subtle
    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.TabBorderSize = 0.0f;
    
    // Button text alignment
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.5f);
    
    // Anti-aliasing
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
    
    // Colors
    ImVec4* colors = style.Colors;
    
    colors[ImGuiCol_Text]                   = Colors::Foreground();
    colors[ImGuiCol_TextDisabled]           = Colors::MutedForeground();
    colors[ImGuiCol_WindowBg]               = Colors::Background();
    colors[ImGuiCol_ChildBg]                = Colors::Card();
    colors[ImGuiCol_PopupBg]                = Colors::Card();
    colors[ImGuiCol_Border]                 = Colors::Border();
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_FrameBg]                = Colors::Muted();
    colors[ImGuiCol_FrameBgHovered]         = Colors::Secondary();
    colors[ImGuiCol_FrameBgActive]          = Colors::Secondary();
    colors[ImGuiCol_TitleBg]                = Colors::Card();
    colors[ImGuiCol_TitleBgActive]          = Colors::Card();
    colors[ImGuiCol_TitleBgCollapsed]       = Colors::Card();
    colors[ImGuiCol_MenuBarBg]              = Colors::Card();
    colors[ImGuiCol_ScrollbarBg]            = Colors::Muted();
    colors[ImGuiCol_ScrollbarGrab]          = Colors::Secondary();
    colors[ImGuiCol_ScrollbarGrabHovered]   = Colors::MutedForeground();
    colors[ImGuiCol_ScrollbarGrabActive]    = Colors::Primary();
    colors[ImGuiCol_CheckMark]              = Colors::Primary();
    colors[ImGuiCol_SliderGrab]             = Colors::Primary();
    colors[ImGuiCol_SliderGrabActive]       = Colors::PrimaryDark();
    colors[ImGuiCol_Button]                 = Colors::Muted();
    colors[ImGuiCol_ButtonHovered]          = Colors::Secondary();
    colors[ImGuiCol_ButtonActive]           = Colors::Primary();
    colors[ImGuiCol_Header]                 = Colors::Muted();
    colors[ImGuiCol_HeaderHovered]          = Colors::Secondary();
    colors[ImGuiCol_HeaderActive]           = Colors::Primary();
    colors[ImGuiCol_Separator]              = Colors::Border();
    colors[ImGuiCol_SeparatorHovered]       = Colors::MutedForeground();
    colors[ImGuiCol_SeparatorActive]        = Colors::Primary();
    colors[ImGuiCol_ResizeGrip]             = Colors::Muted();
    colors[ImGuiCol_ResizeGripHovered]      = Colors::Secondary();
    colors[ImGuiCol_ResizeGripActive]       = Colors::Primary();
    colors[ImGuiCol_Tab]                    = Colors::Card();
    colors[ImGuiCol_TabHovered]             = Colors::Primary();
    colors[ImGuiCol_TabActive]              = Colors::Primary();
    colors[ImGuiCol_TabUnfocused]           = Colors::Card();
    colors[ImGuiCol_TabUnfocusedActive]     = Colors::Secondary();
    colors[ImGuiCol_PlotLines]              = Colors::Primary();
    colors[ImGuiCol_PlotLinesHovered]       = Colors::Accent();
    colors[ImGuiCol_PlotHistogram]          = Colors::Primary();
    colors[ImGuiCol_PlotHistogramHovered]   = Colors::Accent();
    colors[ImGuiCol_TableHeaderBg]          = Colors::Muted();
    colors[ImGuiCol_TableBorderStrong]      = Colors::Border();
    colors[ImGuiCol_TableBorderLight]       = ColorWithAlpha(Colors::Border(), 0.5f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_TableRowBgAlt]          = ColorWithAlpha(Colors::Muted(), 0.3f);
    colors[ImGuiCol_TextSelectedBg]         = ColorWithAlpha(Colors::Primary(), 0.35f);
    colors[ImGuiCol_DragDropTarget]         = Colors::Accent();
    colors[ImGuiCol_NavHighlight]           = Colors::Primary();
    colors[ImGuiCol_NavWindowingHighlight]  = Colors::Primary();
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.0f, 0.0f, 0.0f, 0.50f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.0f, 0.0f, 0.0f, 0.50f);
}

bool LoadFonts(const char* fontPath, const char* monoFontPath, const char* iconFontPath) {
    ImGuiIO& io = ImGui::GetIO();
    
    // TODO: Load custom fonts from fontPath, monoFontPath if provided
    // For now, use ImGui default with adjusted size
    
    // Main UI font
    if (fontPath != nullptr) {
        // TODO: Load font from path
        // io.Fonts->AddFontFromFileTTF(fontPath, FontSize::Normal);
    }
    
    // Fallback to default ImGui font
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 2;
    config.PixelSnapH = true;
    
    io.Fonts->AddFontDefault(&config);
    
    // TODO: Load monospace font if provided
    if (monoFontPath != nullptr) {
        // io.Fonts->AddFontFromFileTTF(monoFontPath, FontSize::Normal);
    }
    
    // TODO: Merge icon font if provided (Font Awesome, etc.)
    if (iconFontPath != nullptr) {
        // ImFontConfig iconConfig;
        // iconConfig.MergeMode = true;
        // iconConfig.PixelSnapH = true;
        // static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        // io.Fonts->AddFontFromFileTTF(iconFontPath, FontSize::Normal, &iconConfig, icon_ranges);
    }
    
    return true;
}

} // namespace ui
