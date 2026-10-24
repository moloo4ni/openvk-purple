#ifndef OPENVK_H
#define OPENVK_H

#include <glib.h>

#include <libpurple/account.h>
#include <libpurple/connection.h>

#define OPENVK_PLUGIN_ID "prpl-openvk"
#define OPENVK_VERSION "0.1.0"
#define OPENVK_DEBUG_CATEGORY "openvk"
#define OPENVK_DEFAULT_API_BASE "https://api.openvk.org"
#define OPENVK_DEFAULT_AUTH_MODE "password"

typedef struct _OpenVkConnection OpenVkConnection;

struct _OpenVkConnection {
	PurpleConnection *gc;
	PurpleAccount *account;
	gboolean closing;
};

OpenVkConnection *openvk_connection_new(PurpleConnection *gc);
void openvk_connection_free(OpenVkConnection *conn);

const char *openvk_account_get_api_base(PurpleAccount *account);
const char *openvk_account_get_auth_mode(PurpleAccount *account);

void openvk_login(PurpleAccount *account);
void openvk_close(PurpleConnection *gc);

#endif /* OPENVK_H */
