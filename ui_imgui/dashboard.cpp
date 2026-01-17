#include "dashboard.h"
#include "widgets.h"
#include "theme.h"
#include <cstdio>
#include <cmath>
#include <ctime>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace ui {

// Helper to format timestamp to HH:MM
static void FormatTime(int64_t timestamp, char* buf, size_t bufSize) {
    time_t t = static_cast<time_t>(timestamp / 1000);
    struct tm* tm_info = localtime(&t);
    if (tm_info) {
        snprintf(buf, bufSize, "%02d:%02d", tm_info->tm_hour, tm_info->tm_min);
    } else {
        snprintf(buf, bufSize, "--:--");
    }
}

// Get color based on battery SOC level
static ImVec4 GetBatteryColor(float soc) {
    if (soc < 20.0f) return Colors::Destructive();
    if (soc < 40.0f) return Colors::Warning();
    return Colors::Primary();
}

// Get color based on speed
static ImVec4 GetSpeedColor(int speed) {
    if (speed > 160) return Colors::Destructive();
    if (speed > 100) return Colors::Warning();
    return Colors::Primary();
}

void RenderDashboard(AppState& state) {
    ImGuiIO& io = ImGui::GetIO();
    
    // Create fullscreen window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | 
                             ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoBringToFrontOnFocus;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(Spacing::WindowPadding, Spacing::WindowPadding));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, Colors::Background());
    
    ImGui::Begin("Dashboard##Main", nullptr, flags);
    
    // Header
    RenderHeader(state);
    
    ImGui::Spacing();
    
    // Main content area - 3 column layout
    float availWidth = ImGui::GetContentRegionAvail().x;
    float leftColWidth = availWidth * 0.15f;
    float rightColWidth = availWidth * 0.15f;
    float centerColWidth = availWidth - leftColWidth - rightColWidth - Spacing::ItemSpacing * 2;
    
    // Constrain column widths
    leftColWidth = std::max(180.0f, std::min(250.0f, leftColWidth));
    rightColWidth = std::max(180.0f, std::min(250.0f, rightColWidth));
    centerColWidth = availWidth - leftColWidth - rightColWidth - Spacing::ItemSpacing * 2;
    
    // Left Column - Battery & System Status
    ImGui::BeginChild("##LeftColumn", ImVec2(leftColWidth, 0), ImGuiChildFlags_None);
    {
        RenderBatteryPanel(state);
        widgets::Space(Spacing::ItemSpacing);
        RenderSystemStatus(state);
    }
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    // Center Column - Speed, Gear, Cameras
    ImGui::BeginChild("##CenterColumn", ImVec2(centerColWidth, 0), ImGuiChildFlags_None);
    {
        // Speed & Gear Row
        float rowHeight = 220.0f;
        ImGui::BeginChild("##SpeedGearRow", ImVec2(0, rowHeight), ImGuiChildFlags_None);
        {
            float gearWidth = 80.0f;
            float cruiseWidth = 80.0f;
            float gaugeWidth = 200.0f;
            float totalWidth = gearWidth + cruiseWidth + gaugeWidth;
            float spacing = (ImGui::GetContentRegionAvail().x - totalWidth) / 4.0f;
            
            ImGui::SetCursorPosX(spacing);
            
            // Gear Indicator
            ImGui::BeginChild("##GearPanel", ImVec2(gearWidth, 0), ImGuiChildFlags_None);
            RenderGearIndicator(state);
            ImGui::EndChild();
            
            ImGui::SameLine(0, spacing);
            
            // Speed Gauge
            ImGui::BeginChild("##SpeedPanel", ImVec2(gaugeWidth, 0), ImGuiChildFlags_None);
            RenderSpeedGauge(state);
            ImGui::EndChild();
            
            ImGui::SameLine(0, spacing);
            
            // Cruise Control
            ImGui::BeginChild("##CruisePanel", ImVec2(cruiseWidth, 0), ImGuiChildFlags_None);
            RenderCruiseControl(state);
            ImGui::EndChild();
        }
        ImGui::EndChild();
        
        widgets::Space(Spacing::ItemSpacing);
        
        // Camera Feeds Row
        float cameraHeight = ImGui::GetContentRegionAvail().y;
        ImGui::BeginChild("##CameraRow", ImVec2(0, cameraHeight), ImGuiChildFlags_None);
        {
            float halfWidth = (ImGui::GetContentRegionAvail().x - Spacing::ItemSpacing) * 0.5f;
            
            // Rear View Camera
            ImGui::BeginChild("##RearCamera", ImVec2(halfWidth, 0), ImGuiChildFlags_None);
            RenderCameraFeed("Rear View", "rear", true, state.rearCameraTexture);
            ImGui::EndChild();
            
            ImGui::SameLine();
            
            // Side Camera (changes based on turn signal)
            ImGui::BeginChild("##SideCamera", ImVec2(halfWidth, 0), ImGuiChildFlags_None);
            const char* sideLabel = "Side Camera";
            const char* sideType = "side";
            bool sideActive = false;
            
            if (state.turnSignal == TurnSignal::Left) {
                sideLabel = "Left Side";
                sideType = "left";
                sideActive = true;
            } else if (state.turnSignal == TurnSignal::Right) {
                sideLabel = "Right Side";
                sideType = "right";
                sideActive = true;
            }
            
            RenderCameraFeed(sideLabel, sideType, sideActive, state.sideCameraTexture);
            ImGui::EndChild();
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    // Right Column - Faults
    ImGui::BeginChild("##RightColumn", ImVec2(rightColWidth, 0), ImGuiChildFlags_None);
    {
        RenderFaultPanel(state);
    }
    ImGui::EndChild();
    
    ImGui::End();
    
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void RenderHeader(AppState& state) {
    // Header bar with title, heartbeat, and turn signals
    float headerHeight = 40.0f;
    
    ImGui::BeginChild("##Header", ImVec2(0, headerHeight), ImGuiChildFlags_None);
    {
        // Title
        ImGui::SetCursorPosY((headerHeight - ImGui::GetTextLineHeight()) * 0.5f);
        ImGui::Text("Vehicle Telemetry");
        
        // Center section with turn signals and heartbeat
        float centerWidth = 200.0f;
        float centerX = (ImGui::GetContentRegionAvail().x - centerWidth) * 0.5f + ImGui::GetCursorPosX();
        
        ImGui::SameLine(centerX);
        ImGui::SetCursorPosY(5.0f);
        
        // Left turn indicator
        if (RenderTurnIndicator(true, state.turnSignal == TurnSignal::Left)) {
            state.turnSignal = (state.turnSignal == TurnSignal::Left) ? TurnSignal::None : TurnSignal::Left;
        }
        
        ImGui::SameLine();
        
        // Heartbeat display
        if (widgets::BeginCard("##Heartbeat", ImVec2(70, 30), true)) {
            char hbText[16];
            snprintf(hbText, sizeof(hbText), "HB %03d", state.heartbeat);
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::Primary());
            ImGui::SetCursorPosY(5.0f);
            ImGui::Text("%s", hbText);
            ImGui::PopStyleColor();
        }
        widgets::EndCard();
        
        ImGui::SameLine();
        
        // Right turn indicator
        if (RenderTurnIndicator(false, state.turnSignal == TurnSignal::Right)) {
            state.turnSignal = (state.turnSignal == TurnSignal::Right) ? TurnSignal::None : TurnSignal::Right;
        }
    }
    ImGui::EndChild();
}

void RenderSpeedGauge(const AppState& state) {
    if (!widgets::BeginCard("##SpeedGauge", ImVec2(0, 0), true)) {
        widgets::EndCard();
        return;
    }
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = ImGui::GetContentRegionAvail();
    
    float radius = std::min(size.x, size.y) * 0.45f;
    float thickness = 12.0f;
    ImVec2 center = ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f);
    
    // Calculate progress
    float maxSpeed = 200.0f;
    float percentage = static_cast<float>(state.speed) / maxSpeed;
    percentage = std::max(0.0f, std::min(1.0f, percentage));
    
    // Arc angles (270 degree arc, from bottom-left to bottom-right via top)
    float startAngle = static_cast<float>(M_PI) * 0.75f;  // 135 degrees
    float maxAngle = static_cast<float>(M_PI) * 1.5f;     // 270 degrees sweep
    int numSegments = 64;
    
    // Background arc
    drawList->PathArcTo(center, radius - thickness * 0.5f, startAngle, startAngle + maxAngle, numSegments);
    drawList->PathStroke(ColorToU32(Colors::Muted()), 0, thickness);
    
    // Progress arc
    if (percentage > 0.001f) {
        float progressAngle = maxAngle * percentage;
        ImVec4 speedColor = GetSpeedColor(state.speed);
        drawList->PathArcTo(center, radius - thickness * 0.5f, startAngle, startAngle + progressAngle, numSegments);
        drawList->PathStroke(ColorToU32(speedColor), 0, thickness);
    }
    
    // Center text
    char speedText[16];
    snprintf(speedText, sizeof(speedText), "%d", state.speed);
    
    // Large speed number
    ImVec2 speedSize = ImGui::CalcTextSize(speedText);
    ImGui::SetWindowFontScale(3.0f);
    speedSize = ImGui::CalcTextSize(speedText);
    drawList->AddText(ImVec2(center.x - speedSize.x * 0.5f, center.y - speedSize.y * 0.6f),
                     ColorToU32(Colors::Foreground()), speedText);
    ImGui::SetWindowFontScale(1.0f);
    
    // km/h unit
    const char* unit = "km/h";
    ImVec2 unitSize = ImGui::CalcTextSize(unit);
    drawList->AddText(ImVec2(center.x - unitSize.x * 0.5f, center.y + 20.0f),
                     ColorToU32(Colors::MutedForeground()), unit);
    
    ImGui::Dummy(size);
    
    widgets::EndCard();
}

