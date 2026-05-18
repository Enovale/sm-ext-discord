/**
 * =============================================================================
 * SourceMod Discord Extension
 * Copyright 2024-2026 ProjectSky
 * =============================================================================
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define SMEXT_CONF_NAME			     "SourceMod Discord Extension"
#define SMEXT_CONF_DESCRIPTION	 "Provide Discord Native"
#define SMEXT_CONF_VERSION		   "3.0.0"
#define SMEXT_CONF_VERSION_FILE	 3,0,0,0
#define SMEXT_CONF_AUTHOR		     "ProjectSky"
#define SMEXT_CONF_URL			     "https://github.com/ProjectSky/sm-ext-discord"
#define SMEXT_CONF_LOGTAG		     "discord"
#define SMEXT_CONF_LICENSE		   "GPL"
#define SMEXT_CONF_DATESTRING	   __DATE__

#define SMEXT_LINK(name) SDKExtension *g_pExtensionIface = name;

#define SMEXT_ENABLE_HANDLESYS
#define SMEXT_ENABLE_FORWARDSYS
#define SMEXT_ENABLE_PLUGINSYS
