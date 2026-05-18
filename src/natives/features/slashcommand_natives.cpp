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

#include "natives/natives_common.h"
#include "features/discord_slashcommand.h"

static cell_t slashcommand_CreateSlashCommand(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	DiscordSlashCommand* command = new DiscordSlashCommand(discord);
	Handle_t handle = Handles.Create(pContext, command, HandleId::DiscordSlashCommand);
	if (!handle) return 0;
	return handle;
}

static cell_t slashcommand_FromGlobalCommand(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* commandId;
	pContext->LocalToString(params[2], &commandId);

	DiscordSlashCommand* command = new DiscordSlashCommand(discord);

	dpp::snowflake cmd_id;
	if (!ParseSnowflake(pContext, commandId, cmd_id)) {
		delete command;
		return 0;
	}
	command->SetCommandId(cmd_id);

	Handle_t handle = Handles.Create(pContext, command, HandleId::DiscordSlashCommand);
	if (!handle) return 0;
	return handle;
}

static cell_t slashcommand_FromGuildCommand(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* commandId;
	pContext->LocalToString(params[2], &commandId);
	char* guildId;
	pContext->LocalToString(params[3], &guildId);

	DiscordSlashCommand* command = new DiscordSlashCommand(discord);

	dpp::snowflake cmd_id, guild_id;
	if (!ParseSnowflake(pContext, commandId, cmd_id) || !ParseSnowflake(pContext, guildId, guild_id)) {
		delete command;
		return 0;
	}
	command->SetCommandId(cmd_id);
	command->SetGuildId(guild_id);

	Handle_t handle = Handles.Create(pContext, command, HandleId::DiscordSlashCommand);
	if (!handle) return 0;
	return handle;
}

static cell_t slashcommand_SetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);
	command->SetName(name);
	return 1;
}

static cell_t slashcommand_SetDescription(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* description;
	pContext->LocalToString(params[2], &description);
	command->SetDescription(description);
	return 1;
}

static cell_t slashcommand_SetDefaultPermissions(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* permissions;
	pContext->LocalToString(params[2], &permissions);
	command->SetDefaultPermissions(permissions);
	return 1;
}

static cell_t slashcommand_GetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	pContext->StringToLocal(params[2], params[3], command->GetName());
	return 1;
}

static cell_t slashcommand_GetDescription(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	pContext->StringToLocal(params[2], params[3], command->GetDescription());
	return 1;
}

static cell_t slashcommand_GetDefaultPermissions(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	std::string permissions = command->GetDefaultPermissions();
	pContext->StringToLocal(params[2], params[3], permissions.c_str());
	return 1;
}

static cell_t slashcommand_AddOption(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	char* description;
	pContext->LocalToString(params[3], &description);

	dpp::command_option_type type = static_cast<dpp::command_option_type>(params[4]);
	bool required = params[5];
	bool autocomplete = params[6];
	if (autocomplete && type != dpp::co_string && type != dpp::co_integer && type != dpp::co_number) {
		pContext->ReportError("Autocomplete is only valid for string, integer, or number options");
		return 0;
	}

	command->AddOption(name, description, type, required, autocomplete);
	return 1;
}

static cell_t slashcommand_AddStringOptionWithChoices(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	char* description;
	pContext->LocalToString(params[3], &description);

	bool required = params[4];

	command->AddStringOptionWithChoices(name, description, required);
	return 1;
}

static cell_t slashcommand_AddIntOptionWithChoices(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	char* description;
	pContext->LocalToString(params[3], &description);

	bool required = params[4];

	command->AddIntOptionWithChoices(name, description, required);
	return 1;
}

static cell_t slashcommand_AddFloatOptionWithChoices(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	char* description;
	pContext->LocalToString(params[3], &description);

	bool required = params[4];

	command->AddFloatOptionWithChoices(name, description, required);
	return 1;
}

static cell_t slashcommand_AddStringChoice(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* choice_name;
	pContext->LocalToString(params[2], &choice_name);

	char* choice_value;
	pContext->LocalToString(params[3], &choice_value);

	if (!command->AddStringChoice(choice_name, choice_value)) {
		pContext->ReportError("AddStringChoice requires the last option to be created with AddStringOptionWithChoices()");
		return 0;
	}
	return 1;
}

static cell_t slashcommand_AddIntChoice(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* choice_name;
	pContext->LocalToString(params[2], &choice_name);

	int64_t choice_value = static_cast<int64_t>(params[3]);
	if (!IsInt32Range(choice_value)) {
		pContext->ReportError("Command integer choice value exceeds 32-bit range. Use AddInt64Choice() instead.");
		return 0;
	}

	if (!command->AddIntChoice(choice_name, choice_value)) {
		pContext->ReportError("AddIntChoice requires the last option to be created with AddIntOptionWithChoices()");
		return 0;
	}
	return 1;
}

