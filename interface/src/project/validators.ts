import Schema from "async-validator";

export const PRESENCE_MQTT_SETTINGS_VALIDATOR = new Schema({
    uniqueId: {
        required: true, message: "Please provide an id"
    },
    name: {
        required: true, message: "Please provide a name"
    },
    mqttPath: {
        required: true, message: "Please provide an MQTT path"
    }
});

export const SENSOR_SETTINGS_VALIDATOR = new Schema({
    detectionDelay: [
        { required: true, message: "Detection delay is required" },
        { type: "number", min: 0, max: 1638, message: "Detection delay must be between 0 and 1638" }
    ],
    clearDelay: [
        { required: true, message: "Clear delay is required" },
        { type: "number", min: 0, max: 1638, message: "Clear delay must be between 0 and 1638" }
    ],
  });
