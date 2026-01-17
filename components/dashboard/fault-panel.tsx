"use client"

import { AlertTriangle, AlertCircle, Info, Trash2, Plus } from "lucide-react"
import { cn } from "@/lib/utils"

interface Fault {
  code: string
  message: string
  severity: "critical" | "warning" | "info"
  timestamp: number
}

interface FaultPanelProps {
  faults: Fault[]
  onAddFault: () => void
  onClearFaults: () => void
}

export function FaultPanel({ faults, onAddFault, onClearFaults }: FaultPanelProps) {
  const getSeverityIcon = (severity: Fault["severity"]) => {
    switch (severity) {
      case "critical":
        return <AlertTriangle className="h-4 w-4 text-destructive" />
      case "warning":
        return <AlertCircle className="h-4 w-4 text-warning" />
      case "info":
        return <Info className="h-4 w-4 text-primary" />
    }
  }

  const getSeverityClass = (severity: Fault["severity"]) => {
    switch (severity) {
      case "critical":
        return "border-destructive/50 bg-destructive/10"
      case "warning":
        return "border-warning/50 bg-warning/10"
      case "info":
        return "border-primary/50 bg-primary/10"
    }
  }

  return (
    <div className="flex h-full flex-col rounded-lg bg-card p-3">
      <div className="mb-2 flex items-center justify-between">
        <h2 className="text-xs font-medium text-muted-foreground">FAULTS</h2>
        <div className="flex gap-0.5">
          <button
            onClick={onAddFault}
            className="rounded p-1 text-muted-foreground hover:bg-muted hover:text-foreground"
            title="Simulate fault"
          >
            <Plus className="h-3 w-3" />
          </button>
          <button
            onClick={onClearFaults}
            className="rounded p-1 text-muted-foreground hover:bg-muted hover:text-foreground"
            title="Clear faults"
          >
            <Trash2 className="h-3 w-3" />
          </button>
        </div>
      </div>

      <div className="flex-1 space-y-1.5 overflow-auto">
        {faults.length === 0 ? (
          <div className="flex h-20 flex-col items-center justify-center text-center">
            <div className="mb-1 rounded-full bg-success/20 p-2">
              <Info className="h-3 w-3 text-success" />
            </div>
            <p className="text-xs text-muted-foreground">No faults</p>
          </div>
        ) : (
          faults.map((fault, index) => (
            <div
              key={`${fault.code}-${fault.timestamp}`}
              className={cn(
                "rounded border p-2 transition-all animate-in slide-in-from-right-5",
                getSeverityClass(fault.severity),
              )}
            >
              <div className="flex items-start gap-1.5">
                {getSeverityIcon(fault.severity)}
                <div className="flex-1 min-w-0">
                  <div className="flex items-center justify-between gap-1">
                    <span className="font-mono text-[10px] font-semibold text-foreground">{fault.code}</span>
                    <span className="text-[10px] text-muted-foreground">
                      {new Date(fault.timestamp).toLocaleTimeString([], { hour: "2-digit", minute: "2-digit" })}
                    </span>
                  </div>
                  <p className="mt-0.5 text-xs text-foreground truncate">{fault.message}</p>
                </div>
              </div>
            </div>
          ))
        )}
      </div>

      {faults.length > 0 && (
        <div className="mt-2 flex items-center justify-between rounded bg-muted/50 px-2 py-1">
          <span className="text-[10px] text-muted-foreground">Active</span>
          <span
            className={cn(
              "font-mono text-xs font-bold",
              faults.some((f) => f.severity === "critical") ? "text-destructive" : "text-warning",
            )}
          >
            {faults.length}
          </span>
        </div>
      )}
    </div>
  )
}
