#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace ui {

/**
 * Fault severity levels matching TSX implementation
 */
enum class FaultSeverity {
    Info,
    Warning,
    Critical
};

/**
 * Gear positions for the vehicle
 */
enum class Gear {
    Park,
    Reverse,
    Neutral,
    Drive
};

/**
 * Turn signal states
 */
enum class TurnSignal {
    None,
    Left,
    Right
};

/**
 * Individual fault record
 */
struct Fault {
    std::string code;
    std::string message;
    FaultSeverity severity;
    int64_t timestamp;  // Unix timestamp in milliseconds
};

/**
 * Main battery state
 */
struct MainBattery {
    float soc;      // State of charge (0-100)
    float voltage;  // Voltage in V
    float current;  // Current in A (negative = discharging)
};

/**
 * Supplementary (12V) battery state
 */
struct SuppBattery {
    float soc;      // State of charge (0-100)
    float voltage;  // Voltage in V
};

/**
 * Cruise control state
 */
struct CruiseControl {
    bool enabled;
    int setSpeed;  // Target speed in km/h
};

/**
 * Contactor states for high voltage system
 */
struct ContactorStates {
    bool main;
    bool precharge;
    bool hvil;  // High Voltage Interlock Loop
};

/**
 * Complete application state mirroring VehicleState from TSX
 * All widgets read/write from this struct - no globals.
 */
struct AppState {
    int speed;              // Current speed in km/h
    Gear gear;
    MainBattery mainBattery;
    SuppBattery suppBattery;
    CruiseControl cruise;
    bool brakeEngaged;
    ContactorStates contactorStates;
    uint8_t heartbeat;      // 0-255 cycling heartbeat counter
    std::vector<Fault> faults;
    TurnSignal turnSignal;

    // Camera texture IDs - placeholders for actual textures
    // TODO: Load actual textures when available
    void* rearCameraTexture = nullptr;
    void* sideCameraTexture = nullptr;
};

/**
 * Initialize AppState with default values matching TSX initialState
 */
inline AppState CreateDefaultState() {
    AppState state{};
    state.speed = 0;
    state.gear = Gear::Park;
    state.mainBattery = { 78.0f, 352.4f, 0.0f };
    state.suppBattery = { 95.0f, 12.6f };
    state.cruise = { false, 0 };
    state.brakeEngaged = true;
    state.contactorStates = { false, false, true };
    state.heartbeat = 0;
    state.turnSignal = TurnSignal::None;
    return state;
}

/**
 * Helper to convert Gear enum to display string
 */
inline const char* GearToString(Gear gear) {
    switch (gear) {
        case Gear::Park:    return "P";
        case Gear::Reverse: return "R";
        case Gear::Neutral: return "N";
        case Gear::Drive:   return "D";
        default:            return "?";
    }
}

/**
 * Helper to get Gear from index (for iteration)
 */
inline Gear GearFromIndex(int index) {
    switch (index) {
        case 0: return Gear::Park;
        case 1: return Gear::Reverse;
        case 2: return Gear::Neutral;
        case 3: return Gear::Drive;
        default: return Gear::Park;
    }
}

} // namespace ui
