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

#include <libpurple/debug.h>

OpenVkConnection *
openvk_connection_new(PurpleConnection *gc)
{
	OpenVkConnection *conn;

	g_return_val_if_fail(gc != NULL, NULL);

	conn = g_new0(OpenVkConnection, 1);
	conn->gc = gc;
	conn->account = purple_connection_get_account(gc);
	conn->closing = FALSE;

	gc->proto_data = conn;
	return conn;
}

void
openvk_connection_free(OpenVkConnection *conn)
{
	if (conn == NULL)
		return;

	if (conn->gc != NULL && conn->gc->proto_data == conn)
		conn->gc->proto_data = NULL;

	g_free(conn);
}

const char *
openvk_account_get_api_base(PurpleAccount *account)
{
	const char *value;

	value = purple_account_get_string(account, "server",
	                                  OPENVK_DEFAULT_API_BASE);
	return (value != NULL && *value != '\0') ? value : OPENVK_DEFAULT_API_BASE;
}

const char *
openvk_account_get_auth_mode(PurpleAccount *account)
{
	const char *value;

	value = purple_account_get_string(account, "auth_mode",
	                                  OPENVK_DEFAULT_AUTH_MODE);
	return (value != NULL && *value != '\0') ? value : OPENVK_DEFAULT_AUTH_MODE;
}

void
openvk_login(PurpleAccount *account)
{
	PurpleConnection *gc;
	OpenVkConnection *conn;

	g_return_if_fail(account != NULL);

	gc = purple_account_get_connection(account);
	g_return_if_fail(gc != NULL);

	conn = openvk_connection_new(gc);
	g_return_if_fail(conn != NULL);
	purple_connection_set_state(gc, PURPLE_CONNECTING);
	purple_connection_set_display_name(gc, purple_account_get_username(account));

	purple_debug_info(OPENVK_DEBUG_CATEGORY,
	                  "login requested for %s (api base %s, auth mode %s)",
	                  purple_account_get_username(account),
	                  openvk_account_get_api_base(account),
	                  openvk_account_get_auth_mode(account));

	/*
	 * M0 is intentionally a loadable skeleton.  The real login state machine
	 * arrives with the HTTP/API layer in M1.
	 */
	purple_connection_error_reason(
	    gc,
	    PURPLE_CONNECTION_ERROR_OTHER_ERROR,
	    "OpenVK login is not implemented yet. This is the M0 skeleton plugin.");
}

void
openvk_close(PurpleConnection *gc)
{
	OpenVkConnection *conn;

	g_return_if_fail(gc != NULL);

	conn = gc->proto_data;
	if (conn != NULL) {
		conn->closing = TRUE;
		openvk_connection_free(conn);
	}

	purple_debug_info(OPENVK_DEBUG_CATEGORY, "connection closed");
}
