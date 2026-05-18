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

#include "core/handle_manager.h"
#include "utils/discord_http.h"

static cell_t httpheaders_HttpHeaders(IPluginContext* pContext, const cell_t* params)
{
	HttpHeaders* headers = new HttpHeaders();
	Handle_t handle = Handles.Create(pContext, headers, HandleId::HttpHeaders);
	if (!handle) return 0;
	return handle;
}

static cell_t httpheaders_SetHeader(IPluginContext* pContext, const cell_t* params)
{
	HttpHeaders* headers = Handles.GetPointer<HttpHeaders>(pContext, params[1]);
	if (!headers) return 0;

	char* name;
	char* value;
	pContext->LocalToString(params[2], &name);
	pContext->LocalToString(params[3], &value);

	headers->SetHeader(name, value);
	return 1;
}

static cell_t httpheaders_GetHeader(IPluginContext* pContext, const cell_t* params)
{
	HttpHeaders* headers = Handles.GetPointer<HttpHeaders>(pContext, params[1]);
	if (!headers) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	std::string value;
	if (headers->GetHeader(name, value)) {
		pContext->StringToLocalUTF8(params[3], params[4], value.c_str(), nullptr);
		return 1;
	}
	return 0;
}

static cell_t httpheaders_RemoveHeader(IPluginContext* pContext, const cell_t* params)
{
	HttpHeaders* headers = Handles.GetPointer<HttpHeaders>(pContext, params[1]);
	if (!headers) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	return headers->RemoveHeader(name);
}

static cell_t httpheaders_ClearHeaders(IPluginContext* pContext, const cell_t* params)
{
	HttpHeaders* headers = Handles.GetPointer<HttpHeaders>(pContext, params[1]);
	if (!headers) return 0;

	headers->ClearHeaders();
	return 1;
}

static cell_t httpheaders_Count(IPluginContext* pContext, const cell_t* params)
{
	HttpHeaders* headers = Handles.GetPointer<HttpHeaders>(pContext, params[1]);
	if (!headers) return 0;

	return static_cast<cell_t>(headers->Count());
}

static cell_t httpheaders_GetHeaderByIndex(IPluginContext* pContext, const cell_t* params)
{
	HttpHeaders* headers = Handles.GetPointer<HttpHeaders>(pContext, params[1]);
	if (!headers) return 0;

	size_t index = static_cast<size_t>(params[2]);
	std::string name, value;

	if (headers->GetHeaderByIndex(index, name, value)) {
		pContext->StringToLocalUTF8(params[3], params[4], name.c_str(), nullptr);
		pContext->StringToLocalUTF8(params[5], params[6], value.c_str(), nullptr);
		return 1;
	}
	return 0;
}

static cell_t httpcompletion_GetStatus(IPluginContext* pContext, const cell_t* params)
{
	HttpCompletion* completion = Handles.GetPointer<HttpCompletion>(pContext, params[1]);
	if (!completion) return 0;

	return static_cast<cell_t>(completion->GetStatus());
}

static cell_t httpcompletion_GetBody(IPluginContext* pContext, const cell_t* params)
{
	HttpCompletion* completion = Handles.GetPointer<HttpCompletion>(pContext, params[1]);
	if (!completion) return 0;

	pContext->StringToLocalUTF8(params[2], params[3], completion->GetBody().c_str(), nullptr);
	return 1;
}

static cell_t httpcompletion_GetBodyLength(IPluginContext* pContext, const cell_t* params)
{
	HttpCompletion* completion = Handles.GetPointer<HttpCompletion>(pContext, params[1]);
	if (!completion) return 0;

	return static_cast<cell_t>(completion->GetBodyLength());
}

static cell_t httpcompletion_GetResponseHeaders(IPluginContext* pContext, const cell_t* params)
{
	HttpCompletion* completion = Handles.GetPointer<HttpCompletion>(pContext, params[1]);
	if (!completion) return 0;

	Handle_t handle = completion->GetResponseHeadersHandle();
	if (!handle) return 0;

	return handle;
}

static cell_t httpcompletion_GetProtocol(IPluginContext* pContext, const cell_t* params)
{
	HttpCompletion* completion = Handles.GetPointer<HttpCompletion>(pContext, params[1]);
	if (!completion) return 0;

	pContext->StringToLocalUTF8(params[2], params[3], completion->GetProtocol().c_str(), nullptr);
	return 1;
}

static cell_t httpcompletion_IsSuccess(IPluginContext* pContext, const cell_t* params)
{
	HttpCompletion* completion = Handles.GetPointer<HttpCompletion>(pContext, params[1]);
	if (!completion) return 0;

	return completion->IsSuccess();
}

extern const sp_nativeinfo_t http_natives[] = {
	{"HttpHeaders.HttpHeaders", httpheaders_HttpHeaders},
	{"HttpHeaders.SetHeader", httpheaders_SetHeader},
	{"HttpHeaders.GetHeader", httpheaders_GetHeader},
	{"HttpHeaders.RemoveHeader", httpheaders_RemoveHeader},
	{"HttpHeaders.ClearHeaders", httpheaders_ClearHeaders},
	{"HttpHeaders.Count.get", httpheaders_Count},
	{"HttpHeaders.GetHeaderByIndex", httpheaders_GetHeaderByIndex},
	{"HttpCompletion.Status.get", httpcompletion_GetStatus},
	{"HttpCompletion.GetBody", httpcompletion_GetBody},
	{"HttpCompletion.BodyLength.get", httpcompletion_GetBodyLength},
	{"HttpCompletion.ResponseHeaders.get", httpcompletion_GetResponseHeaders},
	{"HttpCompletion.GetProtocol", httpcompletion_GetProtocol},
	{"HttpCompletion.IsSuccess.get", httpcompletion_IsSuccess},
	{nullptr, nullptr}
};
