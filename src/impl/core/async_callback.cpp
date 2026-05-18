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

#include "core/async_callback.h"
#include "core/discord_client.h"

AsyncCallbackTracker& AsyncCallbackTracker::Instance() {
	static AsyncCallbackTracker tracker;
	return tracker;
}

void AsyncCallbackTracker::Register() {
	if (m_registered) return;
	plsys->AddPluginsListener(this);
	m_registered = true;
}

void AsyncCallbackTracker::Unregister() {
	if (!m_registered) return;
	plsys->RemovePluginsListener(this);
	m_registered = false;
	m_unloadingSerials.clear();
}

void AsyncCallbackTracker::OnPluginWillUnload(SourceMod::IPlugin* plugin) {
	if (!plugin) return;

	m_unloadingSerials.insert(plugin->GetSerial());
	DiscordClient::ClearCallbacksForPlugin(plugin);
}

void AsyncCallbackTracker::OnPluginDestroyed(SourceMod::IPlugin* plugin) {
	if (!plugin) return;

	m_unloadingSerials.insert(plugin->GetSerial());
	DiscordClient::ClearCallbacksForPlugin(plugin);
}

IPluginFunction* AsyncCallbackTracker::Resolve(unsigned int pluginSerial, funcid_t functionId) const {
	if (!pluginSerial || m_unloadingSerials.find(pluginSerial) != m_unloadingSerials.end()) {
		return nullptr;
	}

	SourceMod::IPlugin* plugin = FindPlugin(pluginSerial);
	if (!plugin || plugin->GetStatus() != SourceMod::Plugin_Running) {
		return nullptr;
	}

	IPluginContext* context = plugin->GetBaseContext();
	if (!context) {
		return nullptr;
	}

	IPluginFunction* function = context->GetFunctionById(functionId);
	if (!function || !function->IsRunnable()) {
		return nullptr;
	}

	return function;
}

SourceMod::IPlugin* AsyncCallbackTracker::FindPlugin(unsigned int pluginSerial) const {
	SourceMod::IPluginIterator* iter = plsys->GetPluginIterator();
	if (!iter) return nullptr;

	SourceMod::IPlugin* found = nullptr;
	while (iter->MorePlugins()) {
		SourceMod::IPlugin* plugin = iter->GetPlugin();
		if (plugin && plugin->GetSerial() == pluginSerial) {
			found = plugin;
			break;
		}
		iter->NextPlugin();
	}

	iter->Release();
	return found;
}

AsyncCallback::AsyncCallback(Handle_t clientHandle, IPluginFunction* function, cell_t data) {
	Set(clientHandle, function, data);
}

void AsyncCallback::Set(Handle_t clientHandle, IPluginFunction* function, cell_t data) {
	*this = AsyncCallback();
	if (!function) return;

	IPluginContext* context = function->GetParentContext();
	if (!context) return;

	SourceMod::IPlugin* plugin = plsys->FindPluginByContext(context->GetContext());
	if (!plugin) return;

	m_clientHandle = clientHandle;
	m_data = data;
	m_pluginSerial = plugin->GetSerial();
	m_functionId = function->GetFunctionID();
}

bool AsyncCallback::IsValid() const {
	return m_pluginSerial != 0;
}

Handle_t AsyncCallback::GetClientHandle() const {
	return m_clientHandle;
}

cell_t AsyncCallback::GetData() const {
	return m_data;
}

bool AsyncCallback::IsForPlugin(unsigned int pluginSerial) const {
	return m_pluginSerial != 0 && m_pluginSerial == pluginSerial;
}

DiscordClient* AsyncCallback::GetClient() const {
	return Handles.Read<DiscordClient>(m_clientHandle);
}

IPluginFunction* AsyncCallback::GetFunction() const {
	return AsyncCallbackTracker::Instance().Resolve(m_pluginSerial, m_functionId);
}

bool AsyncCallback::Invoke(IPluginFunction* function, Handle_t resultHandle) const {
	if (!function) return false;

	function->PushCell(m_clientHandle);
	function->PushCell(resultHandle);
	function->PushCell(m_data);
	function->Execute(nullptr);
	return true;
}

void AsyncCallback::Confirm(const dpp::confirmation_callback_t& confirmation, DiscordResultType type) const {
	Dispatch([confirmation, type](const AsyncCallback& callback, DiscordClient* client, IPluginFunction* function) {
		auto result = std::make_unique<DiscordResult>(type, client);

		if (confirmation.is_error()) {
			result->SetError(confirmation.get_error().human_readable);
		} else {
			result->SetSuccess(true);
		}

		callback.CompleteResult(function, std::move(result));
	});
}

void AsyncCallback::Error(std::string error, DiscordResultType type) const {
	Dispatch([error = std::move(error), type](const AsyncCallback& callback, DiscordClient* client, IPluginFunction* function) {
		auto result = std::make_unique<DiscordResult>(type, client);
		result->SetError(error);
		callback.CompleteResult(function, std::move(result));
	});
}

void AsyncCallback::CompleteResult(IPluginFunction* function, std::unique_ptr<DiscordResult> result) const {
	if (!result) return;

	Handle_t resultHandle = Handles.CreateCallback(result.release(), HandleId::DiscordResult);
	if (!resultHandle) return;

	Invoke(function, resultHandle);
	Handles.FreeHandle(resultHandle);
}
