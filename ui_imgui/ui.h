#pragma once

/**
 * Vehicle Dashboard ImGui UI
 * 
 * Main integration header - include this in your render loop
 */

#include "state.h"
#include "theme.h"
#include "widgets.h"
#include "dashboard.h"

namespace ui {

/**
 * Initialize the UI system
 * Call once after creating ImGui context
 * 
 * @param fontPath Optional path to custom UI font
 * @param monoFontPath Optional path to monospace font
 * @param iconFontPath Optional path to icon font (Font Awesome, etc.)
 */
inline void InitUI(const char* fontPath = nullptr, 
                   const char* monoFontPath = nullptr,
                   const char* iconFontPath = nullptr) {
    ApplyTheme();
    LoadFonts(fontPath, monoFontPath, iconFontPath);
}

/**
 * Render the complete UI
 * Call this from your main render loop
 * 
 * @param state Application state (read/write access for interaction)
 * 
 * Example usage:
 * @code
 *   // In your render loop:
 *   ImGui_ImplXXX_NewFrame();
 *   ImGui::NewFrame();
 *   
 *   ui::RenderUI(appState);
 *   
 *   ImGui::Render();
 *   ImGui_ImplXXX_RenderDrawData(ImGui::GetDrawData());
 * @endcode
 */
inline void RenderUI(AppState& state) {
    RenderDashboard(state);
}

/**
 * Update simulation state (optional helper)
 * Call this at regular intervals to simulate real-time data updates
 * 
 * @param state Application state to update
 * @param deltaTime Time since last update in seconds
 */
inline void UpdateSimulation(AppState& state, float deltaTime) {
    // Increment heartbeat
    state.heartbeat = static_cast<uint8_t>((state.heartbeat + 1) % 256);
    
    // Simulate speed changes when main contactor is closed
    if (state.contactorStates.main) {
        // Random speed fluctuation
        float speedDelta = (static_cast<float>(rand()) / RAND_MAX - 0.3f) * 5.0f;
        state.speed = static_cast<int>(std::max(0.0f, std::min(120.0f, 
            static_cast<float>(state.speed) + speedDelta)));
    } else {
        state.speed = 0;
    }
    
    // Simulate current based on speed
    if (state.speed > 0) {
        state.mainBattery.current = -(50.0f + static_cast<float>(rand()) / RAND_MAX * 150.0f);
    } else {
        state.mainBattery.current = 0.0f;
    }
    
    // Slight voltage fluctuation
    state.mainBattery.voltage = 352.4f + (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f;
    
    // Slow SOC drain when current is negative
    if (state.mainBattery.current < 0) {
        state.mainBattery.soc = std::max(0.0f, state.mainBattery.soc - 0.001f);
    }
}

} // namespace ui
