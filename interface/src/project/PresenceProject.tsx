
import React, { FC } from 'react';
import { Navigate, Route, Routes } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RouterTabs, useRouterTab, useLayoutTitle } from '../components';

import PresenceSettingsForm from './PresenceSettingsForm';
import PresenceMqttSettingsForm from './PresenceMqttSettingsForm';

const PresenceProject: FC = () => {
  useLayoutTitle("Presence");
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        <Tab value="sensor" label="Sensor Settings" />
        <Tab value="mqtt" label="MQTT Settings" />
      </RouterTabs>
      <Routes>
        <Route path="sensor" element={<PresenceSettingsForm />} />
        <Route path="mqtt" element={<PresenceMqttSettingsForm />} />
        <Route path="/*" element={<Navigate replace to="sensor" />} />
      </Routes>
    </>
  );
};

export default PresenceProject;
