/**
 * =============================================================================
 * SourceMod Discord Extension
 * Copyright 2024-2025 ProjectSky
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

#include "core/handle_manager.h"
#include "discord.h"

template<typename T>
static void TypedDestroy(void* ptr) { delete static_cast<T*>(ptr); }

struct HandleMeta {
	const char* name;
	void (*destroy)(void*);
};

static const HandleMeta s_meta[] = {
	{"Discord",                         TypedDestroy<DiscordClient>},
	{"DiscordUser",                     TypedDestroy<DiscordUser>},
	{"DiscordMessage",                  TypedDestroy<DiscordMessage>},
	{"DiscordChannel",                  TypedDestroy<DiscordChannel>},
	{"DiscordGuild",                    TypedDestroy<DiscordGuild>},
	{"DiscordRole",                     TypedDestroy<DiscordRole>},
	{"DiscordWebhook",                  TypedDestroy<DiscordWebhook>},
	{"DiscordEmbed",                    TypedDestroy<DiscordEmbed>},
	{"DiscordInteraction",              TypedDestroy<DiscordInteraction>},
	{"DiscordAutocompleteInteraction",  TypedDestroy<DiscordAutocompleteInteraction>},
	{"DiscordSlashCommand",             TypedDestroy<DiscordSlashCommand>},
	{"DiscordForumTag",                 TypedDestroy<DiscordForumTag>},
	{"DiscordInvite",                   TypedDestroy<DiscordInvite>},
	{"HttpHeaders",                     TypedDestroy<HttpHeaders>},
	{"HttpCompletion",                  TypedDestroy<HttpCompletion>},
	{"DiscordComponent",                TypedDestroy<DiscordComponent>},
	{"DiscordButtonInteraction",        TypedDestroy<DiscordButtonInteraction>},
	{"DiscordSelectInteraction",        TypedDestroy<DiscordSelectInteraction>},
	{"DiscordModalInteraction",         TypedDestroy<DiscordModalInteraction>},
	{"DiscordModal",                    TypedDestroy<DiscordModal>},
	{"DiscordPoll",                     TypedDestroy<DiscordPoll>},
	{"DiscordEvent",                    TypedDestroy<DiscordEvent>},
	{"DiscordResult",                   TypedDestroy<DiscordResult>},
	{"DiscordHandleArray",              TypedDestroy<DiscordHandleArray>},
	{"DiscordGuildMember",              TypedDestroy<DiscordGuildMember>},
	{"DiscordEmoji",                    TypedDestroy<DiscordEmoji>},
	{"DiscordSticker",                  TypedDestroy<DiscordSticker>},
	{"DiscordScheduledEvent",           TypedDestroy<DiscordScheduledEvent>},
	{"DiscordAttachment",               TypedDestroy<DiscordAttachment>},
	{"DiscordReaction",                 TypedDestroy<DiscordReaction>},
	{"DiscordBan",                      TypedDestroy<DiscordBan>},
	{"DiscordThreadMember",             TypedDestroy<DiscordThreadMember>},
};

static_assert(
	sizeof(s_meta) / sizeof(s_meta[0]) == static_cast<size_t>(HandleId::Count),
	"s_meta array size must match HandleId::Count"
);

HandleManager& HandleManager::Instance() {
	static HandleManager instance;
	return instance;
}

HandleType_t HandleManager::Get(HandleId id) const {
	return m_handleTypes[static_cast<size_t>(id)];
}

void HandleManager::FreeHandle(Handle_t handle) {
	HandleSecurity sec(myself->GetIdentity(), myself->GetIdentity());
	handlesys->FreeHandle(handle, &sec);
}

void HandleManager::MaybeFreeHandle(Handle_t handle, bool shouldFree) {
	if (shouldFree) FreeHandle(handle);
}

const char* HandleManager::GetName(HandleId id) const {
	return s_meta[static_cast<size_t>(id)].name;
}

bool HandleManager::Initialize(char* error, size_t maxlen) {
	HandleAccess access;
	handlesys->InitAccessDefaults(nullptr, &access);
	access.access[HandleAccess_Delete] = 0;

	for (size_t i = 0; i < static_cast<size_t>(HandleId::Count); ++i) {
		m_destructors[i].destroy = s_meta[i].destroy;
		m_handleTypes[i] = handlesys->CreateType(
			s_meta[i].name,
			&m_destructors[i],
			0, nullptr, &access,
			myself->GetIdentity(), nullptr
		);
		if (!m_handleTypes[i]) {
			if (error && maxlen > 0) {
				snprintf(error, maxlen, "Failed to register handle type: %s", s_meta[i].name);
			}
			return false;
		}
	}
	return true;
}

void HandleManager::Shutdown() {
	for (size_t i = 0; i < static_cast<size_t>(HandleId::Count); ++i) {
		if (m_handleTypes[i]) {
			handlesys->RemoveType(m_handleTypes[i], myself->GetIdentity());
			m_handleTypes[i] = 0;
		}
	}
}
