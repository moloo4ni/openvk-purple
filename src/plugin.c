/*
 * OpenVK protocol plugin for Pidgin/libpurple.
 *
 * Copyright (C) 2026 moloo4ni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

#include "openvk.h"
#include "prpl.h"

#include <libpurple/debug.h>
#include <libpurple/plugin.h>
#include <libpurple/version.h>

static void
openvk_init(PurplePlugin *plugin)
{
	(void)plugin;

	openvk_prpl_init();
	purple_debug_info(OPENVK_DEBUG_CATEGORY,
	                  "plugin initialized (version %s)", OPENVK_VERSION);
}

static void
openvk_destroy(PurplePlugin *plugin)
{
	(void)plugin;
	purple_debug_info(OPENVK_DEBUG_CATEGORY, "plugin destroyed");
}

static PurplePluginInfo info = {
	.magic = PURPLE_PLUGIN_MAGIC,
	.major_version = PURPLE_MAJOR_VERSION,
	.minor_version = PURPLE_MINOR_VERSION,
	.type = PURPLE_PLUGIN_PROTOCOL,
	.ui_requirement = NULL,
	.flags = 0,
	.dependencies = NULL,
	.priority = PURPLE_PRIORITY_DEFAULT,
	.id = OPENVK_PLUGIN_ID,
	.name = "OpenVK",
	.version = OPENVK_VERSION,
	.summary = "OpenVK protocol plugin",
	.description = "Native OpenVK messaging support for Pidgin 2 / libpurple 2.",
	.author = "moloo4ni",
	.homepage = "https://github.com/moloo4ni/openvk-purple",
	.load = NULL,
	.unload = NULL,
	.destroy = openvk_destroy,
	.ui_info = NULL,
	.extra_info = &openvk_prpl_info,
	.prefs_info = NULL,
	.actions = NULL,
};

PURPLE_INIT_PLUGIN(openvk, openvk_init, info);
