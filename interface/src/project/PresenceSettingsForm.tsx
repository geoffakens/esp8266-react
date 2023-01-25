import { FC, useState } from 'react';
import { ValidateFieldsError } from 'async-validator';

import { Box, Button, Slider, Typography } from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';

import { SectionContent, FormLoader, ButtonRow, ValidatedTextField } from '../components';
import { validate } from '../validators';
import { SENSOR_SETTINGS_VALIDATOR } from './validators';
import { numberValue, updateValue, useRest } from '../utils';

import * as API from './api';
import { PresenceSettings } from './types';

const distanceMarks = [
  {
    value: 0,
    label: '0',
  },
  {
    value: 1,
  },
  {
    value: 2,
  },
  {
    value: 3,
  },
  {
    value: 4.5,
    label: '4.5',
  },
  {
    value: 6,
  },
  {
    value: 7,
  },
  {
    value: 8,
  },
  {
    value: 9,
    label: '9',
  },
];

const PresenceSettingsForm: FC = () => {
  const [fieldErrors, setFieldErrors] = useState<ValidateFieldsError>();
  const {
    loadData, saveData, saving, setData, data, errorMessage
  } = useRest<PresenceSettings>({ read: API.readPresenceSettings, update: API.updatePresenceSettings });

  const updateFormValue = updateValue(setData);

  const updateDetectionRange = (maxRange: number) => {
    setData((previousData) => ({
      ...previousData,
      maxRange,
    } as PresenceSettings));
  };

  const validateAndSubmit = async () => {
    try {
      setFieldErrors(undefined);
      await validate(SENSOR_SETTINGS_VALIDATOR, data as PresenceSettings);
      saveData();
    } catch (errors: any) {
      setFieldErrors(errors);
    }
  };

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    return (
      <>
        <Box sx={{ width: '100%', paddingLeft: 1, paddingRight: 1}} >
          <Box sx={{ paddingTop: 2 }}>
            <Typography sx={{ color: 'rgba(0,0,0,0.6)', fontSize: '0.75rem' }} gutterBottom >Detection Range (meters)</Typography>
            <Slider
              sx={{ flexGrow: 1, '& .MuiSlider-markLabel': { fontSize: '0.75rem' } }}
              aria-label="Detection Range"
              min={0}
              max={9}
              step={0.5}
              marks={distanceMarks}
              value={data.maxRange}
              valueLabelDisplay="auto"
              onChange={(e, v) => { updateDetectionRange(v as number); }}
            />
          </Box>
        </Box>
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="detectionDelay"
          label="Detection Delay (seconds)"
          fullWidth
          variant="outlined"
          value={numberValue(data.detectionDelay)}
          type="number"
          onChange={updateFormValue}
          margin="normal"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="clearDelay"
          label="Clear Delay (seconds)"
          fullWidth
          variant="outlined"
          value={numberValue(data.clearDelay)}
          type="number"
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
    <>
      <SectionContent title='Sensor Settings' titleGutter>
        {content()}
      </SectionContent>
    </>
  );
};

export default PresenceSettingsForm;
