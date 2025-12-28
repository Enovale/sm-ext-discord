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

#include "natives/natives_common.h"

// Common entity native implementations using templates
// These work with any entity type that has the corresponding getter methods

// Get ID as string (for entities with GetId() returning std::string)
template<typename T>
static cell_t EntityGetId(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	pContext->StringToLocal(params[2], params[3], entity->GetId().c_str());
	return 1;
}

// Get name as string (for entities with GetName() returning const char*)
template<typename T>
static cell_t EntityGetName(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	pContext->StringToLocal(params[2], params[3], entity->GetName());
	return 1;
}

// Get description as string (for entities with GetDescription() returning const char*)
template<typename T>
static cell_t EntityGetDescription(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	pContext->StringToLocal(params[2], params[3], entity->GetDescription());
	return 1;
}

// Get URL as string (for entities with GetUrl() returning std::string)
template<typename T>
static cell_t EntityGetUrl(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	pContext->StringToLocal(params[2], params[3], entity->GetUrl().c_str());
	return 1;
}

// Get guild ID as string (for entities with GetGuildId() returning std::string)
template<typename T>
static cell_t EntityGetGuildId(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	pContext->StringToLocal(params[2], params[3], entity->GetGuildId().c_str());
	return 1;
}

// Get user ID as string (for entities with GetUserId() returning std::string)
template<typename T>
static cell_t EntityGetUserId(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	pContext->StringToLocal(params[2], params[3], entity->GetUserId().c_str());
	return 1;
}

// Get boolean property (for entities with a bool getter)
template<typename T, bool (T::*Getter)() const>
static cell_t EntityGetBool(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	return (entity->*Getter)();
}

// Get integer property (for entities with an int/enum getter)
template<typename T, typename R, R (T::*Getter)() const>
static cell_t EntityGetInt(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	return static_cast<cell_t>((entity->*Getter)());
}

// Get mention as string (for entities with GetMention() returning std::string)
template<typename T>
static cell_t EntityGetMention(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	pContext->StringToLocal(params[2], params[3], entity->GetMention().c_str());
	return 1;
}

// Get flags (for entities with GetFlags() returning integer type)
template<typename T>
static cell_t EntityGetFlags(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	return static_cast<cell_t>(entity->GetFlags());
}

// Delete entity (for entities with Delete() method)
template<typename T>
static cell_t EntityDelete(IPluginContext* pContext, const cell_t* params) {
	T* entity = Handles.GetPointer<T>(pContext, params[1]);
	if (!entity) return 0;

	entity->Delete();
	return 1;
}
