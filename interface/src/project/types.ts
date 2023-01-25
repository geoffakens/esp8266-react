export interface PresenceState {
  presenceDetected: boolean;
}

export interface PresenceMqttSettings {
  uniqueId: string;
  name: string;
  mqttPath: string;
}

export interface PresenceSettings {
  minRange: number;
  maxRange: number;
  detectionDelay: number;
  clearDelay: number;
}
