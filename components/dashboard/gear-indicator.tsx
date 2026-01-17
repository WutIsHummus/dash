"use client"

import { cn } from "@/lib/utils"

interface GearIndicatorProps {
  currentGear: string
  onGearChange: (gear: string) => void
  disabled: boolean
}

const gears = ["P", "R", "N", "D"]

export function GearIndicator({ currentGear, onGearChange, disabled }: GearIndicatorProps) {
  return (
    <div className="flex flex-col items-center gap-1 rounded-lg bg-card p-3">
      <span className="mb-1 text-xs text-muted-foreground">GEAR</span>
      <div className="flex flex-col gap-1">
        {gears.map((gear) => (
          <button
            key={gear}
            onClick={() => onGearChange(gear)}
            disabled={disabled && gear !== currentGear}
            className={cn(
              "h-9 w-9 rounded font-mono text-lg font-bold transition-all",
              currentGear === gear
                ? "bg-primary text-primary-foreground"
                : "bg-muted text-muted-foreground hover:bg-secondary",
              disabled && gear !== currentGear && "cursor-not-allowed opacity-50",
            )}
          >
            {gear}
          </button>
        ))}
      </div>
    </div>
  )
}
