export interface VehicleState {
  speed: number
  gear: "P" | "R" | "N" | "D"
  mainBattery: {
    soc: number
    voltage: number
    current: number
  }
  suppBattery: {
    soc: number
    voltage: number
  }
  cruise: {
    enabled: boolean
    setSpeed: number
  }
  brakeEngaged: boolean
  contactorStates: {
    main: boolean
    precharge: boolean
    hvil: boolean
  }
  heartbeat: number
  faults: Array<{
    code: string
    message: string
    severity: "critical" | "warning" | "info"
    timestamp: number
  }>
  turnSignal: "left" | "right" | null
}
