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

#pragma once

#include "smsdk_ext.h"
#include <string>
#include <unordered_map>
#include <variant>

class DiscordClient;

// Result type enum for type-safe result identification
enum class DiscordResultType {
	// Single objects
	Message,
	Channel,
	Guild,
	User,
	Role,
	Webhook,
	Invite,
	GuildMember,
	Emoji,
	Sticker,
	ScheduledEvent,
	Poll,
	Attachment,
	Reaction,
	Ban,
	ThreadMember,
	SlashCommand,

	// Lists
	Messages,
	Channels,
	Users,
	Roles,
	Webhooks,
	Invites,
	Members,
	Emojis,
	Stickers,
	ScheduledEvents,
	Bans,
	Threads,
	Voters,
	ThreadMembers,
	SlashCommands,

	// Operations (no object returned)
	Confirmation,
	Delete,
	Pin,
	Unpin,
	Kick,
	BanOp,
	Unban,
	AddRole,
	RemoveRole,
	Timeout,
	Edit,
	Create,
	Send,
	Http,
	VoiceMove,
	VoiceDisconnect,
	VoiceMute,
	VoiceDeaf,
	ThreadMemberAdd,
	ThreadMemberRemove,
	CommandRegister,
	CommandUpdate,
	CommandDelete,
	CommandPermission,
	CommandBulkDelete
};

// Result value types (same as EventValue)
using ResultValue = std::variant<int, float, bool, std::string, Handle_t>;

class DiscordResult {
private:
	DiscordResultType m_type;
	std::unordered_map<std::string, ResultValue> m_data;
	DiscordClient* m_client;
	bool m_autoFreeHandles;
	bool m_success;
	std::string m_error;

public:
	DiscordResult(DiscordResultType type, DiscordClient* client, bool autoFree = true);
	~DiscordResult();

	// Status
	bool IsSuccess() const { return m_success; }
	const char* GetError() const { return m_error.c_str(); }
	DiscordResultType GetType() const { return m_type; }
	int GetTypeInt() const { return static_cast<int>(m_type); }
	void SetSuccess(bool success) { m_success = success; }
	void SetError(const std::string& error) { m_error = error; m_success = false; }

	// Setters
	void SetInt(const char* key, int value) { m_data[key] = value; }
	void SetFloat(const char* key, float value) { m_data[key] = value; }
	void SetBool(const char* key, bool value) { m_data[key] = value; }
	void SetString(const char* key, const std::string& value) { m_data[key] = value; }
	void SetHandle(const char* key, Handle_t value) { m_data[key] = value; }

	// Getters
	int GetInt(const char* key, int defaultValue = 0) const;
	float GetFloat(const char* key, float defaultValue = 0.0f) const;
	bool GetBool(const char* key, bool defaultValue = false) const;
	const char* GetString(const char* key) const;
	Handle_t GetHandle(const char* key) const;
	bool HasKey(const char* key) const;

	DiscordClient* GetClient() const { return m_client; }
	void SetAutoFreeHandles(bool autoFree) { m_autoFreeHandles = autoFree; }
	bool GetAutoFreeHandles() const { return m_autoFreeHandles; }
};
