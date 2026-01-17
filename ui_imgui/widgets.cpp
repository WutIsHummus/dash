#include "widgets.h"
#include <cmath>
#include <algorithm>
#include <cstdio>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace ui {
namespace widgets {

bool BeginCard(const char* id, const ImVec2& size, bool border) {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar;
    
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, Rounding::Card);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(Spacing::CardPadding, Spacing::CardPadding));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, Colors::Card());
    ImGui::PushStyleColor(ImGuiCol_Border, border ? Colors::Border() : ImVec4(0, 0, 0, 0));
    
    bool result = ImGui::BeginChild(id, size, ImGuiChildFlags_Borders, flags);
    
    return result;
}

void EndCard() {
    ImGui::EndChild();
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(2);
}

void Badge(const char* label, const ImVec4& color, const ImVec4& textColor) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    
    ImVec2 textSize = ImGui::CalcTextSize(label);
    float paddingX = 8.0f;
    float paddingY = 2.0f;
    
    ImVec2 rectMin = pos;
    ImVec2 rectMax = ImVec2(pos.x + textSize.x + paddingX * 2, pos.y + textSize.y + paddingY * 2);
    
    // Draw rounded background
    drawList->AddRectFilled(rectMin, rectMax, ColorToU32(color), Rounding::Badge);
    
    // Draw text
    ImVec4 finalTextColor = (textColor.x < 0) ? Colors::Foreground() : textColor;
    drawList->AddText(ImVec2(pos.x + paddingX, pos.y + paddingY), ColorToU32(finalTextColor), label);
    
    // Advance cursor
    ImGui::Dummy(ImVec2(textSize.x + paddingX * 2, textSize.y + paddingY * 2));
}

void StatusBadge(const char* label, bool active, bool critical) {
    ImVec4 bgColor, textColor;
    
    if (active) {
        if (critical) {
            bgColor = Colors::DestructiveBg();
            textColor = Colors::Destructive();
        } else {
            bgColor = Colors::SuccessBg();
            textColor = Colors::Success();
        }
    } else {
        bgColor = Colors::Muted();
        textColor = Colors::MutedForeground();
    }
    
    Badge(label, bgColor, textColor);
}

bool LabeledToggle(const char* label, bool* value) {
    ImGui::PushID(label);
    
    ImGui::Text("%s", label);
    ImGui::SameLine();
    
    // Custom toggle implementation
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    
    float width = 36.0f;
    float height = 20.0f;
    float radius = height * 0.5f;
    
    ImVec2 rectMin = pos;
    ImVec2 rectMax = ImVec2(pos.x + width, pos.y + height);
    
    // Interactive area
    ImGui::InvisibleButton("toggle", ImVec2(width, height));
    bool changed = false;
    if (ImGui::IsItemClicked()) {
        *value = !*value;
        changed = true;
    }
    
    // Draw background
    ImU32 bgColor = *value ? ColorToU32(Colors::Primary()) : ColorToU32(Colors::Muted());
    drawList->AddRectFilled(rectMin, rectMax, bgColor, radius);
    
    // Draw knob
    float knobX = *value ? (pos.x + width - radius) : (pos.x + radius);
    drawList->AddCircleFilled(ImVec2(knobX, pos.y + radius), radius - 3.0f, ColorToU32(Colors::Foreground()));
    
    ImGui::PopID();
    return changed;
}

bool LabeledSlider(const char* label, float* value, float minVal, float maxVal, const char* format) {
    ImGui::Text("%s", label);
    
    char idBuf[128];
    snprintf(idBuf, sizeof(idBuf), "##%s", label);
    
    ImGui::PushItemWidth(-1);
    bool changed = ImGui::SliderFloat(idBuf, value, minVal, maxVal, format);
    ImGui::PopItemWidth();
    
    return changed;
}

bool LabeledInput(const char* label, char* buf, size_t bufSize) {
    ImGui::Text("%s", label);
    
    char idBuf[128];
    snprintf(idBuf, sizeof(idBuf), "##%s", label);
    
    ImGui::PushItemWidth(-1);
    bool changed = ImGui::InputText(idBuf, buf, bufSize);
    ImGui::PopItemWidth();
    
    return changed;
}

