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

#include "task_manager.h"
#include "handle_manager.h"
#include "handle_traits.h"
#include "discord_result.h"
#include "discord_handle_array.h"
#include "utils/discord_common.h"
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

class DiscordClient;

class AsyncCallbackTracker : public SourceMod::IPluginsListener {
public:
	static AsyncCallbackTracker& Instance();

	void Register();
	void Unregister();
	void OnPluginWillUnload(SourceMod::IPlugin* plugin) override;
	void OnPluginDestroyed(SourceMod::IPlugin* plugin) override;
	IPluginFunction* Resolve(unsigned int pluginSerial, funcid_t functionId) const;

private:
	SourceMod::IPlugin* FindPlugin(unsigned int pluginSerial) const;

	bool m_registered = false;
	std::unordered_set<unsigned int> m_unloadingSerials;
};

class AsyncCallback {
public:
	AsyncCallback() = default;

	AsyncCallback(Handle_t clientHandle, IPluginFunction* function, cell_t data = 0);

	void Set(Handle_t clientHandle, IPluginFunction* function, cell_t data = 0);

	bool IsValid() const;

	explicit operator bool() const {
		return IsValid();
	}

	Handle_t GetClientHandle() const;

	cell_t GetData() const;

	bool IsForPlugin(unsigned int pluginSerial) const;

	DiscordClient* GetClient() const;

	IPluginFunction* GetFunction() const;

	template<typename Func>
	void Dispatch(Func&& func) const {
		if (!IsValid()) return;

		AsyncCallback callback = *this;
		Tasks.Push([callback, func = std::forward<Func>(func)]() mutable {
			DiscordClient* client = callback.GetClient();
			IPluginFunction* function = callback.GetFunction();
			if (!client || !function) return;

			func(callback, client, function);
		});
	}

	bool Invoke(IPluginFunction* function, Handle_t resultHandle) const;

	template<typename T>
	void CachedResult(T* obj) const {
		using Traits = HandleTraits<T>;
		std::unique_ptr<T> object(obj);

		Dispatch([object = std::move(object)](const AsyncCallback& callback, DiscordClient* client, IPluginFunction* function) mutable {
			auto result = std::make_unique<DiscordResult>(Traits::type, client);
			result->SetSuccess(true);

				if (object) {
					Handle_t objHandle = Handles.CreateCallback(object.release(), HandleIdOf<T>::value);
					if (objHandle) {
						result->SetHandle(Traits::key, objHandle);
					} else {
						result->SetError("Failed to create result handle");
					}
				}

			callback.CompleteResult(function, std::move(result));
		});
	}

	template<typename T, typename DppT = typename HandleTraits<T>::DppType>
	void Result(const dpp::confirmation_callback_t& confirmation) const {
		using Traits = HandleTraits<T, DppT>;

		Dispatch([confirmation](const AsyncCallback& callback, DiscordClient* client, IPluginFunction* function) {
			auto result = std::make_unique<DiscordResult>(Traits::type, client);

			if (confirmation.is_error()) {
				result->SetError(confirmation.get_error().human_readable);
			} else {
				result->SetSuccess(true);
				try {
						auto obj = std::make_unique<T>(confirmation.get<DppT>(), client);
						Handle_t objHandle = Handles.CreateCallback(obj.release(), HandleIdOf<T>::value);
						if (objHandle) {
							result->SetHandle(Traits::key, objHandle);
						} else {
							result->SetError("Failed to create result handle");
						}
				} catch (...) {
					result->SetError("Failed to create object from response");
				}
			}

			callback.CompleteResult(function, std::move(result));
		});
	}

	void Confirm(const dpp::confirmation_callback_t& confirmation, DiscordResultType type = DiscordResultType::Confirmation) const;
	void Error(std::string error, DiscordResultType type = DiscordResultType::Confirmation) const;

	template<typename Builder>
	void ResultCustom(DiscordResultType type, Builder&& builder) const {
		Dispatch([type, builder = std::forward<Builder>(builder)](const AsyncCallback& callback, DiscordClient* client, IPluginFunction* function) mutable {
			auto result = std::make_unique<DiscordResult>(type, client);
			builder(*result, client);
			callback.CompleteResult(function, std::move(result));
		});
	}

	template<typename T, typename MapType>
	void ResultList(const dpp::confirmation_callback_t& confirmation, DiscordResultType type) const {
		ResultList<T, MapType>(confirmation, type, [](const auto& item) -> decltype(auto) { return item; });
	}

	template<typename T, typename MapType, typename Extractor>
	void ResultList(const dpp::confirmation_callback_t& confirmation, DiscordResultType type, Extractor&& extractor) const {
		Dispatch([confirmation, type, extractor = std::forward<Extractor>(extractor)](const AsyncCallback& callback, DiscordClient* client, IPluginFunction* function) mutable {
			auto result = std::make_unique<DiscordResult>(type, client);

			if (confirmation.is_error()) {
				result->SetError(confirmation.get_error().human_readable);
			} else {
				result->SetSuccess(true);
				try {
					const auto& items = confirmation.get<MapType>();
					auto arr = std::make_unique<DiscordHandleArray>();

					for (const auto& [id, item] : items) {
						auto obj = std::make_unique<T>(extractor(item), client);
						Handle_t h = Handles.CreateCallback(obj.release(), HandleIdOf<T>::value);
						if (h) arr->Add(h);
					}

					result->SetInt("count", static_cast<int>(arr->GetLength()));
					Handle_t arrHandle = Handles.CreateCallback(arr.release(), HandleId::DiscordHandleArray);
					if (arrHandle) {
						result->SetHandle("items", arrHandle);
					} else {
						result->SetError("Failed to create result array handle");
					}
				} catch (...) {
					result->SetError("Failed to create objects from response");
				}
			}

			callback.CompleteResult(function, std::move(result));
		});
	}

private:
	void CompleteResult(IPluginFunction* function, std::unique_ptr<DiscordResult> result) const;

	Handle_t m_clientHandle{BAD_HANDLE};
	cell_t m_data{0};
	unsigned int m_pluginSerial{0};
	funcid_t m_functionId{0};
};
