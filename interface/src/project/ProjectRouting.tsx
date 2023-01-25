import { FC } from 'react';
import { Navigate, Routes, Route } from 'react-router-dom';

import PresenceProject from './PresenceProject';

const ProjectRouting: FC = () => {
  return (
    <Routes>
      {
        // Add the default route for your project below
      }
      <Route path="/*" element={<Navigate to="presence/sensor" />} />
      {
        // Add your project page routes below.
      }
      <Route path="presence/*" element={<PresenceProject />} />
    </Routes>
  );
};

export default ProjectRouting;
