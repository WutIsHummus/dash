"use client"

interface SpeedGaugeProps {
  speed: number
  maxSpeed: number
}

export function SpeedGauge({ speed, maxSpeed }: SpeedGaugeProps) {
  const percentage = (speed / maxSpeed) * 100
  const strokeDasharray = 440
  const strokeDashoffset = strokeDasharray - (strokeDasharray * Math.min(percentage, 100)) / 100

  return (
    <div className="relative flex flex-col items-center">
      <svg className="h-48 w-48 -rotate-90" viewBox="0 0 160 160">
        {/* Background arc */}
        <circle
          cx="80"
          cy="80"
          r="70"
          fill="none"
          stroke="currentColor"
          strokeWidth="12"
          strokeLinecap="round"
          className="text-muted"
          strokeDasharray="330"
          strokeDashoffset="110"
        />
        {/* Progress arc */}
        <circle
          cx="80"
          cy="80"
          r="70"
          fill="none"
          stroke="currentColor"
          strokeWidth="12"
          strokeLinecap="round"
          className={speed > 160 ? "text-destructive" : speed > 100 ? "text-warning" : "text-primary"}
          strokeDasharray="330"
          strokeDashoffset={330 - (330 * Math.min(percentage, 100)) / 100 + 110}
          style={{ transition: "stroke-dashoffset 0.1s ease-out" }}
        />
      </svg>
      <div className="absolute inset-0 flex flex-col items-center justify-center">
        <span className="text-5xl font-bold tabular-nums text-foreground">{speed}</span>
        <span className="text-sm text-muted-foreground">km/h</span>
      </div>
    </div>
  )
}
