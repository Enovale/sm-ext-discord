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

#include "natives/entity_natives_common.h"
#include "entities/discord_scheduled_event.h"

static cell_t event_GetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetChannelId().c_str());
	return 1;
}

static cell_t event_GetCreatorId(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetCreatorId().c_str());
	return 1;
}

static cell_t event_SetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);
	event->SetName(name);
	return 1;
}

static cell_t event_SetDescription(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	char* desc;
	pContext->LocalToString(params[2], &desc);
	event->SetDescription(desc);
	return 1;
}

static cell_t event_SetScheduledStartTime(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	event->SetStartTime(static_cast<time_t>(params[2]));
	return 1;
}

static cell_t event_SetScheduledEndTime(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	event->SetEndTime(static_cast<time_t>(params[2]));
	return 1;
}

static cell_t event_GetEntityId(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetEntityId().c_str());
	return 1;
}

static cell_t event_GetLocation(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetLocation());
	return 1;
}

static cell_t event_SetLocation(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	char* location;
	pContext->LocalToString(params[2], &location);
	event->SetLocation(location);
	return 1;
}

static cell_t event_GetCreatorUserId(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetCreatorUserId().c_str());
	return 1;
}

static cell_t event_GetCreatorUserName(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetCreatorUserName());
	return 1;
}

static cell_t event_SetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	char* channel_id;
	pContext->LocalToString(params[2], &channel_id);
	event->SetChannelId(channel_id);
	return 1;
}

static cell_t event_Delete(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	event->Delete(callback, data);
	return 1;
}

static cell_t event_Edit(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	event->Edit(callback, data);
	return 1;
}

static cell_t event_Start(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	event->Start(callback, data);
	return 1;
}

static cell_t event_End(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	event->End(callback, data);
	return 1;
}

static cell_t event_Cancel(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	event->Cancel(callback, data);
	return 1;
}

extern const sp_nativeinfo_t scheduled_event_natives[] = {
	{"DiscordScheduledEvent.GetId", EntityGetId<DiscordScheduledEvent>},
	{"DiscordScheduledEvent.GetGuildId", EntityGetGuildId<DiscordScheduledEvent>},
	{"DiscordScheduledEvent.GetChannelId", event_GetChannelId},
	{"DiscordScheduledEvent.GetCreatorId", event_GetCreatorId},
	{"DiscordScheduledEvent.GetName", EntityGetName<DiscordScheduledEvent>},
	{"DiscordScheduledEvent.SetName", event_SetName},
	{"DiscordScheduledEvent.GetDescription", EntityGetDescription<DiscordScheduledEvent>},
	{"DiscordScheduledEvent.SetDescription", event_SetDescription},
	{"DiscordScheduledEvent.ScheduledStartTime.get", EntityGetInt<DiscordScheduledEvent, time_t, &DiscordScheduledEvent::GetScheduledStartTime>},
	{"DiscordScheduledEvent.ScheduledStartTime.set", event_SetScheduledStartTime},
	{"DiscordScheduledEvent.ScheduledEndTime.get", EntityGetInt<DiscordScheduledEvent, time_t, &DiscordScheduledEvent::GetScheduledEndTime>},
	{"DiscordScheduledEvent.ScheduledEndTime.set", event_SetScheduledEndTime},
	{"DiscordScheduledEvent.PrivacyLevel.get", EntityGetInt<DiscordScheduledEvent, uint8_t, &DiscordScheduledEvent::GetPrivacyLevel>},
	{"DiscordScheduledEvent.Status.get", EntityGetInt<DiscordScheduledEvent, uint8_t, &DiscordScheduledEvent::GetStatus>},
	{"DiscordScheduledEvent.EntityType.get", EntityGetInt<DiscordScheduledEvent, uint8_t, &DiscordScheduledEvent::GetEntityType>},
	{"DiscordScheduledEvent.GetEntityId", event_GetEntityId},
	{"DiscordScheduledEvent.GetLocation", event_GetLocation},
	{"DiscordScheduledEvent.SetLocation", event_SetLocation},
	{"DiscordScheduledEvent.UserCount.get", EntityGetInt<DiscordScheduledEvent, uint32_t, &DiscordScheduledEvent::GetUserCount>},
	{"DiscordScheduledEvent.GetCreatorUserId", event_GetCreatorUserId},
	{"DiscordScheduledEvent.GetCreatorUserName", event_GetCreatorUserName},
	{"DiscordScheduledEvent.SetChannelId", event_SetChannelId},
	{"DiscordScheduledEvent.Delete", event_Delete},
	{"DiscordScheduledEvent.Edit", event_Edit},
	{"DiscordScheduledEvent.Start", event_Start},
	{"DiscordScheduledEvent.End", event_End},
	{"DiscordScheduledEvent.Cancel", event_Cancel},
	{nullptr, nullptr}
};
