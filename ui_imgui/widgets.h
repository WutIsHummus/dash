#pragma once

#include "imgui.h"
#include "theme.h"
#include <string>
#include <vector>

namespace ui {
namespace widgets {

/**
 * Begin a styled card panel with rounded background and optional border
 * Uses BeginChild + ImDrawList for polished look
 * 
 * @param id Unique ID for the panel
 * @param size Size of the card (0,0 for auto)
 * @param border Whether to draw border
 * @return true if card is visible
 */
bool BeginCard(const char* id, const ImVec2& size = ImVec2(0, 0), bool border = true);

/**
 * End a card panel (must match BeginCard)
 */
void EndCard();

/**
 * Render a status badge/pill indicator
 * 
 * @param label Text to display
 * @param color Background color
 * @param textColor Text color (defaults to white if not specified)
 */
void Badge(const char* label, const ImVec4& color, const ImVec4& textColor = ImVec4(-1, 0, 0, 0));

/**
 * Render a colored status badge based on severity
 * Uses predefined colors for success, warning, critical states
 */
void StatusBadge(const char* label, bool active, bool critical = false);

/**
 * Render a labeled toggle switch
 * 
 * @param label Label text
 * @param value Toggle state (modified)
 * @return true if value changed
 */
bool LabeledToggle(const char* label, bool* value);

/**
 * Render a labeled slider
 * 
 * @param label Label text
 * @param value Current value (modified)
 * @param minVal Minimum value
 * @param maxVal Maximum value
 * @param format Printf format for value display
 * @return true if value changed
 */
bool LabeledSlider(const char* label, float* value, float minVal, float maxVal, const char* format = "%.1f");

/**
 * Render a labeled input field
 * 
 * @param label Label text
 * @param buf Input buffer
 * @param bufSize Buffer size
 * @return true if value changed
 */
bool LabeledInput(const char* label, char* buf, size_t bufSize);

/**
 * Render an icon button (works with or without icon font)
 * Falls back to text if no icon font available
 * 
 * @param icon Icon character/codepoint or text fallback
 * @param tooltip Optional tooltip text
 * @param active Whether button should show active state
 * @param size Button size (0 for auto)
 * @return true if clicked
 */
bool IconButton(const char* icon, const char* tooltip = nullptr, bool active = false, float size = 0);

/**
 * Render a circular progress/gauge indicator
 * 
 * @param label Text to display in center
 * @param value Current value (0-1)
 * @param radius Outer radius
 * @param thickness Arc thickness
 * @param color Progress color
 * @param bgColor Background arc color
 */
void CircularProgress(const char* label, float value, float radius = 60.0f, float thickness = 8.0f,
                      const ImVec4& color = Colors::Primary(), 
                      const ImVec4& bgColor = Colors::Muted());

/**
 * Render a horizontal progress bar with label
 * 
 * @param value Progress value (0-1)
 * @param size Size of bar
 * @param color Bar color
 * @param label Optional overlay label
 */
void ProgressBar(float value, const ImVec2& size = ImVec2(-1, 8), 
                 const ImVec4& color = Colors::Primary(),
                 const char* label = nullptr);

/**
 * Render a sparkline chart
 * 
 * @param values Array of values
 * @param count Number of values
 * @param minVal Minimum value (for scaling)
 * @param maxVal Maximum value (for scaling)
 * @param size Size of chart
 * @param color Line color
 */
void Sparkline(const float* values, int count, float minVal, float maxVal,
               const ImVec2& size = ImVec2(100, 30),
               const ImVec4& color = Colors::Primary());

/**
 * Render a simple line chart
 * 
 * @param label Chart title
 * @param values Array of values
 * @param count Number of values
 * @param minVal Minimum value
 * @param maxVal Maximum value
 * @param size Size of chart
 * @param color Line color
 */
void LineChart(const char* label, const float* values, int count, 
               float minVal, float maxVal,
               const ImVec2& size = ImVec2(-1, 100),
               const ImVec4& color = Colors::Primary());

/**
 * Render a bar chart
 * 
 * @param label Chart title
 * @param values Array of values
 * @param count Number of values
 * @param labels Optional array of bar labels
 * @param size Size of chart
 * @param color Bar color
 */
void BarChart(const char* label, const float* values, int count,
              const char** labels = nullptr,
              const ImVec2& size = ImVec2(-1, 100),
              const ImVec4& color = Colors::Primary());

/**
 * Render a section header with muted styling
 */
void SectionHeader(const char* label);

/**
 * Render a value display with label above
 * 
 * @param label Small label text above
 * @param value Large value text
 * @param unit Optional unit text after value
 * @param valueColor Color for value text
 */
void ValueDisplay(const char* label, const char* value, const char* unit = nullptr,
                  const ImVec4& valueColor = Colors::Foreground());

/**
 * Render a key-value row for info displays
 */
void KeyValue(const char* key, const char* value, const ImVec4& valueColor = Colors::Foreground());

/**
 * Separator with proper spacing
 */
void Separator();

/**
 * Spacing helper
 */
void Space(float height = 8.0f);

} // namespace widgets
} // namespace ui
