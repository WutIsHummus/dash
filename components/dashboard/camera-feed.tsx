"use client"

import { Video, VideoOff, Maximize2 } from "lucide-react"
import { cn } from "@/lib/utils"

interface CameraFeedProps {
  label: string
  type: "rear" | "left" | "right" | "side"
  isActive: boolean
}

export function CameraFeed({ label, type, isActive }: CameraFeedProps) {
  return (
    <div className="overflow-hidden rounded-lg bg-card">
      <div className="flex items-center justify-between border-b border-border px-3 py-2">
        <div className="flex items-center gap-2">
          {isActive ? (
            <Video className="h-4 w-4 text-success" />
          ) : (
            <VideoOff className="h-4 w-4 text-muted-foreground" />
          )}
          <span className="text-sm font-medium text-foreground">{label}</span>
        </div>
        <button className="rounded p-1 text-muted-foreground hover:bg-muted hover:text-foreground">
          <Maximize2 className="h-4 w-4" />
        </button>
      </div>

      <div className={cn("relative aspect-[16/10] bg-muted", !isActive && "opacity-50")}>
        {/* Camera feed placeholder */}
        <img
          src={`/.jpg?key=doovy&height=400&width=640&query=${type} view camera feed car`}
          alt={`${label} camera feed`}
          className="h-full w-full object-cover"
        />

        {/* Overlay indicators */}
        <div className="absolute inset-0 pointer-events-none">
          {/* Recording indicator */}
          {isActive && (
            <div className="absolute left-3 top-3 flex items-center gap-1.5 rounded bg-background/80 px-2 py-1">
              <div className="h-2 w-2 animate-pulse rounded-full bg-destructive" />
              <span className="text-xs font-medium text-foreground">LIVE</span>
            </div>
          )}

          {/* Guidelines for rear camera */}
          {type === "rear" && isActive && (
            <svg className="absolute inset-0 h-full w-full" viewBox="0 0 640 360">
              {/* Trajectory lines */}
              <path d="M 280 360 Q 300 250 320 180" fill="none" stroke="rgba(34, 197, 94, 0.6)" strokeWidth="2" />
              <path d="M 360 360 Q 340 250 320 180" fill="none" stroke="rgba(34, 197, 94, 0.6)" strokeWidth="2" />
              {/* Distance markers */}
              <line x1="250" y1="320" x2="390" y2="320" stroke="rgba(250, 204, 21, 0.6)" strokeWidth="2" />
              <line x1="270" y1="280" x2="370" y2="280" stroke="rgba(250, 204, 21, 0.6)" strokeWidth="2" />
              <line x1="290" y1="240" x2="350" y2="240" stroke="rgba(239, 68, 68, 0.6)" strokeWidth="2" />
            </svg>
          )}
        </div>

        {!isActive && (
          <div className="absolute inset-0 flex items-center justify-center bg-background/50">
            <div className="text-center">
              <VideoOff className="mx-auto h-8 w-8 text-muted-foreground" />
              <p className="mt-2 text-sm text-muted-foreground">Camera inactive</p>
            </div>
          </div>
        )}
      </div>
    </div>
  )
}