void RenderGearIndicator(AppState& state) {
    if (!widgets::BeginCard("##GearIndicator", ImVec2(0, 0), true)) {
        widgets::EndCard();
        return;
    }
    
    widgets::SectionHeader("GEAR");
    widgets::Space(4.0f);
    
    const Gear gears[] = { Gear::Park, Gear::Reverse, Gear::Neutral, Gear::Drive };
    bool disabled = state.speed >= 5;
    
    for (const Gear& gear : gears) {
        const char* gearStr = GearToString(gear);
        bool isSelected = (state.gear == gear);
        bool isDisabled = disabled && !isSelected;
        
        ImVec2 buttonSize(40, 40);
        
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Button, Colors::Primary());
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::Primary());
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::PrimaryForeground());
        } else if (isDisabled) {
            ImGui::PushStyleColor(ImGuiCol_Button, Colors::Muted());
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::Muted());
            ImGui::PushStyleColor(ImGuiCol_Text, ColorWithAlpha(Colors::MutedForeground(), 0.5f));
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, Colors::Muted());
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::Secondary());
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
        }
        
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Button);
        
        char buttonId[32];
        snprintf(buttonId, sizeof(buttonId), "%s##Gear", gearStr);
        
        if (ImGui::Button(buttonId, buttonSize) && !isDisabled) {
            state.gear = gear;
        }
        
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
    }
    
    widgets::EndCard();
}

