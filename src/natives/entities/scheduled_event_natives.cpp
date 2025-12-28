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

#include "natives/natives_common.h"
#include "entities/discord_scheduled_event.h"

static cell_t event_GetId(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetId().c_str());
	return 1;
}

static cell_t event_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetGuildId().c_str());
	return 1;
}

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

static cell_t event_GetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetName());
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

static cell_t event_GetDescription(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetDescription());
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

static cell_t event_GetScheduledStartTime(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	return static_cast<cell_t>(event->GetScheduledStartTime());
}

static cell_t event_SetScheduledStartTime(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	event->SetStartTime(static_cast<time_t>(params[2]));
	return 1;
}

static cell_t event_GetScheduledEndTime(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	return static_cast<cell_t>(event->GetScheduledEndTime());
}

static cell_t event_SetScheduledEndTime(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	event->SetEndTime(static_cast<time_t>(params[2]));
	return 1;
}

static cell_t event_GetPrivacyLevel(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	return static_cast<cell_t>(event->GetPrivacyLevel());
}

static cell_t event_GetStatus(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	return static_cast<cell_t>(event->GetStatus());
}

static cell_t event_GetEntityType(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	return static_cast<cell_t>(event->GetEntityType());
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

static cell_t event_GetUserCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	return static_cast<cell_t>(event->GetUserCount());
}

static cell_t event_GetCreatorUserId(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetCreatorUserId().c_str());
	return 1;
}

static cell_t event_GetCreatorUsername(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetCreatorUsername());
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

	event->Delete();
	return 1;
}

static cell_t event_Edit(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	event->Edit();
	return 1;
}

static cell_t event_Start(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	event->Start();
	return 1;
}

static cell_t event_End(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	event->End();
	return 1;
}

static cell_t event_Cancel(IPluginContext* pContext, const cell_t* params)
{
	DiscordScheduledEvent* event = Handles.GetPointer<DiscordScheduledEvent>(pContext, params[1]);
	if (!event) return 0;

	event->Cancel();
	return 1;
}

extern const sp_nativeinfo_t scheduled_event_natives[] = {
	{"DiscordScheduledEvent.GetId", event_GetId},
	{"DiscordScheduledEvent.GetGuildId", event_GetGuildId},
	{"DiscordScheduledEvent.GetChannelId", event_GetChannelId},
	{"DiscordScheduledEvent.GetCreatorId", event_GetCreatorId},
	{"DiscordScheduledEvent.GetName", event_GetName},
	{"DiscordScheduledEvent.SetName", event_SetName},
	{"DiscordScheduledEvent.GetDescription", event_GetDescription},
	{"DiscordScheduledEvent.SetDescription", event_SetDescription},
	{"DiscordScheduledEvent.ScheduledStartTime.get", event_GetScheduledStartTime},
	{"DiscordScheduledEvent.ScheduledStartTime.set", event_SetScheduledStartTime},
	{"DiscordScheduledEvent.ScheduledEndTime.get", event_GetScheduledEndTime},
	{"DiscordScheduledEvent.ScheduledEndTime.set", event_SetScheduledEndTime},
	{"DiscordScheduledEvent.PrivacyLevel.get", event_GetPrivacyLevel},
	{"DiscordScheduledEvent.Status.get", event_GetStatus},
	{"DiscordScheduledEvent.EntityType.get", event_GetEntityType},
	{"DiscordScheduledEvent.GetEntityId", event_GetEntityId},
	{"DiscordScheduledEvent.GetLocation", event_GetLocation},
	{"DiscordScheduledEvent.SetLocation", event_SetLocation},
	{"DiscordScheduledEvent.UserCount.get", event_GetUserCount},
	{"DiscordScheduledEvent.GetCreatorUserId", event_GetCreatorUserId},
	{"DiscordScheduledEvent.GetCreatorUsername", event_GetCreatorUsername},
	{"DiscordScheduledEvent.SetChannelId", event_SetChannelId},
	{"DiscordScheduledEvent.Delete", event_Delete},
	{"DiscordScheduledEvent.Edit", event_Edit},
	{"DiscordScheduledEvent.Start", event_Start},
	{"DiscordScheduledEvent.End", event_End},
	{"DiscordScheduledEvent.Cancel", event_Cancel},
	{nullptr, nullptr}
};