static cell_t slashcommand_AddInt64Choice(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* choice_name;
	pContext->LocalToString(params[2], &choice_name);

	char* choice_value_str;
	pContext->LocalToString(params[3], &choice_value_str);

	int64_t choice_value;
	if (!ParseInt64(choice_value_str, choice_value)) {
		const char* display_value = "(null)";
		if (choice_value_str) {
			display_value = choice_value_str;
		}
		pContext->ReportError("Invalid command integer choice value: %s", display_value);
		return 0;
	}

	if (!command->AddIntChoice(choice_name, choice_value)) {
		pContext->ReportError("AddInt64Choice requires the last option to be created with AddIntOptionWithChoices()");
		return 0;
	}
	return 1;
}

static cell_t slashcommand_AddFloatChoice(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* choice_name;
	pContext->LocalToString(params[2], &choice_name);

	double choice_value = sp_ctof(params[3]);

	if (!command->AddFloatChoice(choice_name, choice_value)) {
		pContext->ReportError("AddFloatChoice requires the last option to be created with AddFloatOptionWithChoices()");
		return 0;
	}
	return 1;
}

static cell_t slashcommand_RegisterToGuild(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* guildId;
	pContext->LocalToString(params[2], &guildId);

	dpp::snowflake guild;
	if (!ParseSnowflake(pContext, guildId, guild)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	command->RegisterToGuild(guild, callback, data);
	return 1;
}

static cell_t slashcommand_RegisterGlobally(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	command->RegisterGlobally(callback, data);
	return 1;
}

static cell_t slashcommand_Update(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* guildId;
	pContext->LocalToString(params[2], &guildId);

	dpp::snowflake guild_id = 0;
	if (guildId[0] != '\0') {
		if (!ParseSnowflake(pContext, guildId, guild_id)) return 0;
	}

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	return command->Update(guild_id, callback, data);
}

static cell_t slashcommand_Delete(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* guildId;
	pContext->LocalToString(params[2], &guildId);

	dpp::snowflake guild_id = 0;
	if (guildId[0] != '\0') {
		if (!ParseSnowflake(pContext, guildId, guild_id)) return 0;
	}

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	command->Delete(guild_id, callback, data);
	return 1;
}

static cell_t slashcommand_AddPermissionOverride(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* targetId;
	pContext->LocalToString(params[2], &targetId);

	dpp::command_permission_type type = static_cast<dpp::command_permission_type>(params[3]);
	bool permission = params[4] != 0;

	dpp::snowflake target;
	if (!ParseSnowflake(pContext, targetId, target)) return 0;
	command->AddPermissionOverride(target, type, permission);
	return 1;
}

static cell_t slashcommand_RemovePermissionOverride(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* targetId;
	pContext->LocalToString(params[2], &targetId);

	dpp::command_permission_type type = static_cast<dpp::command_permission_type>(params[3]);

	dpp::snowflake target;
	if (!ParseSnowflake(pContext, targetId, target)) return 0;
	command->RemovePermissionOverride(target, type);
	return 1;
}

static cell_t slashcommand_ClearPermissionOverrides(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	command->ClearPermissionOverrides();
	return 1;
}

static cell_t slashcommand_GetPermissionOverrideCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	return static_cast<cell_t>(command->GetPermissionOverrideCount());
}

static cell_t slashcommand_GetPermissionOverride(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	int count = static_cast<int>(command->GetPermissionOverrideCount());
	if (count <= 0) {
		pContext->ReportError("No permission overrides are available");
		return 0;
	}
	int index;
	if (!GetNativeIntInRange(pContext, params[2], 0, count - 1, "Permission override index", index)) return 0;

	dpp::snowflake target_id;
	dpp::command_permission_type type;
	bool permission;

	if (!command->GetPermissionOverride(static_cast<size_t>(index), target_id, type, permission)) {
		return 0;
	}

	char targetStr[32];
	FormatInt64(static_cast<int64_t>(target_id), targetStr, sizeof(targetStr));
	pContext->StringToLocal(params[3], params[4], targetStr);

	cell_t* typePtr;
	if (!GetNativeArray(pContext, params[5], &typePtr, "permission type output")) return 0;
	*typePtr = static_cast<cell_t>(type);

	cell_t* permPtr;
	if (!GetNativeArray(pContext, params[6], &permPtr, "permission value output")) return 0;
	*permPtr = permission;

	return 1;
}