void RenderBatteryPanel(const AppState& state) {
    if (!widgets::BeginCard("##BatteryPanel", ImVec2(0, 0), true)) {
        widgets::EndCard();
        return;
    }
    
    widgets::SectionHeader("BATTERY STATUS");
    widgets::Space(Spacing::SmallPadding);
    
    // Main Battery Section
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ColorWithAlpha(Colors::Muted(), 0.5f));
    ImGui::BeginChild("##MainBattery", ImVec2(0, 150), ImGuiChildFlags_Borders);
    {
        ImGui::Spacing();
        
        // Icon and label
        bool isCharging = state.mainBattery.current > 0;
        ImVec4 batteryIconColor = isCharging ? Colors::Success() : Colors::Primary();
        
        ImGui::PushStyleColor(ImGuiCol_Text, batteryIconColor);
        ImGui::Text("[=]");  // Battery icon placeholder
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::Text("Main HV");
        
        // SOC percentage
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 60);
        ImVec4 socColor = GetBatteryColor(state.mainBattery.soc);
        ImGui::PushStyleColor(ImGuiCol_Text, socColor);
        ImGui::Text("%.1f%%", state.mainBattery.soc);
        ImGui::PopStyleColor();
        
        widgets::Space(4.0f);
        
        // SOC Progress Bar
        widgets::ProgressBar(state.mainBattery.soc / 100.0f, ImVec2(-1, 8), GetBatteryColor(state.mainBattery.soc));
        
        widgets::Space(Spacing::SmallPadding);
        
        // Voltage and Current row
        float halfWidth = (ImGui::GetContentRegionAvail().x - 8) * 0.5f;
        
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ColorWithAlpha(Colors::Background(), 0.5f));
        
        ImGui::BeginChild("##Voltage", ImVec2(halfWidth, 40), ImGuiChildFlags_None);
        {
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
            ImGui::Text("Voltage");
            ImGui::PopStyleColor();
            ImGui::Text("%.1f V", state.mainBattery.voltage);
        }
        ImGui::EndChild();
        
        ImGui::SameLine();
        
        ImGui::BeginChild("##Current", ImVec2(halfWidth, 40), ImGuiChildFlags_None);
        {
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
            ImGui::Text("Current");
            ImGui::PopStyleColor();
            
            ImVec4 currentColor = Colors::Foreground();
            if (state.mainBattery.current < 0) currentColor = Colors::Accent();
            else if (state.mainBattery.current > 0) currentColor = Colors::Success();
            
            ImGui::PushStyleColor(ImGuiCol_Text, currentColor);
            char prefix = state.mainBattery.current > 0 ? '+' : ' ';
            if (state.mainBattery.current >= 0) {
                ImGui::Text("+%.1f A", state.mainBattery.current);
            } else {
                ImGui::Text("%.1f A", state.mainBattery.current);
            }
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();
        
        ImGui::PopStyleColor();
        
        // Power display (if current is non-zero)
        if (std::abs(state.mainBattery.current) > 0.1f) {
            widgets::Space(4.0f);
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ColorWithAlpha(Colors::Background(), 0.5f));
            ImGui::BeginChild("##Power", ImVec2(0, 30), ImGuiChildFlags_None);
            {
                ImVec4 powerColor = state.mainBattery.current < 0 ? Colors::Accent() : Colors::Success();
                ImGui::PushStyleColor(ImGuiCol_Text, powerColor);
                ImGui::Text("[*]");  // Zap icon placeholder
                ImGui::PopStyleColor();
                
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
                ImGui::Text("Power");
                ImGui::PopStyleColor();
                
                float power = std::abs(state.mainBattery.voltage * state.mainBattery.current) / 1000.0f;
                ImGui::SameLine(ImGui::GetContentRegionAvail().x - 60);
                ImGui::PushStyleColor(ImGuiCol_Text, powerColor);
                ImGui::Text("%.1f kW", power);
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    
    widgets::Space(Spacing::SmallPadding);
    
    // Supplementary Battery Section
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ColorWithAlpha(Colors::Muted(), 0.5f));
    ImGui::BeginChild("##SuppBattery", ImVec2(0, 80), ImGuiChildFlags_Borders);
    {
        ImGui::Spacing();
        
        // Icon and label
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
        ImGui::Text("[=]");  // Battery icon
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::Text("12V Aux");
        
        // SOC percentage
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 40);
        ImVec4 suppSocColor = state.suppBattery.soc < 30 ? Colors::Destructive() : Colors::Foreground();
        ImGui::PushStyleColor(ImGuiCol_Text, suppSocColor);
        ImGui::Text("%d%%", static_cast<int>(state.suppBattery.soc));
        ImGui::PopStyleColor();
        
        widgets::Space(4.0f);
        
        // SOC Progress Bar (thinner)
        ImVec4 barColor = state.suppBattery.soc < 30 ? Colors::Destructive() : Colors::MutedForeground();
        widgets::ProgressBar(state.suppBattery.soc / 100.0f, ImVec2(-1, 6), barColor);
        
        widgets::Space(4.0f);
        
        // Voltage
        char voltageStr[16];
        snprintf(voltageStr, sizeof(voltageStr), "%.1f V", state.suppBattery.voltage);
        widgets::KeyValue("Voltage", voltageStr, Colors::Foreground());
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    
    widgets::EndCard();
}

