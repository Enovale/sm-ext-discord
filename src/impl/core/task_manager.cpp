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

#include "core/task_manager.h"

TaskManager& TaskManager::Instance() {
	static TaskManager instance;
	return instance;
}

void TaskManager::Push(std::function<void()> task) {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_queue.push(std::move(task));
}

void TaskManager::ProcessFrame(int maxTasks) {
	std::function<void()> task;
	int count = 0;
	while (count < maxTasks) {
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			if (m_queue.empty()) break;
			task = std::move(m_queue.front());
			m_queue.pop();
		}
		task();
		++count;
	}
}

void TaskManager::Clear() {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_queue = std::queue<std::function<void()>>();
}
