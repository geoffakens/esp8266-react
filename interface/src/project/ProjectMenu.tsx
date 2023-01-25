import { FC, useMemo } from 'react';

import { List, SxProps } from '@mui/material';
import { SensorOccupied } from '@mui/icons-material';

import { PROJECT_PATH } from '../api/env';
import { WEB_SOCKET_ROOT } from '../api/endpoints';
import LayoutMenuItem from '../components/layout/LayoutMenuItem';
import { PresenceState } from './types';
import { useWs } from '../utils';

const PRESENCE_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "presenceState";

const ProjectMenu: FC = () => {
  const { connected, data } = useWs<PresenceState>(PRESENCE_SETTINGS_WEBSOCKET_URL);

  const sx = useMemo<SxProps>(() => {
    if (!connected || !data || !data.presenceDetected) {
      return {} as SxProps;
    }

    return {
      '& .MuiSvgIcon-root': {
        color: '#6eb300',
      },
    } as SxProps;
  }, [connected, data]);

  return (
    <List>
      <LayoutMenuItem sx={sx} icon={SensorOccupied} label="Presence" to={`/${PROJECT_PATH}/presence`} />
    </List>
  );
};

export default ProjectMenu;