void RenderCruiseControl(AppState& state) {
    if (!widgets::BeginCard("##CruiseControl", ImVec2(0, 0), true)) {
        widgets::EndCard();
        return;
    }
    
    widgets::SectionHeader("CRUISE");
    widgets::Space(4.0f);
    
    // Center the button
    float buttonSize = 48.0f;
    float contentWidth = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (contentWidth - buttonSize) * 0.5f);
    
    // Cruise toggle button
    ImVec4 btnColor = state.cruise.enabled ? Colors::Primary() : Colors::Muted();
    ImVec4 textColor = state.cruise.enabled ? Colors::PrimaryForeground() : Colors::MutedForeground();
    
    ImGui::PushStyleColor(ImGuiCol_Button, btnColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, state.cruise.enabled ? Colors::PrimaryDark() : Colors::Secondary());
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, buttonSize * 0.5f);
    
    if (ImGui::Button("CC##CruiseToggle", ImVec2(buttonSize, buttonSize))) {
        state.cruise.enabled = !state.cruise.enabled;
        if (state.cruise.enabled) {
            state.cruise.setSpeed = state.speed;
        } else {
            state.cruise.setSpeed = 0;
        }
    }
    
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);
    
    // Show set speed and adjustment buttons when enabled
    if (state.cruise.enabled) {
        widgets::Space(8.0f);
        
        // Set speed display
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (contentWidth - 50) * 0.5f);
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::Primary());
        ImGui::Text("%d", state.cruise.setSpeed);
        ImGui::PopStyleColor();
        
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (contentWidth - 35) * 0.5f);
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
        ImGui::Text("km/h");
        ImGui::PopStyleColor();
        
        widgets::Space(4.0f);
        
        // Adjustment buttons
        float btnWidth = 30.0f;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (contentWidth - btnWidth * 2 - 4) * 0.5f);
        
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Button);
        
        if (ImGui::Button("^##CruiseUp", ImVec2(btnWidth, btnWidth))) {
            state.cruise.setSpeed = std::min(200, state.cruise.setSpeed + 5);
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("v##CruiseDown", ImVec2(btnWidth, btnWidth))) {
            state.cruise.setSpeed = std::max(0, state.cruise.setSpeed - 5);
        }
        
        ImGui::PopStyleVar();
    }
    
    widgets::EndCard();
}

