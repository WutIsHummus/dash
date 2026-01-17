"use client"

import { ChevronLeft, ChevronRight } from "lucide-react"
import { cn } from "@/lib/utils"

interface TurnIndicatorProps {
  direction: "left" | "right"
  active: boolean
  onClick: () => void
}

export function TurnIndicator({ direction, active, onClick }: TurnIndicatorProps) {
  const Icon = direction === "left" ? ChevronLeft : ChevronRight

  return (
    <button
      onClick={onClick}
      className={cn(
        "flex h-10 w-10 items-center justify-center rounded-lg transition-all",
        active ? "bg-accent text-accent-foreground animate-pulse" : "bg-card text-muted-foreground hover:bg-muted",
      )}
    >
      <Icon className="h-6 w-6" />
    </button>
  )
}
