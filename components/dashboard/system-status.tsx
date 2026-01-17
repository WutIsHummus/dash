"use client"

import { Power, Circle, AlertTriangle } from "lucide-react"
import { cn } from "@/lib/utils"

interface SystemStatusProps {
  contactorStates: {
    main: boolean
    precharge: boolean
    hvil: boolean
  }
  brakeEngaged: boolean
  onToggleContactor: (key: "main" | "precharge" | "hvil") => void
  onToggleBrake: () => void
}

export function SystemStatus({ contactorStates, brakeEngaged, onToggleContactor, onToggleBrake }: SystemStatusProps) {
  return (
    <div className="rounded-lg bg-card p-4">
      <h2 className="mb-4 text-sm font-medium text-muted-foreground">SYSTEM STATUS</h2>

      {/* Contactors */}
      <div className="mb-4 space-y-2">
        <h3 className="text-xs font-medium text-muted-foreground">CONTACTORS</h3>

        <button
          onClick={() => onToggleContactor("main")}
          className="flex w-full items-center justify-between rounded-md bg-muted/50 p-2 transition-colors hover:bg-muted"
        >
          <div className="flex items-center gap-2">
            <Power className={cn("h-4 w-4", contactorStates.main ? "text-success" : "text-muted-foreground")} />
            <span className="text-sm text-foreground">Main</span>
          </div>
          <span
            className={cn(
              "rounded px-2 py-0.5 text-xs font-medium",
              contactorStates.main ? "bg-success/20 text-success" : "bg-muted text-muted-foreground",
            )}
          >
            {contactorStates.main ? "CLOSED" : "OPEN"}
          </span>
        </button>

        <button
          onClick={() => onToggleContactor("precharge")}
          className="flex w-full items-center justify-between rounded-md bg-muted/50 p-2 transition-colors hover:bg-muted"
        >
          <div className="flex items-center gap-2">
            <Circle
              className={cn(
                "h-4 w-4",
                contactorStates.precharge ? "text-success fill-success" : "text-muted-foreground",
              )}
            />
            <span className="text-sm text-foreground">Precharge</span>
          </div>
          <span
            className={cn(
              "rounded px-2 py-0.5 text-xs font-medium",
              contactorStates.precharge ? "bg-success/20 text-success" : "bg-muted text-muted-foreground",
            )}
          >
            {contactorStates.precharge ? "ACTIVE" : "INACTIVE"}
          </span>
        </button>

        <div className="flex w-full items-center justify-between rounded-md bg-muted/50 p-2">
          <div className="flex items-center gap-2">
            <AlertTriangle className={cn("h-4 w-4", contactorStates.hvil ? "text-success" : "text-destructive")} />
            <span className="text-sm text-foreground">HVIL</span>
          </div>
          <span
            className={cn(
              "rounded px-2 py-0.5 text-xs font-medium",
              contactorStates.hvil ? "bg-success/20 text-success" : "bg-destructive/20 text-destructive",
            )}
          >
            {contactorStates.hvil ? "OK" : "FAULT"}
          </span>
        </div>
      </div>

      {/* Brake Status */}
      <div className="space-y-2">
        <h3 className="text-xs font-medium text-muted-foreground">BRAKE</h3>
        <button
          onClick={onToggleBrake}
          className="flex w-full items-center justify-between rounded-md bg-muted/50 p-2 transition-colors hover:bg-muted"
        >
          <div className="flex items-center gap-2">
            <div className={cn("h-3 w-3 rounded-full", brakeEngaged ? "bg-destructive" : "bg-muted-foreground")} />
            <span className="text-sm text-foreground">Brake Pedal</span>
          </div>
          <span
            className={cn(
              "rounded px-2 py-0.5 text-xs font-medium",
              brakeEngaged ? "bg-destructive/20 text-destructive" : "bg-muted text-muted-foreground",
            )}
          >
            {brakeEngaged ? "ENGAGED" : "RELEASED"}
          </span>
        </button>
      </div>
    </div>
  )
}