void RenderSystemStatus(AppState& state) {
    if (!widgets::BeginCard("##SystemStatus", ImVec2(0, 0), true)) {
        widgets::EndCard();
        return;
    }
    
    widgets::SectionHeader("SYSTEM STATUS");
    widgets::Space(Spacing::SmallPadding);
    
    // Contactors section
    ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
    ImGui::Text("CONTACTORS");
    ImGui::PopStyleColor();
    widgets::Space(4.0f);
    
    // Main contactor button
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ColorWithAlpha(Colors::Muted(), 0.5f));
    ImGui::BeginChild("##MainContactor", ImVec2(0, 35), ImGuiChildFlags_Borders);
    {
        ImGui::SetCursorPosY(7);
        
        // Icon
        ImVec4 iconColor = state.contactorStates.main ? Colors::Success() : Colors::MutedForeground();
        ImGui::PushStyleColor(ImGuiCol_Text, iconColor);
        ImGui::Text("[O]");  // Power icon placeholder
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::Text("Main");
        
        // Status badge
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50);
        if (state.contactorStates.main) {
            widgets::Badge("CLOSED", Colors::SuccessBg(), Colors::Success());
        } else {
            widgets::Badge("OPEN", Colors::Muted(), Colors::MutedForeground());
        }
        
        // Make it clickable
        ImGui::SetCursorPos(ImVec2(0, 0));
        if (ImGui::InvisibleButton("##MainContactorBtn", ImGui::GetContentRegionAvail())) {
            state.contactorStates.main = !state.contactorStates.main;
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    
    widgets::Space(4.0f);
    
    // Precharge contactor button
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ColorWithAlpha(Colors::Muted(), 0.5f));
    ImGui::BeginChild("##PrechargeContactor", ImVec2(0, 35), ImGuiChildFlags_Borders);
    {
        ImGui::SetCursorPosY(7);
        
        // Icon
        ImVec4 iconColor = state.contactorStates.precharge ? Colors::Success() : Colors::MutedForeground();
        ImGui::PushStyleColor(ImGuiCol_Text, iconColor);
        ImGui::Text("(*)");  // Circle icon placeholder
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::Text("Precharge");
        
        // Status badge
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 55);
        if (state.contactorStates.precharge) {
            widgets::Badge("ACTIVE", Colors::SuccessBg(), Colors::Success());
        } else {
            widgets::Badge("INACTIVE", Colors::Muted(), Colors::MutedForeground());
        }
        
        // Make it clickable
        ImGui::SetCursorPos(ImVec2(0, 0));
        if (ImGui::InvisibleButton("##PrechargeBtn", ImGui::GetContentRegionAvail())) {
            state.contactorStates.precharge = !state.contactorStates.precharge;
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    
    widgets::Space(4.0f);
    
    // HVIL status (read-only display)
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ColorWithAlpha(Colors::Muted(), 0.5f));
    ImGui::BeginChild("##HVILStatus", ImVec2(0, 35), ImGuiChildFlags_Borders);
    {
        ImGui::SetCursorPosY(7);
        
        // Icon
        ImVec4 iconColor = state.contactorStates.hvil ? Colors::Success() : Colors::Destructive();
        ImGui::PushStyleColor(ImGuiCol_Text, iconColor);
        ImGui::Text("/!\\");  // Warning icon placeholder
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::Text("HVIL");
        
        // Status badge
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 40);
        if (state.contactorStates.hvil) {
            widgets::Badge("OK", Colors::SuccessBg(), Colors::Success());
        } else {
            widgets::Badge("FAULT", Colors::DestructiveBg(), Colors::Destructive());
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    
    widgets::Space(Spacing::SmallPadding);
    
    // Brake section
    ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
    ImGui::Text("BRAKE");
    ImGui::PopStyleColor();
    widgets::Space(4.0f);
    
    // Brake status button
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ColorWithAlpha(Colors::Muted(), 0.5f));
    ImGui::BeginChild("##BrakeStatus", ImVec2(0, 35), ImGuiChildFlags_Borders);
    {
        ImGui::SetCursorPosY(7);
        
        // Status indicator dot
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 dotPos = ImGui::GetCursorScreenPos();
        dotPos.x += 8;
        dotPos.y += 5;
        ImVec4 dotColor = state.brakeEngaged ? Colors::Destructive() : Colors::MutedForeground();
        drawList->AddCircleFilled(dotPos, 6, ColorToU32(dotColor));
        
        ImGui::SetCursorPosX(24);
        ImGui::Text("Brake Pedal");
        
        // Status badge
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 55);
        if (state.brakeEngaged) {
            widgets::Badge("ENGAGED", Colors::DestructiveBg(), Colors::Destructive());
        } else {
            widgets::Badge("RELEASED", Colors::Muted(), Colors::MutedForeground());
        }
        
        // Make it clickable
        ImGui::SetCursorPos(ImVec2(0, 0));
        if (ImGui::InvisibleButton("##BrakeBtn", ImGui::GetContentRegionAvail())) {
            state.brakeEngaged = !state.brakeEngaged;
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    
    widgets::EndCard();
}

