"use client"

import { Battery, BatteryCharging, Zap } from "lucide-react"
import { cn } from "@/lib/utils"

interface BatteryPanelProps {
  mainBattery: {
    soc: number
    voltage: number
    current: number
  }
  suppBattery: {
    soc: number
    voltage: number
  }
}

export function BatteryPanel({ mainBattery, suppBattery }: BatteryPanelProps) {
  const isCharging = mainBattery.current > 0

  return (
    <div className="rounded-lg bg-card p-4">
      <h2 className="mb-4 text-sm font-medium text-muted-foreground">BATTERY STATUS</h2>

      {/* Main Battery */}
      <div className="mb-4 rounded-md bg-muted/50 p-3">
        <div className="mb-2 flex items-center justify-between">
          <div className="flex items-center gap-2">
            {isCharging ? (
              <BatteryCharging className="h-5 w-5 text-success" />
            ) : (
              <Battery className="h-5 w-5 text-primary" />
            )}
            <span className="text-sm font-medium text-foreground">Main HV</span>
          </div>
          <span
            className={cn(
              "text-2xl font-bold tabular-nums",
              mainBattery.soc < 20 ? "text-destructive" : mainBattery.soc < 40 ? "text-warning" : "text-primary",
            )}
          >
            {mainBattery.soc.toFixed(1)}%
          </span>
        </div>

        {/* SOC Bar */}
        <div className="mb-3 h-2 overflow-hidden rounded-full bg-muted">
          <div
            className={cn(
              "h-full rounded-full transition-all duration-300",
              mainBattery.soc < 20 ? "bg-destructive" : mainBattery.soc < 40 ? "bg-warning" : "bg-primary",
            )}
            style={{ width: `${mainBattery.soc}%` }}
          />
        </div>

        <div className="grid grid-cols-2 gap-2">
          <div className="rounded bg-background/50 p-2">
            <div className="text-xs text-muted-foreground">Voltage</div>
            <div className="font-mono text-sm font-semibold text-foreground">{mainBattery.voltage.toFixed(1)} V</div>
          </div>
          <div className="rounded bg-background/50 p-2">
            <div className="text-xs text-muted-foreground">Current</div>
            <div
              className={cn(
                "font-mono text-sm font-semibold",
                mainBattery.current < 0 ? "text-accent" : mainBattery.current > 0 ? "text-success" : "text-foreground",
              )}
            >
              {mainBattery.current > 0 ? "+" : ""}
              {mainBattery.current.toFixed(1)} A
            </div>
          </div>
        </div>

        {/* Power indicator */}
        {mainBattery.current !== 0 && (
          <div className="mt-2 flex items-center gap-2 rounded bg-background/50 p-2">
            <Zap className={cn("h-4 w-4", mainBattery.current < 0 ? "text-accent" : "text-success")} />
            <span className="text-xs text-muted-foreground">Power</span>
            <span
              className={cn(
                "ml-auto font-mono text-sm font-semibold",
                mainBattery.current < 0 ? "text-accent" : "text-success",
              )}
            >
              {Math.abs((mainBattery.voltage * mainBattery.current) / 1000).toFixed(1)} kW
            </span>
          </div>
        )}
      </div>

      {/* Supplementary Battery */}
      <div className="rounded-md bg-muted/50 p-3">
        <div className="mb-2 flex items-center justify-between">
          <div className="flex items-center gap-2">
            <Battery className="h-4 w-4 text-muted-foreground" />
            <span className="text-sm font-medium text-foreground">12V Aux</span>
          </div>
          <span
            className={cn(
              "text-lg font-bold tabular-nums",
              suppBattery.soc < 30 ? "text-destructive" : "text-foreground",
            )}
          >
            {suppBattery.soc}%
          </span>
        </div>

        <div className="h-1.5 overflow-hidden rounded-full bg-muted">
          <div
            className={cn(
              "h-full rounded-full transition-all",
              suppBattery.soc < 30 ? "bg-destructive" : "bg-muted-foreground",
            )}
            style={{ width: `${suppBattery.soc}%` }}
          />
        </div>

        <div className="mt-2 flex items-center justify-between text-xs">
          <span className="text-muted-foreground">Voltage</span>
          <span className="font-mono font-medium text-foreground">{suppBattery.voltage.toFixed(1)} V</span>
        </div>
      </div>
    </div>
  )
}
