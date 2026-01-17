"use client"

import { useState, useEffect } from "react"
import { SpeedGauge } from "./dashboard/speed-gauge"
import { GearIndicator } from "./dashboard/gear-indicator"
import { BatteryPanel } from "./dashboard/battery-panel"
import { CruiseControl } from "./dashboard/cruise-control"
import { SystemStatus } from "./dashboard/system-status"
import { FaultPanel } from "./dashboard/fault-panel"
import { CameraFeed } from "./dashboard/camera-feed"
import { TurnIndicator } from "./dashboard/turn-indicator"
import type { VehicleState } from "@/lib/types"

const initialState: VehicleState = {
  speed: 0,
  gear: "P",
  mainBattery: {
    soc: 78,
    voltage: 352.4,
    current: 0,
  },
  suppBattery: {
    soc: 95,
    voltage: 12.6,
  },
  cruise: {
    enabled: false,
    setSpeed: 0,
  },
  brakeEngaged: true,
  contactorStates: {
    main: false,
    precharge: false,
    hvil: true,
  },
  heartbeat: 0,
  faults: [],
  turnSignal: null,
}

export function Dashboard() {
  const [state, setState] = useState<VehicleState>(initialState)

  // Simulate real-time data updates
  useEffect(() => {
    const interval = setInterval(() => {
      setState((prev) => {
        const newSpeed = prev.contactorStates.main
          ? Math.min(120, Math.max(0, prev.speed + (Math.random() - 0.3) * 5))
          : 0
        const newCurrent = newSpeed > 0 ? -(50 + Math.random() * 150) : 0

        return {
          ...prev,
          speed: Math.round(newSpeed),
          mainBattery: {
            ...prev.mainBattery,
            current: Math.round(newCurrent * 10) / 10,
            voltage: 352.4 + (Math.random() - 0.5) * 2,
            soc: Math.max(0, prev.mainBattery.soc - (newCurrent < 0 ? 0.001 : 0)),
          },
          heartbeat: (prev.heartbeat + 1) % 256,
        }
      })
    }, 100)

    return () => clearInterval(interval)
  }, [])

  const toggleContactor = (key: keyof typeof state.contactorStates) => {
    setState((prev) => ({
      ...prev,
      contactorStates: {
        ...prev.contactorStates,
        [key]: !prev.contactorStates[key],
      },
    }))
  }

  const toggleCruise = () => {
    setState((prev) => ({
      ...prev,
      cruise: {
        enabled: !prev.cruise.enabled,
        setSpeed: !prev.cruise.enabled ? prev.speed : 0,
      },
    }))
  }

  const adjustCruiseSpeed = (delta: number) => {
    if (!state.cruise.enabled) return
    setState((prev) => ({
      ...prev,
      cruise: {
        ...prev.cruise,
        setSpeed: Math.max(0, Math.min(200, prev.cruise.setSpeed + delta)),
      },
    }))
  }

  const setGear = (gear: VehicleState["gear"]) => {
    if (state.speed < 5) {
      setState((prev) => ({ ...prev, gear }))
    }
  }

  const toggleBrake = () => {
    setState((prev) => ({ ...prev, brakeEngaged: !prev.brakeEngaged }))
  }

  const toggleTurnSignal = (direction: "left" | "right" | null) => {
    setState((prev) => ({
      ...prev,
      turnSignal: prev.turnSignal === direction ? null : direction,
    }))
  }

  const addFault = () => {
    const faultCodes = [
      { code: "E001", message: "Battery temp high", severity: "warning" as const },
      { code: "E002", message: "Motor overheat", severity: "critical" as const },
      { code: "E003", message: "CAN timeout", severity: "warning" as const },
      { code: "E004", message: "HVIL open", severity: "critical" as const },
      { code: "E005", message: "Low 12V battery", severity: "info" as const },
    ]
    const randomFault = faultCodes[Math.floor(Math.random() * faultCodes.length)]
    setState((prev) => ({
      ...prev,
      faults: [...prev.faults.slice(-4), { ...randomFault, timestamp: Date.now() }],
    }))
  }

  const clearFaults = () => {
    setState((prev) => ({ ...prev, faults: [] }))
  }

  return (
    <div className="min-h-screen bg-background p-4 lg:p-6">
      <div className="mx-auto max-w-[1800px]">
        {/* Header */}
        <header className="mb-4 flex items-center justify-between">
          <h1 className="text-lg font-semibold text-foreground">Vehicle Telemetry</h1>
          <div className="flex items-center gap-4">
            <TurnIndicator
              direction="left"
              active={state.turnSignal === "left"}
              onClick={() => toggleTurnSignal("left")}
            />
            <div className="flex items-center gap-2 rounded-md bg-card px-3 py-1.5">
              <span className="text-xs text-muted-foreground">HB</span>
              <span className="font-mono text-sm text-primary">{state.heartbeat.toString().padStart(3, "0")}</span>
            </div>
            <TurnIndicator
              direction="right"
              active={state.turnSignal === "right"}
              onClick={() => toggleTurnSignal("right")}
            />
          </div>
        </header>

        {/* Main Grid */}
        <div className="grid gap-4 lg:grid-cols-12 lg:gap-6">
          {/* Left Column - Battery & System */}
          <div className="flex flex-col gap-4 lg:col-span-2">
            <BatteryPanel mainBattery={state.mainBattery} suppBattery={state.suppBattery} />
            <SystemStatus
              contactorStates={state.contactorStates}
              brakeEngaged={state.brakeEngaged}
              onToggleContactor={toggleContactor}
              onToggleBrake={toggleBrake}
            />
          </div>

          {/* Center Column - Speed, Gear, Cameras */}
          <div className="flex flex-col gap-4 lg:col-span-8">
            {/* Speed & Gear Row */}
            <div className="flex items-center justify-center gap-6">
              <GearIndicator currentGear={state.gear} onGearChange={setGear} disabled={state.speed >= 5} />
              <SpeedGauge speed={state.speed} maxSpeed={200} />
              <CruiseControl cruise={state.cruise} onToggle={toggleCruise} onAdjust={adjustCruiseSpeed} />
            </div>

            {/* Camera Feeds - now larger */}
            <div className="grid gap-4 sm:grid-cols-2">
              <CameraFeed label="Rear View" type="rear" isActive={true} />
              <CameraFeed
                label={
                  state.turnSignal === "left"
                    ? "Left Side"
                    : state.turnSignal === "right"
                      ? "Right Side"
                      : "Side Camera"
                }
                type={state.turnSignal || "side"}
                isActive={state.turnSignal !== null}
              />
            </div>
          </div>

          {/* Right Column - Faults (narrower) */}
          <div className="lg:col-span-2">
            <FaultPanel faults={state.faults} onAddFault={addFault} onClearFaults={clearFaults} />
          </div>
        </div>
      </div>
    </div>
  )
}