static cell_t slashcommand_ApplyPermissionOverrides(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* guildId;
	pContext->LocalToString(params[2], &guildId);

	dpp::snowflake guild_id;
	if (!ParseSnowflake(pContext, guildId, guild_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	return command->ApplyPermissionOverrides(guild_id, callback, data);
}

static cell_t slashcommand_SetContextMenuType(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	dpp::slashcommand_contextmenu_type contextType = static_cast<dpp::slashcommand_contextmenu_type>(params[2]);
	command->SetContextMenuType(contextType);
	return 1;
}

static cell_t slashcommand_GetContextMenuType(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	return static_cast<cell_t>(command->GetContextMenuType());
}

static cell_t slashcommand_SetNSFW(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	bool nsfw = params[2] != 0;
	command->SetNSFW(nsfw);
	return 1;
}

static cell_t slashcommand_GetNSFW(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	return command->GetNSFW();
}

static cell_t slashcommand_SetDMPermission(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	bool dm_permission = params[2] != 0;
	command->SetDMPermission(dm_permission);
	return 1;
}

static cell_t slashcommand_GetDMPermission(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	return command->GetDMPermission();
}

static cell_t slashcommand_AddLocalization(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	char* language;
	char* name;
	char* description = nullptr;

	pContext->LocalToString(params[2], &language);
	pContext->LocalToString(params[3], &name);
	pContext->LocalToString(params[4], &description);

	command->AddLocalization(language, name, description && description[0] ? description : nullptr);
	return 1;
}

static cell_t slashcommand_SetInteractionContexts(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	cell_t* contexts_array = nullptr;
	int numContexts;
	if (!GetNativeIntInRange(pContext, params[3], 0, 3, "Interaction context count", numContexts)) return 0;
	if (numContexts > 0 && !GetNativeArray(pContext, params[2], &contexts_array, "interaction context")) return 0;

	std::vector<dpp::interaction_context_type> contexts;
	contexts.reserve(numContexts);
	for (int i = 0; i < numContexts; i++) {
		dpp::interaction_context_type context = static_cast<dpp::interaction_context_type>(contexts_array[i]);
		contexts.push_back(context);
	}

	command->SetInteractionContexts(contexts);
	return 1;
}

static cell_t slashcommand_SetIntegrationTypes(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	cell_t* types_array = nullptr;
	int numTypes;
	if (!GetNativeIntInRange(pContext, params[3], 0, 2, "Integration type count", numTypes)) return 0;
	if (numTypes > 0 && !GetNativeArray(pContext, params[2], &types_array, "integration type")) return 0;

	std::vector<dpp::application_integration_types> types;
	types.reserve(numTypes);
	for (int i = 0; i < numTypes; i++) {
		dpp::application_integration_types type = static_cast<dpp::application_integration_types>(types_array[i]);
		types.push_back(type);
	}

	command->SetIntegrationTypes(types);
	return 1;
}

static cell_t slashcommand_SetOptionMinValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	float min_value = sp_ctof(params[2]);
	command->SetLastOptionMinValue(static_cast<double>(min_value));
	return 1;
}

static cell_t slashcommand_SetOptionMaxValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	float max_value = sp_ctof(params[2]);
	command->SetLastOptionMaxValue(static_cast<double>(max_value));
	return 1;
}

static cell_t slashcommand_SetOptionMinLength(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	int min_length;
	if (!GetNativeIntInRange(pContext, params[2], 0, 6000, "Option minimum length", min_length)) return 0;
	command->SetLastOptionMinLength(static_cast<int64_t>(min_length));
	return 1;
}

static cell_t slashcommand_SetOptionMaxLength(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	int max_length;
	if (!GetNativeIntInRange(pContext, params[2], 1, 6000, "Option maximum length", max_length)) return 0;
	command->SetLastOptionMaxLength(static_cast<int64_t>(max_length));
	return 1;
}

static cell_t slashcommand_AddOptionChannelType(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	dpp::channel_type channel_type = static_cast<dpp::channel_type>(params[2]);

	command->AddLastOptionChannelType(channel_type);
	return 1;
}

static cell_t slashcommand_GetMention(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	std::string mention = command->GetMention();
	pContext->StringToLocal(params[2], params[3], mention.c_str());
	return 1;
}

static cell_t slashcommand_GetCommandId(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	std::string commandId = std::to_string(command->GetCommandId());
	pContext->StringToLocal(params[2], params[3], commandId.c_str());
	return 1;
}

static cell_t slashcommand_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordSlashCommand* command = Handles.GetPointer<DiscordSlashCommand>(pContext, params[1]);
	if (!command) return 0;

	std::string guildId = std::to_string(command->GetGuildId());
	pContext->StringToLocal(params[2], params[3], guildId.c_str());
	return 1;
}

