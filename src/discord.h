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

// Core managers
#include "core/handle_manager.h"
#include "core/natives_manager.h"
#include "core/task_manager.h"
#include "core/callback_manager.h"
#include "core/async_callback.h"
#include "core/discord_event.h"
#include "core/event_registry.h"

// Common utilities and forward declarations
#include "utils/discord_common.h"
#include "utils/discord_embed.h"
#include "utils/discord_http.h"
#include "utils/discord_forum.h"
#include "utils/discord_component.h"
#include "utils/discord_modal.h"
#include "utils/discord_poll.h"

// Entity classes
#include "entities/discord_user.h"
#include "entities/discord_message.h"
#include "entities/discord_channel.h"
#include "entities/discord_guild.h"
#include "entities/discord_role.h"
#include "entities/discord_guild_member.h"
#include "entities/discord_emoji.h"
#include "entities/discord_sticker.h"
#include "entities/discord_scheduled_event.h"
#include "entities/discord_attachment.h"
#include "entities/discord_reaction.h"
#include "entities/discord_ban.h"
#include "entities/discord_thread_member.h"

// Feature classes
#include "features/discord_webhook.h"
#include "features/discord_interaction.h"
#include "features/discord_invite.h"
#include "features/discord_slashcommand.h"
#include "features/discord_button_interaction.h"
#include "features/discord_select_interaction.h"
#include "features/discord_modal_interaction.h"

// Operations
#include "core/operations/operations.h"

// Core client class
#include "core/discord_client.h"
