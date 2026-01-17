#pragma once

#include "state.h"

namespace ui {

/**
 * Render the complete vehicle dashboard
 * Maps to components/dashboard.tsx layout
 * 
 * @param state Application state (read/write)
 */
void RenderDashboard(AppState& state);

// Individual panel render functions (internal use, but exposed for flexibility)

/**
 * Render the header bar with title, heartbeat display, and turn signals
 */
void RenderHeader(AppState& state);

/**
 * Render the speed gauge with circular arc
 * Maps to speed-gauge.tsx
 */
void RenderSpeedGauge(const AppState& state);

/**
 * Render the gear indicator (P/R/N/D)
 * Maps to gear-indicator.tsx
 */
void RenderGearIndicator(AppState& state);

/**
 * Render the battery status panel
 * Maps to battery-panel.tsx
 */
void RenderBatteryPanel(const AppState& state);

/**
 * Render the cruise control panel
 * Maps to cruise-control.tsx
 */
void RenderCruiseControl(AppState& state);

/**
 * Render the system status panel with contactors and brake
 * Maps to system-status.tsx
 */
void RenderSystemStatus(AppState& state);

/**
 * Render the fault panel
 * Maps to fault-panel.tsx
 */
void RenderFaultPanel(AppState& state);

/**
 * Render a camera feed placeholder
 * Maps to camera-feed.tsx
 * 
 * @param label Camera label text
 * @param type Camera type identifier
 * @param isActive Whether camera is currently active
 * @param texture Optional texture ID (placeholder if nullptr)
 */
void RenderCameraFeed(const char* label, const char* type, bool isActive, void* texture = nullptr);

/**
 * Render a turn indicator button
 * Maps to turn-indicator.tsx
 */
bool RenderTurnIndicator(bool isLeft, bool active);

} // namespace ui