bool IconButton(const char* icon, const char* tooltip, bool active, float size) {
    if (size <= 0) {
        size = ImGui::GetFrameHeight();
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    
    if (active) {
        ImGui::PushStyleColor(ImGuiCol_Button, Colors::Primary());
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::PrimaryForeground());
    }
    
    bool clicked = ImGui::Button(icon, ImVec2(size, size));
    
    if (active) {
        ImGui::PopStyleColor(2);
    }
    
    ImGui::PopStyleVar();
    
    if (tooltip && ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", tooltip);
    }
    
    return clicked;
}

void CircularProgress(const char* label, float value, float radius, float thickness,
                      const ImVec4& color, const ImVec4& bgColor) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(pos.x + radius, pos.y + radius);
    
    // Clamp value
    value = std::max(0.0f, std::min(1.0f, value));
    
    // Draw background arc (270 degrees, from bottom-left to bottom-right via top)
    float startAngle = static_cast<float>(M_PI) * 0.75f;  // 135 degrees
    float maxAngle = static_cast<float>(M_PI) * 1.5f;     // 270 degrees sweep
    int numSegments = 64;
    
    // Background arc
    drawList->PathArcTo(center, radius - thickness * 0.5f, startAngle, startAngle + maxAngle, numSegments);
    drawList->PathStroke(ColorToU32(bgColor), 0, thickness);
    
    // Progress arc
    if (value > 0.001f) {
        float progressAngle = maxAngle * value;
        drawList->PathArcTo(center, radius - thickness * 0.5f, startAngle, startAngle + progressAngle, numSegments);
        drawList->PathStroke(ColorToU32(color), 0, thickness);
    }
    
    // Draw center label
    if (label && label[0]) {
        ImVec2 textSize = ImGui::CalcTextSize(label);
        drawList->AddText(ImVec2(center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f), 
                         ColorToU32(Colors::Foreground()), label);
    }
    
    // Advance cursor
    ImGui::Dummy(ImVec2(radius * 2, radius * 2));
}

void ProgressBar(float value, const ImVec2& size, const ImVec4& color, const char* label) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    
    ImVec2 actualSize = size;
    if (actualSize.x <= 0) actualSize.x = ImGui::GetContentRegionAvail().x;
    if (actualSize.y <= 0) actualSize.y = 8.0f;
    
    ImVec2 rectMax = ImVec2(pos.x + actualSize.x, pos.y + actualSize.y);
    
    // Background
    drawList->AddRectFilled(pos, rectMax, ColorToU32(Colors::Muted()), Rounding::ProgressBar);
    
    // Progress
    value = std::max(0.0f, std::min(1.0f, value));
    if (value > 0.001f) {
        ImVec2 progressMax = ImVec2(pos.x + actualSize.x * value, pos.y + actualSize.y);
        drawList->AddRectFilled(pos, progressMax, ColorToU32(color), Rounding::ProgressBar);
    }
    
    // Label
    if (label && label[0]) {
        ImVec2 textSize = ImGui::CalcTextSize(label);
        drawList->AddText(ImVec2(pos.x + (actualSize.x - textSize.x) * 0.5f, 
                                  pos.y + (actualSize.y - textSize.y) * 0.5f),
                         ColorToU32(Colors::Foreground()), label);
    }
    
    ImGui::Dummy(actualSize);
}

void Sparkline(const float* values, int count, float minVal, float maxVal,
               const ImVec2& size, const ImVec4& color) {
    if (count < 2) return;
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    
    float range = maxVal - minVal;
    if (range < 0.001f) range = 1.0f;
    
    // Draw line
    for (int i = 0; i < count - 1; i++) {
        float x1 = pos.x + (static_cast<float>(i) / static_cast<float>(count - 1)) * size.x;
        float x2 = pos.x + (static_cast<float>(i + 1) / static_cast<float>(count - 1)) * size.x;
        float y1 = pos.y + size.y - ((values[i] - minVal) / range) * size.y;
        float y2 = pos.y + size.y - ((values[i + 1] - minVal) / range) * size.y;
        
        drawList->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ColorToU32(color), 2.0f);
    }
    
    ImGui::Dummy(size);
}

