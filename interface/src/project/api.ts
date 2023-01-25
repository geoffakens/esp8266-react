import { AxiosPromise } from "axios";

import { AXIOS } from "../api/endpoints";
import { PresenceMqttSettings, PresenceState, PresenceSettings } from "./types";

export function readPresenceState(): AxiosPromise<PresenceState> {
  return AXIOS.get('/presenceState');
}

export function readPresenceSettings(): AxiosPromise<PresenceSettings> {
  return AXIOS.get('/presenceSettings');
}

export function updatePresenceSettings(presenceSettings: PresenceSettings): AxiosPromise<PresenceSettings> {
  return AXIOS.post('/presenceSettings', presenceSettings);
}

export function readBrokerSettings(): AxiosPromise<PresenceMqttSettings> {
  return AXIOS.get('/brokerSettings');
}

export function updateBrokerSettings(lightMqttSettings: PresenceMqttSettings): AxiosPromise<PresenceMqttSettings> {
  return AXIOS.post('/brokerSettings', lightMqttSettings);
}