void RenderFaultPanel(AppState& state) {
    if (!widgets::BeginCard("##FaultPanel", ImVec2(0, 0), true)) {
        widgets::EndCard();
        return;
    }
    
    // Header with action buttons
    ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
    ImGui::Text("FAULTS");
    ImGui::PopStyleColor();
    
    // Action buttons
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 40);
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
    
    if (ImGui::Button("+##AddFault")) {
        // Add a random fault
        static const struct { const char* code; const char* msg; FaultSeverity sev; } faultTemplates[] = {
            { "E001", "Battery temp high", FaultSeverity::Warning },
            { "E002", "Motor overheat", FaultSeverity::Critical },
            { "E003", "CAN timeout", FaultSeverity::Warning },
            { "E004", "HVIL open", FaultSeverity::Critical },
            { "E005", "Low 12V battery", FaultSeverity::Info },
        };
        
        int idx = static_cast<int>(state.heartbeat) % 5;  // Pseudo-random based on heartbeat
        
        Fault newFault;
        newFault.code = faultTemplates[idx].code;
        newFault.message = faultTemplates[idx].msg;
        newFault.severity = faultTemplates[idx].sev;
        newFault.timestamp = static_cast<int64_t>(time(nullptr)) * 1000;
        
        // Keep only last 5 faults
        if (state.faults.size() >= 5) {
            state.faults.erase(state.faults.begin());
        }
        state.faults.push_back(newFault);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Simulate fault");
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("X##ClearFaults")) {
        state.faults.clear();
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Clear faults");
    }
    
    ImGui::PopStyleVar();
    
    widgets::Space(Spacing::SmallPadding);
    
    // Fault list
    if (state.faults.empty()) {
        // No faults message
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30);
        float contentWidth = ImGui::GetContentRegionAvail().x;
        
        // Success icon background
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 iconCenter = ImGui::GetCursorScreenPos();
        iconCenter.x += contentWidth * 0.5f;
        iconCenter.y += 15;
        
        drawList->AddCircleFilled(iconCenter, 15, ColorToU32(Colors::SuccessBg()));
        drawList->AddText(ImVec2(iconCenter.x - 4, iconCenter.y - 6), ColorToU32(Colors::Success()), "i");
        
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);
        
        float textWidth = ImGui::CalcTextSize("No faults").x;
        ImGui::SetCursorPosX((contentWidth - textWidth) * 0.5f);
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
        ImGui::Text("No faults");
        ImGui::PopStyleColor();
    } else {
        // Display faults
        for (size_t i = 0; i < state.faults.size(); i++) {
            const Fault& fault = state.faults[i];
            
            // Determine colors based on severity
            ImVec4 bgColor, borderColor, iconColor;
            switch (fault.severity) {
                case FaultSeverity::Critical:
                    bgColor = Colors::DestructiveBg();
                    borderColor = ColorWithAlpha(Colors::Destructive(), 0.5f);
                    iconColor = Colors::Destructive();
                    break;
                case FaultSeverity::Warning:
                    bgColor = Colors::WarningBg();
                    borderColor = ColorWithAlpha(Colors::Warning(), 0.5f);
                    iconColor = Colors::Warning();
                    break;
                case FaultSeverity::Info:
                default:
                    bgColor = Colors::PrimaryBg();
                    borderColor = ColorWithAlpha(Colors::Primary(), 0.5f);
                    iconColor = Colors::Primary();
                    break;
            }
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
            ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
            
            char childId[32];
            snprintf(childId, sizeof(childId), "##Fault%zu", i);
            ImGui::BeginChild(childId, ImVec2(0, 50), ImGuiChildFlags_Borders);
            {
                ImGui::Spacing();
                
                // Icon
                ImGui::PushStyleColor(ImGuiCol_Text, iconColor);
                if (fault.severity == FaultSeverity::Critical) {
                    ImGui::Text("/!\\");
                } else if (fault.severity == FaultSeverity::Warning) {
                    ImGui::Text("(!)");
                } else {
                    ImGui::Text("(i)");
                }
                ImGui::PopStyleColor();
                
                ImGui::SameLine();
                
                // Code and timestamp
                ImGui::Text("%s", fault.code.c_str());
                
                char timeStr[16];
                FormatTime(fault.timestamp, timeStr, sizeof(timeStr));
                ImGui::SameLine(ImGui::GetContentRegionAvail().x - 30);
                ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
                ImGui::Text("%s", timeStr);
                ImGui::PopStyleColor();
                
                // Message
                ImGui::SetCursorPosX(30);
                ImGui::Text("%s", fault.message.c_str());
            }
            ImGui::EndChild();
            
            ImGui::PopStyleColor(2);
            
            widgets::Space(4.0f);
        }
        
        widgets::Space(4.0f);
        
        // Active fault count
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ColorWithAlpha(Colors::Muted(), 0.5f));
        ImGui::BeginChild("##FaultCount", ImVec2(0, 25), ImGuiChildFlags_None);
        {
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::MutedForeground());
            ImGui::Text("Active");
            ImGui::PopStyleColor();
            
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - 20);
            
            bool hasCritical = false;
            for (const auto& f : state.faults) {
                if (f.severity == FaultSeverity::Critical) {
                    hasCritical = true;
                    break;
                }
            }
            
            ImVec4 countColor = hasCritical ? Colors::Destructive() : Colors::Warning();
            ImGui::PushStyleColor(ImGuiCol_Text, countColor);
            ImGui::Text("%zu", state.faults.size());
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
    }
    
    widgets::EndCard();
}

