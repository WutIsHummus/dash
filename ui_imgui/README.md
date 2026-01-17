# Vehicle Dashboard - Dear ImGui Port

This directory contains the Dear ImGui (C++) port of the vehicle dashboard UI, converted from the original Next.js/React + TSX implementation.

## TSX to ImGui Mapping

| Original TSX File | ImGui Implementation |
|-------------------|---------------------|
| `app/page.tsx` | `dashboard.cpp` - `RenderDashboard()` |
| `components/dashboard.tsx` | `dashboard.cpp` - Main layout structure |
| `components/dashboard/speed-gauge.tsx` | `dashboard.cpp` - `RenderSpeedGauge()` |
| `components/dashboard/gear-indicator.tsx` | `dashboard.cpp` - `RenderGearIndicator()` |
| `components/dashboard/battery-panel.tsx` | `dashboard.cpp` - `RenderBatteryPanel()` |
| `components/dashboard/cruise-control.tsx` | `dashboard.cpp` - `RenderCruiseControl()` |
| `components/dashboard/system-status.tsx` | `dashboard.cpp` - `RenderSystemStatus()` |
| `components/dashboard/fault-panel.tsx` | `dashboard.cpp` - `RenderFaultPanel()` |
| `components/dashboard/camera-feed.tsx` | `dashboard.cpp` - `RenderCameraFeed()` |
| `components/dashboard/turn-indicator.tsx` | `dashboard.cpp` - `RenderTurnIndicator()` |
| `lib/types.ts` | `state.h` - `AppState` struct |
| `app/globals.css` | `theme.h/cpp` - Color palette and style |

## File Structure

```
ui_imgui/
├── ui.h           # Main integration header (include this)
├── state.h        # AppState definition and helpers
├── theme.h        # Color palette and style constants
├── theme.cpp      # ApplyTheme() implementation
├── widgets.h      # Reusable widget declarations
├── widgets.cpp    # Widget implementations (Card, Badge, ProgressBar, etc.)
├── dashboard.h    # Dashboard panel function declarations
├── dashboard.cpp  # Dashboard layout and panel implementations
└── README.md      # This file
```

## Quick Start

### 1. Include in your project

Add the `ui_imgui` directory to your project and include:

```cpp
#include "ui_imgui/ui.h"
```

### 2. Initialize

After creating your ImGui context:

```cpp
// Optional: provide custom font paths
ui::InitUI();  // Uses default ImGui font

// Or with custom fonts:
// ui::InitUI("path/to/font.ttf", "path/to/mono.ttf", "path/to/icons.ttf");
```

### 3. Render

In your main render loop:

```cpp
// Create and maintain state
static ui::AppState state = ui::CreateDefaultState();

// In render loop:
ImGui_ImplXXX_NewFrame();
ImGui::NewFrame();

ui::RenderUI(state);

ImGui::Render();
ImGui_ImplXXX_RenderDrawData(ImGui::GetDrawData());
```

### 4. Optional: Simulation

To simulate real-time data updates:

```cpp
// Call at regular intervals (e.g., every 100ms)
ui::UpdateSimulation(state, deltaTime);
```

## Theme Customization

### Colors

Edit `theme.h` to modify the color palette:

```cpp
namespace Colors {
    inline ImVec4 Primary()     { return ImVec4(0.30f, 0.80f, 0.77f, 1.0f); }  // Teal
    inline ImVec4 Success()     { return ImVec4(0.30f, 0.75f, 0.50f, 1.0f); }  // Green
    inline ImVec4 Warning()     { return ImVec4(0.95f, 0.75f, 0.25f, 1.0f); }  // Yellow
    inline ImVec4 Destructive() { return ImVec4(0.85f, 0.30f, 0.25f, 1.0f); }  // Red
    // ... more colors
}
```

### Spacing & Rounding

```cpp
namespace Spacing {
    constexpr float WindowPadding = 16.0f;
    constexpr float CardPadding = 16.0f;
    // ...
}

namespace Rounding {
    constexpr float Window = 10.0f;
    constexpr float Card = 8.0f;
    constexpr float Button = 6.0f;
    // ...
}
```

### Fonts

To use custom fonts, provide paths to `InitUI()`:

```cpp
ui::InitUI(
    "assets/fonts/Geist-Regular.ttf",      // Main UI font
    "assets/fonts/GeistMono-Regular.ttf",  // Monospace font
    "assets/fonts/fa-solid-900.ttf"        // Icon font (optional)
);
```

**Note:** If fonts are not found, the UI falls back to ImGui's default font.

## Camera Textures

The camera feeds show placeholder content by default. To display actual camera feeds:

1. Load your camera texture using your graphics API
2. Pass the texture ID to the state:

```cpp
state.rearCameraTexture = (void*)myRearTextureID;
state.sideCameraTexture = (void*)mySideTextureID;
```

## Backend Compatibility

The UI code is backend-agnostic. It works with any ImGui backend:

- **Vulkan:** `ImGui_ImplVulkan`
- **OpenGL:** `ImGui_ImplOpenGL3`
- **DirectX 11/12:** `ImGui_ImplDX11`, `ImGui_ImplDX12`
- **Metal:** `ImGui_ImplMetal`

No changes to your existing renderer are required - just call `RenderUI()` between `NewFrame()` and `Render()`.

## Design Notes

### Visual Fidelity

The ImGui port aims to match the original TSX design:

- Dark theme with blue-gray background (#1a1d2e)
- Teal/cyan primary accent color
- Rounded cards and buttons (8-10px radius)
- Subtle borders and separators
- Status badges with colored backgrounds

### Layout

- 3-column responsive layout
- Left: Battery status, System status
- Center: Speed gauge, Gear indicator, Cruise control, Camera feeds
- Right: Fault panel

### Interactivity

All interactive elements from the TSX are preserved:
- Turn signal toggles
- Gear selection (disabled when speed ≥5 km/h)
- Cruise control toggle and speed adjustment
- Contactor toggles
- Brake toggle
- Fault simulation (add/clear)

## Dependencies

- **Dear ImGui** (v1.89+)
- Standard C++ library

No additional dependencies required.

## TODO

- [ ] Load custom fonts (Geist) when available in assets/fonts
- [ ] Merge icon font (Font Awesome) for better icons
- [ ] Implement actual camera texture rendering
- [ ] Add animation for pulsing elements (turn signals, LIVE indicator)
