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

#include "task_manager.h"
#include "handle_manager.h"
#include "handle_traits.h"
#include "discord_result.h"
#include "discord_handle_array.h"
#include "utils/discord_common.h"

class DiscordClient;

// Generic task callback - executes a lambda on main thread
template<typename Func>
void PushTask(Func&& func) {
	Tasks.Push(std::forward<Func>(func));
}

// Push a pre-created object as result (for cached objects)
// Usage: PushCachedResult<DiscordChannel>(client_handle, client, callback, data, channel);
template<typename T>
void PushCachedResult(Handle_t client_handle, DiscordClient* client, IPluginFunction* callback, cell_t data, T* obj) {
	using Traits = HandleTraits<T>;
	Tasks.Push([client_handle, client, callback, data, obj]() {
		DiscordResult* result = new DiscordResult(Traits::type, client);
		result->SetSuccess(true);

		if (obj) {
			Handle_t objHandle = Handles.CreateCallback(obj, HandleIdOf<T>::value);
			result->SetHandle(Traits::key, objHandle);
		}

		Handle_t resultHandle = Handles.CreateCallback(result, HandleId::DiscordResult);

		callback->PushCell(client_handle);
		callback->PushCell(resultHandle);
		callback->PushCell(data);
		callback->Execute(nullptr);

		Handles.FreeHandle(resultHandle);
	});
}

// Single object result with automatic type deduction
// Usage: PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
template<typename T, typename DppT = typename HandleTraits<T>::DppType>
void PushResult(Handle_t client_handle, DiscordClient* client, IPluginFunction* callback, cell_t data,
				const dpp::confirmation_callback_t& confirmation) {
	using Traits = HandleTraits<T, DppT>;
	Tasks.Push([client_handle, client, callback, data, confirmation]() {
		DiscordResult* result = new DiscordResult(Traits::type, client);

		if (confirmation.is_error()) {
			result->SetError(confirmation.get_error().human_readable);
		} else {
			result->SetSuccess(true);
			try {
				T* obj = new T(confirmation.get<DppT>(), client);
				Handle_t objHandle = Handles.CreateCallback(obj, HandleIdOf<T>::value);
				result->SetHandle(Traits::key, objHandle);
			} catch (...) {
				result->SetError("Failed to create object from response");
			}
		}

		Handle_t resultHandle = Handles.CreateCallback(result, HandleId::DiscordResult);

		callback->PushCell(client_handle);
		callback->PushCell(resultHandle);
		callback->PushCell(data);
		callback->Execute(nullptr);

		Handles.FreeHandle(resultHandle);
	});
}

// Simple confirmation callback (no object returned)
// Usage: PushConfirm(client_handle, client, callback, data, cb);
// Usage: PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::Delete);
inline void PushConfirm(Handle_t client_handle, DiscordClient* client, IPluginFunction* callback, cell_t data,
						const dpp::confirmation_callback_t& confirmation, DiscordResultType type = DiscordResultType::Confirmation) {
	Tasks.Push([client_handle, client, callback, data, type, confirmation]() {
		DiscordResult* result = new DiscordResult(type, client);

		if (confirmation.is_error()) {
			result->SetError(confirmation.get_error().human_readable);
		} else {
			result->SetSuccess(true);
		}

		Handle_t resultHandle = Handles.CreateCallback(result, HandleId::DiscordResult);

		callback->PushCell(client_handle);
		callback->PushCell(resultHandle);
		callback->PushCell(data);
		callback->Execute(nullptr);

		Handles.FreeHandle(resultHandle);
	});
}

// Error-only result callback
// Usage: PushError(client_handle, client, callback, data, "error message");
// Usage: PushError(client_handle, client, callback, data, "error message", DiscordResultType::Send);
inline void PushError(Handle_t client_handle, DiscordClient* client, IPluginFunction* callback, cell_t data,
					  const std::string& error, DiscordResultType type = DiscordResultType::Confirmation) {
	Tasks.Push([client_handle, client, callback, data, type, error]() {
		DiscordResult* result = new DiscordResult(type, client);
		result->SetError(error);

		Handle_t resultHandle = Handles.CreateCallback(result, HandleId::DiscordResult);

		callback->PushCell(client_handle);
		callback->PushCell(resultHandle);
		callback->PushCell(data);
		callback->Execute(nullptr);

		Handles.FreeHandle(resultHandle);
	});
}

// List result with automatic type deduction
// Usage: PushResultList<DiscordMessage, dpp::message_map>(client_handle, client, callback, data, cb, DiscordResultType::Messages);
template<typename T, typename MapType>
void PushResultList(Handle_t client_handle, DiscordClient* client, IPluginFunction* callback, cell_t data,
					const dpp::confirmation_callback_t& confirmation, DiscordResultType type) {
	Tasks.Push([client_handle, client, callback, data, type, confirmation]() {
		DiscordResult* result = new DiscordResult(type, client);

		if (confirmation.is_error()) {
			result->SetError(confirmation.get_error().human_readable);
		} else {
			result->SetSuccess(true);
			try {
				const auto& items = confirmation.get<MapType>();
				DiscordHandleArray* arr = new DiscordHandleArray();

				for (const auto& [id, item] : items) {
					T* obj = new T(item, client);
					Handle_t h = Handles.CreateCallback(obj, HandleIdOf<T>::value);
					if (h) arr->Add(h);
				}

				result->SetInt("count", static_cast<int>(arr->GetLength()));
				Handle_t arrHandle = Handles.CreateCallback(arr, HandleId::DiscordHandleArray);
				result->SetHandle("items", arrHandle);
			} catch (...) {
				result->SetError("Failed to create objects from response");
			}
		}

		Handle_t resultHandle = Handles.CreateCallback(result, HandleId::DiscordResult);

		callback->PushCell(client_handle);
		callback->PushCell(resultHandle);
		callback->PushCell(data);
		callback->Execute(nullptr);

		Handles.FreeHandle(resultHandle);
	});
}

// List result with custom extractor for complex types
// Usage: PushResultList<DiscordChannel, dpp::active_threads>(client_handle, client, callback, data, cb, DiscordResultType::Threads,
//        [](const auto& item) { return item.active_thread; });
template<typename T, typename MapType, typename Extractor>
void PushResultList(Handle_t client_handle, DiscordClient* client, IPluginFunction* callback, cell_t data,
				const dpp::confirmation_callback_t& confirmation, DiscordResultType type, Extractor&& extractor) {
	Tasks.Push([client_handle, client, callback, data, type, confirmation, extractor]() {
		DiscordResult* result = new DiscordResult(type, client);

		if (confirmation.is_error()) {
			result->SetError(confirmation.get_error().human_readable);
		} else {
			result->SetSuccess(true);
			try {
				const auto& items = confirmation.get<MapType>();
				DiscordHandleArray* arr = new DiscordHandleArray();

				for (const auto& [id, item] : items) {
					T* obj = new T(extractor(item), client);
					Handle_t h = Handles.CreateCallback(obj, HandleIdOf<T>::value);
					if (h) arr->Add(h);
				}

				result->SetInt("count", static_cast<int>(arr->GetLength()));
				Handle_t arrHandle = Handles.CreateCallback(arr, HandleId::DiscordHandleArray);
				result->SetHandle("items", arrHandle);
			} catch (...) {
				result->SetError("Failed to create objects from response");
			}
		}

		Handle_t resultHandle = Handles.CreateCallback(result, HandleId::DiscordResult);

		callback->PushCell(client_handle);
		callback->PushCell(resultHandle);
		callback->PushCell(data);
		callback->Execute(nullptr);

		Handles.FreeHandle(resultHandle);
	});
}
