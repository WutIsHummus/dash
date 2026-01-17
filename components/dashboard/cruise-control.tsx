"use client"

import { Gauge, ChevronUp, ChevronDown } from "lucide-react"
import { cn } from "@/lib/utils"

interface CruiseControlProps {
  cruise: {
    enabled: boolean
    setSpeed: number
  }
  onToggle: () => void
  onAdjust: (delta: number) => void
}

export function CruiseControl({ cruise, onToggle, onAdjust }: CruiseControlProps) {
  return (
    <div className="flex flex-col items-center gap-2 rounded-lg bg-card p-3">
      <span className="text-xs text-muted-foreground">CRUISE</span>

      <button
        onClick={onToggle}
        className={cn(
          "flex h-12 w-12 items-center justify-center rounded-full transition-all",
          cruise.enabled ? "bg-primary text-primary-foreground" : "bg-muted text-muted-foreground hover:bg-secondary",
        )}
      >
        <Gauge className="h-6 w-6" />
      </button>

      {cruise.enabled && (
        <>
          <div className="text-center">
            <div className="font-mono text-2xl font-bold text-primary">{cruise.setSpeed}</div>
            <div className="text-xs text-muted-foreground">km/h</div>
          </div>

          <div className="flex gap-1">
            <button
              onClick={() => onAdjust(5)}
              className="rounded bg-muted p-1.5 text-muted-foreground hover:bg-secondary hover:text-foreground"
            >
              <ChevronUp className="h-4 w-4" />
            </button>
            <button
              onClick={() => onAdjust(-5)}
              className="rounded bg-muted p-1.5 text-muted-foreground hover:bg-secondary hover:text-foreground"
            >
              <ChevronDown className="h-4 w-4" />
            </button>
          </div>
        </>
      )}
    </div>
  )
}