static cell_t slashcommand_BulkDeleteGlobalCommands(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	DiscordSlashCommand::BulkDeleteGlobal(discord, callback, data);
	return 1;
}

extern const sp_nativeinfo_t slashcommand_natives[] = {
	{"DiscordSlashCommand.DiscordSlashCommand", slashcommand_CreateSlashCommand},
	{"DiscordSlashCommand.FromGlobalCommand", slashcommand_FromGlobalCommand},
	{"DiscordSlashCommand.FromGuildCommand", slashcommand_FromGuildCommand},
	{"DiscordSlashCommand.SetName", slashcommand_SetName},
	{"DiscordSlashCommand.SetDescription", slashcommand_SetDescription},
	{"DiscordSlashCommand.SetDefaultPermissions", slashcommand_SetDefaultPermissions},
	{"DiscordSlashCommand.GetName", slashcommand_GetName},
	{"DiscordSlashCommand.GetDescription", slashcommand_GetDescription},
	{"DiscordSlashCommand.GetDefaultPermissions", slashcommand_GetDefaultPermissions},
	{"DiscordSlashCommand.AddOption", slashcommand_AddOption},
	{"DiscordSlashCommand.AddStringOptionWithChoices", slashcommand_AddStringOptionWithChoices},
	{"DiscordSlashCommand.AddIntOptionWithChoices", slashcommand_AddIntOptionWithChoices},
	{"DiscordSlashCommand.AddFloatOptionWithChoices", slashcommand_AddFloatOptionWithChoices},
	{"DiscordSlashCommand.AddStringChoice", slashcommand_AddStringChoice},
	{"DiscordSlashCommand.AddIntChoice", slashcommand_AddIntChoice},
	{"DiscordSlashCommand.AddInt64Choice", slashcommand_AddInt64Choice},
	{"DiscordSlashCommand.AddFloatChoice", slashcommand_AddFloatChoice},
	{"DiscordSlashCommand.RegisterToGuild", slashcommand_RegisterToGuild},
	{"DiscordSlashCommand.RegisterGlobally", slashcommand_RegisterGlobally},
	{"DiscordSlashCommand.Update", slashcommand_Update},
	{"DiscordSlashCommand.Delete", slashcommand_Delete},
	{"DiscordSlashCommand.AddPermissionOverride", slashcommand_AddPermissionOverride},
	{"DiscordSlashCommand.RemovePermissionOverride", slashcommand_RemovePermissionOverride},
	{"DiscordSlashCommand.ClearPermissionOverrides", slashcommand_ClearPermissionOverrides},
	{"DiscordSlashCommand.PermissionOverrideCount.get", slashcommand_GetPermissionOverrideCount},
	{"DiscordSlashCommand.GetPermissionOverride", slashcommand_GetPermissionOverride},
	{"DiscordSlashCommand.ApplyPermissionOverrides", slashcommand_ApplyPermissionOverrides},
	{"DiscordSlashCommand.ContextMenuType.get", slashcommand_GetContextMenuType},
	{"DiscordSlashCommand.ContextMenuType.set", slashcommand_SetContextMenuType},
	{"DiscordSlashCommand.IsNSFW.get", slashcommand_GetNSFW},
	{"DiscordSlashCommand.IsNSFW.set", slashcommand_SetNSFW},
	{"DiscordSlashCommand.DMPermission.get", slashcommand_GetDMPermission},
	{"DiscordSlashCommand.DMPermission.set", slashcommand_SetDMPermission},
	{"DiscordSlashCommand.AddLocalization", slashcommand_AddLocalization},
	{"DiscordSlashCommand.SetInteractionContexts", slashcommand_SetInteractionContexts},
	{"DiscordSlashCommand.SetIntegrationTypes", slashcommand_SetIntegrationTypes},
	{"DiscordSlashCommand.SetOptionMinValue", slashcommand_SetOptionMinValue},
	{"DiscordSlashCommand.SetOptionMaxValue", slashcommand_SetOptionMaxValue},
	{"DiscordSlashCommand.SetOptionMinLength", slashcommand_SetOptionMinLength},
	{"DiscordSlashCommand.SetOptionMaxLength", slashcommand_SetOptionMaxLength},
	{"DiscordSlashCommand.AddOptionChannelType", slashcommand_AddOptionChannelType},
	{"DiscordSlashCommand.GetMention", slashcommand_GetMention},
	{"DiscordSlashCommand.GetCommandId", slashcommand_GetCommandId},
	{"DiscordSlashCommand.GetGuildId", slashcommand_GetGuildId},
	{"DiscordSlashCommand.BulkDeleteGlobalCommands", slashcommand_BulkDeleteGlobalCommands},
	{nullptr, nullptr}
};
