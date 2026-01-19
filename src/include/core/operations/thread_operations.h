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

#include "base_operations.h"

class ThreadOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void Get(dpp::snowflake thread_id, Callback callback);
	void Create(dpp::snowflake channel_id, const char* name, dpp::channel_type type = dpp::CHANNEL_PUBLIC_THREAD, uint16_t auto_archive = 1440, bool invitable = true, uint16_t rate_limit = 0, Callback callback = nullptr);
	void CreateWithMessage(dpp::snowflake channel_id, dpp::snowflake message_id, const char* name, uint16_t auto_archive = 1440, uint16_t rate_limit = 0, Callback callback = nullptr);
	void CreateInForum(dpp::snowflake channel_id, const char* name, const char* message, const std::vector<dpp::snowflake>& tag_ids = {}, uint16_t auto_archive = 1440, uint16_t rate_limit = 0, Callback callback = nullptr);
	void CreateInForumWithMessage(dpp::snowflake channel_id, const char* name, const dpp::message& message, const std::vector<dpp::snowflake>& tag_ids = {}, uint16_t auto_archive = 1440, uint16_t rate_limit = 0, Callback callback = nullptr);
	void GetActive(dpp::snowflake guild_id, Callback callback);
	void Join(dpp::snowflake thread_id, Callback callback = nullptr);
	void Leave(dpp::snowflake thread_id, Callback callback = nullptr);
	void MemberAdd(dpp::snowflake thread_id, dpp::snowflake user_id, Callback callback = nullptr);
	void MemberRemove(dpp::snowflake thread_id, dpp::snowflake user_id, Callback callback = nullptr);
	void Archive(dpp::snowflake thread_id, bool archived = true, Callback callback = nullptr);
	void Lock(dpp::snowflake thread_id, bool locked = true, Callback callback = nullptr);
	void Modify(dpp::snowflake thread_id, const std::string& name = "", uint16_t auto_archive = 0, uint16_t rate_limit = 0, bool archived = false, bool locked = false, Callback callback = nullptr);
	void GetPublicArchived(dpp::snowflake channel_id, time_t before = 0, uint16_t limit = 0, Callback callback = nullptr);
	void GetPrivateArchived(dpp::snowflake channel_id, time_t before = 0, uint16_t limit = 0, Callback callback = nullptr);
	void GetMembers(dpp::snowflake thread_id, Callback callback = nullptr);
};
