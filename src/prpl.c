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

#include "prpl.h"
#include "openvk.h"

#include <libpurple/accountopt.h>
#include <libpurple/debug.h>
#include <libpurple/notify.h>
#include <libpurple/status.h>
#include <libpurple/version.h>

static const char *
openvk_list_icon(PurpleAccount *account, PurpleBuddy *buddy)
{
	(void)account;
	(void)buddy;
	return "openvk";
}

static GList *
openvk_status_types(PurpleAccount *account)
{
	GList *types = NULL;
	PurpleStatusType *type;

	(void)account;

	type = purple_status_type_new(PURPLE_STATUS_AVAILABLE, NULL, NULL, TRUE);
	types = g_list_append(types, type);

	type = purple_status_type_new(PURPLE_STATUS_OFFLINE, NULL, NULL, TRUE);
	types = g_list_append(types, type);

	return types;
}

static void
openvk_tooltip_text(PurpleBuddy *buddy, PurpleNotifyUserInfo *user_info,
                    gboolean full)
{
	(void)buddy;
	(void)user_info;
	(void)full;
}

static GList *
openvk_chat_info(PurpleConnection *gc)
{
	GList *info = NULL;
	struct proto_chat_entry *entry;

	(void)gc;

	entry = g_new0(struct proto_chat_entry, 1);
	entry->label = "Chat ID or #chat<id>";
	entry->identifier = "chat_id";
	entry->required = TRUE;
	info = g_list_append(info, entry);

	entry = g_new0(struct proto_chat_entry, 1);
	entry->label = "Invite link (optional)";
	entry->identifier = "invite";
	entry->required = FALSE;
	info = g_list_append(info, entry);

	return info;
}

static GHashTable *
openvk_chat_info_defaults(PurpleConnection *gc, const char *chat_name)
{
	GHashTable *defaults;

	(void)gc;

	defaults = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);

	if (chat_name != NULL && *chat_name != '\0') {
		const char *value = chat_name;

		if (g_str_has_prefix(value, "#chat"))
			value += 5;
		g_hash_table_insert(defaults, "chat_id", g_strdup(value));
	}

	return defaults;
}

static void
openvk_join_chat(PurpleConnection *gc, GHashTable *components)
{
	const char *chat_id;

	(void)components;

	chat_id = g_hash_table_lookup(components, "chat_id");
	purple_debug_info(OPENVK_DEBUG_CATEGORY,
	                  "join chat requested: %s",
	                  chat_id != NULL ? chat_id : "(none)");
	purple_notify_error(gc, "OpenVK",
	                    "OpenVK chat support is not implemented yet.",
	                    "This is the M0 skeleton plugin.");
}

static char *
openvk_get_chat_name(GHashTable *components)
{
	const char *chat_id;

	chat_id = g_hash_table_lookup(components, "chat_id");
	if (chat_id == NULL || *chat_id == '\0')
		return g_strdup("#chat");
	if (g_str_has_prefix(chat_id, "#chat"))
		return g_strdup(chat_id);
	return g_strdup_printf("#chat%s", chat_id);
}

static void
openvk_chat_leave(PurpleConnection *gc, int id)
{
	(void)gc;
	(void)id;
}

static int
openvk_chat_send(PurpleConnection *gc, int id, const char *message,
                 PurpleMessageFlags flags)
{
	(void)gc;
	(void)id;
	(void)message;
	(void)flags;
	return -1;
}

static int
openvk_send_im(PurpleConnection *gc, const char *who, const char *message,
               PurpleMessageFlags flags)
{
	(void)who;
	(void)message;
	(void)flags;

	purple_notify_error(gc, "OpenVK",
	                    "OpenVK messaging is not implemented yet.",
	                    "This is the M0 skeleton plugin.");
	return -1;
}

static unsigned int
openvk_send_typing(PurpleConnection *gc, const char *name,
                   PurpleTypingState state)
{
	(void)gc;
	(void)name;
	(void)state;
	return 0;
}

static void
openvk_convo_closed(PurpleConnection *gc, const char *who)
{
	(void)gc;
	(void)who;
}

static const char *
openvk_normalize(const PurpleAccount *account, const char *who)
{
	(void)account;
	return (who != NULL && *who != '\0') ? who : NULL;
}

static char *
openvk_get_cb_alias(PurpleConnection *gc, int id, const char *who)
{
	(void)gc;
	(void)id;
	return g_strdup(who != NULL ? who : "");
}

static void
openvk_set_chat_topic(PurpleConnection *gc, int id, const char *topic)
{
	(void)gc;
	(void)id;
	(void)topic;
}

