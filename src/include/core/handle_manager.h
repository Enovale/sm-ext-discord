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

#include "smsdk_ext.h"

enum class HandleId {
	Discord,
	DiscordUser,
	DiscordMessage,
	DiscordChannel,
	DiscordGuild,
	DiscordRole,
	DiscordWebhook,
	DiscordEmbed,
	DiscordInteraction,
	DiscordAutocompleteInteraction,
	DiscordSlashCommand,
	DiscordForumTag,
	DiscordInvite,
	HttpHeaders,
	HttpCompletion,
	DiscordComponent,
	DiscordButtonInteraction,
	DiscordSelectInteraction,
	DiscordModalInteraction,
	DiscordModal,
	DiscordPoll,
	DiscordEvent,
	DiscordResult,
	DiscordHandleArray,
	DiscordGuildMember,
	DiscordEmoji,
	DiscordSticker,
	DiscordScheduledEvent,
	DiscordAttachment,
	DiscordReaction,
	DiscordBan,
	DiscordThreadMember,
	Count
};

// Forward declarations
class DiscordClient;
class DiscordUser;
class DiscordMessage;
class DiscordChannel;
class DiscordGuild;
class DiscordRole;
class DiscordWebhook;
class DiscordEmbed;
class DiscordInteraction;
class DiscordAutocompleteInteraction;
class DiscordSlashCommand;
class DiscordForumTag;
class DiscordInvite;
class HttpHeaders;
class HttpCompletion;
class DiscordComponent;
class DiscordButtonInteraction;
class DiscordSelectInteraction;
class DiscordModalInteraction;
class DiscordModal;
class DiscordPoll;
class DiscordEvent;
class DiscordResult;
class DiscordHandleArray;
class DiscordGuildMember;
class DiscordEmoji;
class DiscordSticker;
class DiscordScheduledEvent;
class DiscordAttachment;
class DiscordReaction;
class DiscordBan;
class DiscordThreadMember;

// Type to HandleId mapping
template<typename T> struct HandleIdOf;
#define HANDLE_TYPE(Type, Id) template<> struct HandleIdOf<Type> { static constexpr HandleId value = HandleId::Id; }
HANDLE_TYPE(DiscordClient, Discord);
HANDLE_TYPE(DiscordUser, DiscordUser);
HANDLE_TYPE(DiscordMessage, DiscordMessage);
HANDLE_TYPE(DiscordChannel, DiscordChannel);
HANDLE_TYPE(DiscordGuild, DiscordGuild);
HANDLE_TYPE(DiscordRole, DiscordRole);
HANDLE_TYPE(DiscordWebhook, DiscordWebhook);
HANDLE_TYPE(DiscordEmbed, DiscordEmbed);
HANDLE_TYPE(DiscordInteraction, DiscordInteraction);
HANDLE_TYPE(DiscordAutocompleteInteraction, DiscordAutocompleteInteraction);
HANDLE_TYPE(DiscordSlashCommand, DiscordSlashCommand);
HANDLE_TYPE(DiscordForumTag, DiscordForumTag);
HANDLE_TYPE(DiscordInvite, DiscordInvite);
HANDLE_TYPE(HttpHeaders, HttpHeaders);
HANDLE_TYPE(HttpCompletion, HttpCompletion);
HANDLE_TYPE(DiscordComponent, DiscordComponent);
HANDLE_TYPE(DiscordButtonInteraction, DiscordButtonInteraction);
HANDLE_TYPE(DiscordSelectInteraction, DiscordSelectInteraction);
HANDLE_TYPE(DiscordModalInteraction, DiscordModalInteraction);
HANDLE_TYPE(DiscordModal, DiscordModal);
HANDLE_TYPE(DiscordPoll, DiscordPoll);
HANDLE_TYPE(DiscordEvent, DiscordEvent);
HANDLE_TYPE(DiscordResult, DiscordResult);
HANDLE_TYPE(DiscordHandleArray, DiscordHandleArray);
HANDLE_TYPE(DiscordGuildMember, DiscordGuildMember);
HANDLE_TYPE(DiscordEmoji, DiscordEmoji);
HANDLE_TYPE(DiscordSticker, DiscordSticker);
HANDLE_TYPE(DiscordScheduledEvent, DiscordScheduledEvent);
HANDLE_TYPE(DiscordAttachment, DiscordAttachment);
HANDLE_TYPE(DiscordReaction, DiscordReaction);
HANDLE_TYPE(DiscordBan, DiscordBan);
HANDLE_TYPE(DiscordThreadMember, DiscordThreadMember);
#undef HANDLE_TYPE

class HandleManager {
public:
	static HandleManager& Instance();

