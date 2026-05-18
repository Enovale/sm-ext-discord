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

#include "core/task_manager.h"
#include "core/logger.h"

TaskManager& TaskManager::Instance() {
	static TaskManager instance;
	return instance;
}

void TaskManager::PushTask(Task task) {
	std::lock_guard<std::mutex> lock(m_mutex);
	if (!m_accepting) return;
	if (m_maxQueueSize > 0 && m_queue.size() >= m_maxQueueSize) {
		++m_droppedTasks;
		if (m_droppedTasks == 1 || (m_droppedTasks % 1024) == 0) {
			Log.Error("Discord task queue is full (%zu pending); dropped %zu task(s)", m_queue.size(), m_droppedTasks);
		}
		return;
	}
	m_queue.push(std::move(task));
}

void TaskManager::ProcessFrame(size_t maxTasks) {
	size_t limit = maxTasks;
	if (limit == 0) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_queue.empty()) return;
		limit = m_queue.size();
		if (m_maxTasksPerFrame > 0 && limit > m_maxTasksPerFrame) {
			limit = m_maxTasksPerFrame;
		}
	}

	Task task;
	size_t count = 0;
	while (count < limit) {
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
	std::queue<Task> pending;
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		std::swap(m_queue, pending);
		m_droppedTasks = 0;
	}
}

void TaskManager::SetAccepting(bool accepting) {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_accepting = accepting;
}

void TaskManager::SetMaxTasksPerFrame(size_t maxTasks) {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_maxTasksPerFrame = maxTasks;
}

void TaskManager::SetMaxQueueSize(size_t maxQueueSize) {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_maxQueueSize = maxQueueSize;
}