PurplePluginProtocolInfo openvk_prpl_info = {
	.options = OPT_PROTO_CHAT_TOPIC | OPT_PROTO_UNIQUE_CHATNAME,
	.user_splits = NULL,
	.protocol_options = NULL,
	.icon_spec = {
		.format = "png",
		.min_width = 0,
		.min_height = 0,
		.max_width = 128,
		.max_height = 128,
		.max_filesize = 100 * 1024,
		.scale_rules = PURPLE_ICON_SCALE_DISPLAY,
	},
	.list_icon = openvk_list_icon,
	.list_emblem = NULL,
	.status_text = NULL,
	.tooltip_text = openvk_tooltip_text,
	.status_types = openvk_status_types,
	.blist_node_menu = NULL,
	.chat_info = openvk_chat_info,
	.chat_info_defaults = openvk_chat_info_defaults,
	.login = openvk_login,
	.close = openvk_close,
	.send_im = openvk_send_im,
	.set_info = NULL,
	.send_typing = openvk_send_typing,
	.get_info = NULL,
	.set_status = NULL,
	.set_idle = NULL,
	.change_passwd = NULL,
	.add_buddy = NULL,
	.add_buddies = NULL,
	.remove_buddy = NULL,
	.remove_buddies = NULL,
	.add_permit = NULL,
	.add_deny = NULL,
	.rem_permit = NULL,
	.rem_deny = NULL,
	.set_permit_deny = NULL,
	.join_chat = openvk_join_chat,
	.reject_chat = NULL,
	.get_chat_name = openvk_get_chat_name,
	.chat_invite = NULL,
	.chat_leave = openvk_chat_leave,
	.chat_whisper = NULL,
	.chat_send = openvk_chat_send,
	.keepalive = NULL,
	.register_user = NULL,
	.get_cb_info = NULL,
	.get_cb_away = NULL,
	.alias_buddy = NULL,
	.group_buddy = NULL,
	.rename_group = NULL,
	.buddy_free = NULL,
	.convo_closed = openvk_convo_closed,
	.normalize = openvk_normalize,
	.set_buddy_icon = NULL,
	.remove_group = NULL,
	.get_cb_real_name = NULL,
	.set_chat_topic = openvk_set_chat_topic,
	.find_blist_chat = NULL,
	.roomlist_get_list = NULL,
	.roomlist_cancel = NULL,
	.roomlist_expand_category = NULL,
	.can_receive_file = NULL,
	.send_file = NULL,
	.new_xfer = NULL,
	.offline_message = NULL,
	.whiteboard_prpl_ops = NULL,
	.send_raw = NULL,
	.roomlist_room_serialize = NULL,
	.unregister_user = NULL,
	.send_attention = NULL,
	.get_attention_types = NULL,
	.struct_size = sizeof(PurplePluginProtocolInfo),
	.get_account_text_table = NULL,
	.initiate_media = NULL,
	.get_media_caps = NULL,
	.get_moods = NULL,
	.set_public_alias = NULL,
	.get_public_alias = NULL,
	.add_buddy_with_invite = NULL,
	.add_buddies_with_invite = NULL,
	.get_cb_alias = openvk_get_cb_alias,
	.chat_can_receive_file = NULL,
	.chat_send_file = NULL,
};

void
openvk_prpl_init(void)
{
	static gboolean initialized = FALSE;
	PurpleAccountOption *option;

	if (initialized)
		return;
	initialized = TRUE;

	option = purple_account_option_string_new(
	    "API base URL", "server", OPENVK_DEFAULT_API_BASE);
	openvk_prpl_info.protocol_options =
	    g_list_append(openvk_prpl_info.protocol_options, option);

	option = purple_account_option_list_new("Authentication", "auth_mode", NULL);
	purple_account_option_add_list_item(option, "Password", "password");
	purple_account_option_add_list_item(option, "Access token", "token");
	openvk_prpl_info.protocol_options =
	    g_list_append(openvk_prpl_info.protocol_options, option);

	option = purple_account_option_string_new(
	    "API version", "api_version", "5.131");
	openvk_prpl_info.protocol_options =
	    g_list_append(openvk_prpl_info.protocol_options, option);

	option = purple_account_option_int_new("History messages", "history_count", 50);
	openvk_prpl_info.protocol_options =
	    g_list_append(openvk_prpl_info.protocol_options, option);

	option = purple_account_option_bool_new(
	    "Download avatars", "download_avatars", TRUE);
	openvk_prpl_info.protocol_options =
	    g_list_append(openvk_prpl_info.protocol_options, option);

	option = purple_account_option_bool_new(
	    "Debug logging", "debug_log", FALSE);
	openvk_prpl_info.protocol_options =
	    g_list_append(openvk_prpl_info.protocol_options, option);
}