void LineChart(const char* label, const float* values, int count,
               float minVal, float maxVal, const ImVec2& size, const ImVec4& color) {
    if (count < 2) return;
    
    ImGui::Text("%s", label);
    
    ImVec2 actualSize = size;
    if (actualSize.x <= 0) actualSize.x = ImGui::GetContentRegionAvail().x;
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    
    // Background
    ImVec2 rectMax = ImVec2(pos.x + actualSize.x, pos.y + actualSize.y);
    drawList->AddRectFilled(pos, rectMax, ColorToU32(Colors::Muted()), Rounding::Frame);
    
    float range = maxVal - minVal;
    if (range < 0.001f) range = 1.0f;
    
    float padding = 4.0f;
    float chartWidth = actualSize.x - padding * 2;
    float chartHeight = actualSize.y - padding * 2;
    
    // Draw line
    for (int i = 0; i < count - 1; i++) {
        float x1 = pos.x + padding + (static_cast<float>(i) / static_cast<float>(count - 1)) * chartWidth;
        float x2 = pos.x + padding + (static_cast<float>(i + 1) / static_cast<float>(count - 1)) * chartWidth;
        float y1 = pos.y + padding + chartHeight - ((values[i] - minVal) / range) * chartHeight;
        float y2 = pos.y + padding + chartHeight - ((values[i + 1] - minVal) / range) * chartHeight;
        
        drawList->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ColorToU32(color), 2.0f);
    }
    
    ImGui::Dummy(actualSize);
}

void BarChart(const char* label, const float* values, int count,
              const char** labels, const ImVec2& size, const ImVec4& color) {
    if (count < 1) return;
    
    ImGui::Text("%s", label);
    
    ImVec2 actualSize = size;
    if (actualSize.x <= 0) actualSize.x = ImGui::GetContentRegionAvail().x;
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    
    // Find max value for scaling
    float maxVal = 0.001f;
    for (int i = 0; i < count; i++) {
        if (values[i] > maxVal) maxVal = values[i];
    }
    
    float padding = 4.0f;
    float barWidth = (actualSize.x - padding * 2) / static_cast<float>(count) - 4.0f;
    float chartHeight = actualSize.y - padding * 2;
    
    for (int i = 0; i < count; i++) {
        float x = pos.x + padding + static_cast<float>(i) * (barWidth + 4.0f);
        float barHeight = (values[i] / maxVal) * chartHeight;
        float y = pos.y + padding + chartHeight - barHeight;
        
        ImVec2 barMin = ImVec2(x, y);
        ImVec2 barMax = ImVec2(x + barWidth, pos.y + padding + chartHeight);
        
        drawList->AddRectFilled(barMin, barMax, ColorToU32(color), Rounding::Badge);
    }
    
    ImGui::Dummy(actualSize);
}

void SectionHeader(const char* label) {
    ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
}

void ValueDisplay(const char* label, const char* value, const char* unit, const ImVec4& valueColor) {
    ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
    
    ImGui::PushStyleColor(ImGuiCol_Text, valueColor);
    ImGui::TextUnformatted(value);
    ImGui::PopStyleColor();
    
    if (unit && unit[0]) {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
        ImGui::TextUnformatted(unit);
        ImGui::PopStyleColor();
    }
}

void KeyValue(const char* key, const char* value, const ImVec4& valueColor) {
    ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
    ImGui::TextUnformatted(key);
    ImGui::PopStyleColor();
    
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(value).x);
    
    ImGui::PushStyleColor(ImGuiCol_Text, valueColor);
    ImGui::TextUnformatted(value);
    ImGui::PopStyleColor();
}

void Separator() {
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}

void Space(float height) {
    ImGui::Dummy(ImVec2(0, height));
}

} // namespace widgets
} // namespace ui