	bool Initialize(char* error = nullptr, size_t maxlen = 0);
	void Shutdown();

	HandleType_t Get(HandleId id) const;

	template<typename T>
	Handle_t Create(IPluginContext* ctx, T* obj, HandleId id) {
		HandleError err;
		HandleSecurity sec(ctx->GetIdentity(), myself->GetIdentity());
		Handle_t h = handlesys->CreateHandleEx(Get(id), obj, &sec, nullptr, &err);
		if (!h) {
			delete obj;
			ctx->ReportError("Could not create %s handle (error %d)", GetName(id), err);
		}
		return h;
	}

	template<typename T>
	Handle_t CreateCallback(T* obj, HandleId id) {
		HandleError err;
		HandleSecurity sec(myself->GetIdentity(), myself->GetIdentity());
		Handle_t h = handlesys->CreateHandleEx(Get(id), obj, &sec, nullptr, &err);
		if (!h) {
			delete obj;
		}
		return h;
	}

	template<typename T>
	Handle_t CreateCached(T* obj, HandleId id) {
		HandleError err;
		HandleSecurity sec(nullptr, myself->GetIdentity());
		Handle_t h = handlesys->CreateHandleEx(Get(id), obj, &sec, nullptr, &err);
		if (!h) {
			delete obj;
		}
		return h;
	}

	template<typename T>
	T* GetPointer(IPluginContext* ctx, Handle_t handle, HandleId id) {
		HandleError err;
		HandleSecurity sec(ctx->GetIdentity(), myself->GetIdentity());
		T* obj;
		if ((err = handlesys->ReadHandle(handle, Get(id), &sec, (void**)&obj)) != HandleError_None) {
			ctx->ReportError("Invalid %s handle %x (error %d)", GetName(id), handle, err);
			return nullptr;
		}
		return obj;
	}

	// Auto-deduce HandleId version
	template<typename T>
	T* GetPointer(IPluginContext* ctx, Handle_t handle) {
		return GetPointer<T>(ctx, handle, HandleIdOf<T>::value);
	}

	template<typename T>
	T* Read(Handle_t handle, HandleId id) {
		if (!handle) return nullptr;

		HandleSecurity sec(nullptr, myself->GetIdentity());
		T* obj = nullptr;
		if (handlesys->ReadHandle(handle, Get(id), &sec, reinterpret_cast<void**>(&obj)) != HandleError_None) {
			return nullptr;
		}
		return obj;
	}

	template<typename T>
	T* Read(Handle_t handle) {
		return Read<T>(handle, HandleIdOf<T>::value);
	}

	void FreeHandle(Handle_t handle);
	void MaybeFreeHandle(Handle_t handle, bool shouldFree);

	const char* GetName(HandleId id) const;

private:
	HandleManager() = default;

	class Destructor : public IHandleTypeDispatch {
	public:
		void (*destroy)(void*) = nullptr;
		void OnHandleDestroy(HandleType_t type, void* object) override {
			if (destroy) destroy(object);
		}
	};

	HandleType_t m_handleTypes[static_cast<size_t>(HandleId::Count)] = {};
	Destructor m_destructors[static_cast<size_t>(HandleId::Count)];
};

inline HandleManager& Handles = HandleManager::Instance();

// RAII wrapper for cached handles - automatically frees on destruction
template<typename T>
class CachedHandle {
	mutable Handle_t m_handle = BAD_HANDLE;

public:
	CachedHandle() = default;
	~CachedHandle() {
		if (m_handle) handlesys->FreeHandle(m_handle, nullptr);
	}

	CachedHandle(const CachedHandle&) = delete;
	CachedHandle& operator=(const CachedHandle&) = delete;

	CachedHandle(CachedHandle&& other) noexcept : m_handle(other.m_handle) {
		other.m_handle = BAD_HANDLE;
	}
	CachedHandle& operator=(CachedHandle&& other) noexcept {
		if (this != &other) {
			if (m_handle) handlesys->FreeHandle(m_handle, nullptr);
			m_handle = other.m_handle;
			other.m_handle = BAD_HANDLE;
		}
		return *this;
	}

	// Get or create handle lazily
	template<typename Factory>
	Handle_t GetOrCreate(Factory&& factory) const {
		if (m_handle) return m_handle;
		T* obj = factory();
		if (!obj) return BAD_HANDLE;
		m_handle = Handles.CreateCached(obj, HandleIdOf<T>::value);
		return m_handle;
	}

	Handle_t Get() const { return m_handle; }
	operator Handle_t() const { return m_handle; }
	explicit operator bool() const { return m_handle != BAD_HANDLE; }
};
