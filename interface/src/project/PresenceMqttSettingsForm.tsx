import { FC, useState } from "react";
import { ValidateFieldsError } from "async-validator";

import { Button } from "@mui/material";
import SaveIcon from '@mui/icons-material/Save';

import { ButtonRow, FormLoader, SectionContent, ValidatedTextField } from "../components";
import { validate } from "../validators";
import { useRest, updateValue } from "../utils";

import * as API from './api';
import { PresenceMqttSettings } from "./types";
import { PRESENCE_MQTT_SETTINGS_VALIDATOR } from "./validators";

const PresenceMqttSettingsForm: FC = () => {
  const [fieldErrors, setFieldErrors] = useState<ValidateFieldsError>();
  const {
    loadData, saveData, saving, setData, data, errorMessage
  } = useRest<PresenceMqttSettings>({ read: API.readBrokerSettings, update: API.updateBrokerSettings });

  const updateFormValue = updateValue(setData);

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    const validateAndSubmit = async () => {
      try {
        setFieldErrors(undefined);
        await validate(PRESENCE_MQTT_SETTINGS_VALIDATOR, data);
        saveData();
      } catch (errors: any) {
        setFieldErrors(errors);
      }
    };

    return (
      <>
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="uniqueId"
          label="Unique ID"
          fullWidth
          variant="outlined"
          value={data.uniqueId}
          onChange={updateFormValue}
          margin="normal"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="name"
          label="Name"
          fullWidth
          variant="outlined"
          value={data.name}
          onChange={updateFormValue}
          margin="normal"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="mqttPath"
          label="MQTT Path"
          fullWidth
          variant="outlined"
          value={data.mqttPath}
          onChange={updateFormValue}
          margin="normal"
        />
        <ButtonRow mt={1}>
          <Button startIcon={<SaveIcon />} disabled={saving} variant="contained" color="primary" type="submit" onClick={validateAndSubmit}>
            Save
          </Button>
        </ButtonRow>
      </>
    );
  };

  return (
    <SectionContent title='MQTT Settings' titleGutter>
      {content()}
    </SectionContent>
  );
};

export default PresenceMqttSettingsForm;