void RenderCameraFeed(const char* label, const char* type, bool isActive, void* texture) {
    if (!widgets::BeginCard("", ImVec2(0, 0), true)) {
        widgets::EndCard();
        return;
    }
    
    // Header
    ImGui::PushStyleColor(ImGuiCol_ChildBg, Colors::Card());
    ImGui::BeginChild("##CameraHeader", ImVec2(0, 30), ImGuiChildFlags_Borders);
    {
        ImGui::SetCursorPosY(5);
        
        // Camera icon
        ImVec4 iconColor = isActive ? Colors::Success() : Colors::MutedForeground();
        ImGui::PushStyleColor(ImGuiCol_Text, iconColor);
        ImGui::Text("[V]");  // Video icon placeholder
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::Text("%s", label);
        
        // Maximize button
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 20);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        if (ImGui::Button("[+]##Maximize")) {
            // TODO: Implement fullscreen camera view
        }
        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    
    // Camera feed area
    ImVec2 feedSize = ImGui::GetContentRegionAvail();
    
    ImGui::PushStyleColor(ImGuiCol_ChildBg, Colors::Muted());
    ImGui::BeginChild("##CameraFeed", feedSize, ImGuiChildFlags_None);
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        
        // Draw placeholder background
        if (!isActive) {
            // Dimmed overlay
            drawList->AddRectFilled(pos, ImVec2(pos.x + feedSize.x, pos.y + feedSize.y),
                                   ColorToU32(ColorWithAlpha(Colors::Background(), 0.5f)));
            
            // Inactive message
            ImVec2 center = ImVec2(pos.x + feedSize.x * 0.5f, pos.y + feedSize.y * 0.5f);
            
            // Camera off icon
            const char* offIcon = "[X]";
            ImVec2 iconSize = ImGui::CalcTextSize(offIcon);
            drawList->AddText(ImVec2(center.x - iconSize.x * 0.5f, center.y - 20),
                            ColorToU32(Colors::MutedForeground()), offIcon);
            
            const char* offText = "Camera inactive";
            ImVec2 textSize = ImGui::CalcTextSize(offText);
            drawList->AddText(ImVec2(center.x - textSize.x * 0.5f, center.y + 10),
                            ColorToU32(Colors::MutedForeground()), offText);
        } else {
            // TODO: If texture is provided, draw it using ImGui::Image
            // For now, draw placeholder with "LIVE" indicator
            
            // Placeholder pattern
            drawList->AddRectFilled(pos, ImVec2(pos.x + feedSize.x, pos.y + feedSize.y),
                                   ColorToU32(Colors::Muted()));
            
            // Grid pattern to simulate camera feed
            ImU32 lineColor = ColorToU32(ColorWithAlpha(Colors::MutedForeground(), 0.2f));
            for (float x = pos.x; x < pos.x + feedSize.x; x += 20) {
                drawList->AddLine(ImVec2(x, pos.y), ImVec2(x, pos.y + feedSize.y), lineColor);
            }
            for (float y = pos.y; y < pos.y + feedSize.y; y += 20) {
                drawList->AddLine(ImVec2(pos.x, y), ImVec2(pos.x + feedSize.x, y), lineColor);
            }
            
            // LIVE indicator
            float indicatorX = pos.x + 10;
            float indicatorY = pos.y + 10;
            
            drawList->AddRectFilled(ImVec2(indicatorX, indicatorY),
                                   ImVec2(indicatorX + 50, indicatorY + 20),
                                   ColorToU32(ColorWithAlpha(Colors::Background(), 0.8f)),
                                   Rounding::Badge);
            
            // Pulsing red dot
            drawList->AddCircleFilled(ImVec2(indicatorX + 10, indicatorY + 10), 4,
                                     ColorToU32(Colors::Destructive()));
            
            drawList->AddText(ImVec2(indicatorX + 20, indicatorY + 3),
                            ColorToU32(Colors::Foreground()), "LIVE");
            
            // Draw trajectory lines for rear camera
            if (strcmp(type, "rear") == 0) {
                float cx = pos.x + feedSize.x * 0.5f;
                float bottom = pos.y + feedSize.y;
                
                // Green trajectory lines
                ImU32 greenLine = ColorToU32(ImVec4(0.13f, 0.77f, 0.37f, 0.6f));
                drawList->AddBezierQuadratic(
                    ImVec2(cx - 40, bottom),
                    ImVec2(cx - 20, bottom - feedSize.y * 0.4f),
                    ImVec2(cx, bottom - feedSize.y * 0.6f),
                    greenLine, 2.0f);
                drawList->AddBezierQuadratic(
                    ImVec2(cx + 40, bottom),
                    ImVec2(cx + 20, bottom - feedSize.y * 0.4f),
                    ImVec2(cx, bottom - feedSize.y * 0.6f),
                    greenLine, 2.0f);
                
                // Yellow distance markers
                ImU32 yellowLine = ColorToU32(ImVec4(0.98f, 0.80f, 0.08f, 0.6f));
                float y1 = bottom - feedSize.y * 0.15f;
                float y2 = bottom - feedSize.y * 0.30f;
                drawList->AddLine(ImVec2(cx - 60, y1), ImVec2(cx + 60, y1), yellowLine, 2.0f);
                drawList->AddLine(ImVec2(cx - 45, y2), ImVec2(cx + 45, y2), yellowLine, 2.0f);
                
                // Red close distance marker
                ImU32 redLine = ColorToU32(ImVec4(0.94f, 0.27f, 0.27f, 0.6f));
                float y3 = bottom - feedSize.y * 0.45f;
                drawList->AddLine(ImVec2(cx - 30, y3), ImVec2(cx + 30, y3), redLine, 2.0f);
            }
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    
    widgets::EndCard();
}

bool RenderTurnIndicator(bool isLeft, bool active) {
    ImVec2 buttonSize(40, 40);
    
    ImVec4 bgColor = active ? Colors::Accent() : Colors::Card();
    ImVec4 textColor = active ? Colors::AccentForeground() : Colors::MutedForeground();
    
    ImGui::PushStyleColor(ImGuiCol_Button, bgColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, active ? Colors::Accent() : Colors::Muted());
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding::Card);
    
    const char* icon = isLeft ? "<" : ">";
    char buttonId[32];
    snprintf(buttonId, sizeof(buttonId), "%s##Turn%s", icon, isLeft ? "Left" : "Right");
    
    bool clicked = ImGui::Button(buttonId, buttonSize);
    
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);
    
    return clicked;
}

} // namespace ui
