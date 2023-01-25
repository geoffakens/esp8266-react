import React, { FC } from "react";
import { Link, useLocation } from "react-router-dom";

import { ListItem, ListItemButton, ListItemIcon, ListItemText, SvgIconProps, SxProps } from "@mui/material";

import { routeMatches } from "../../utils";

interface LayoutMenuItemProps {
  icon: React.ComponentType<SvgIconProps>;
  label: string;
  to: string;
  disabled?: boolean;
  sx?: SxProps;
}

const LayoutMenuItem: FC<LayoutMenuItemProps> = ({ icon: Icon, label, to, disabled, sx }) => {
  const { pathname } = useLocation();

  return (
    <ListItem disablePadding selected={routeMatches(to, pathname)} sx={sx}>
      <ListItemButton component={Link} to={to} disabled={disabled}>
        <ListItemIcon>
          <Icon />
        </ListItemIcon>
        <ListItemText>{label}</ListItemText>
      </ListItemButton>
    </ListItem>
  );
};

export default LayoutMenuItem;
