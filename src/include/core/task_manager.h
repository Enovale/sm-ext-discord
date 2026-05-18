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

#include <queue>
#include <mutex>
#include <memory>
#include <type_traits>
#include <utility>

class TaskManager {
public:
	static TaskManager& Instance();

	class Task {
	public:
		Task() = default;

		template<typename Func>
		explicit Task(Func&& func)
			: m_impl(std::make_unique<TaskImpl<std::decay_t<Func>>>(std::forward<Func>(func))) {}

		Task(Task&&) noexcept = default;
		Task& operator=(Task&&) noexcept = default;

		Task(const Task&) = delete;
		Task& operator=(const Task&) = delete;

		explicit operator bool() const { return static_cast<bool>(m_impl); }
		void operator()() { m_impl->Run(); }

	private:
		struct TaskBase {
			virtual ~TaskBase() = default;
			virtual void Run() = 0;
		};

		template<typename Func>
		struct TaskImpl final : TaskBase {
			template<typename Callable>
			explicit TaskImpl(Callable&& func) : m_func(std::forward<Callable>(func)) {}
			void Run() override { m_func(); }

			Func m_func;
		};

		std::unique_ptr<TaskBase> m_impl;
	};

	template<typename Func>
	void Push(Func&& task) {
		PushTask(Task(std::forward<Func>(task)));
	}

	void ProcessFrame(size_t maxTasks = 0);
	void Clear();
	void SetAccepting(bool accepting);
	void SetMaxTasksPerFrame(size_t maxTasks);
	void SetMaxQueueSize(size_t maxQueueSize);

private:
	TaskManager() = default;
	void PushTask(Task task);

	std::queue<Task> m_queue;
	mutable std::mutex m_mutex;
	bool m_accepting{true};
	size_t m_maxTasksPerFrame{256};
	size_t m_maxQueueSize{4096};
	size_t m_droppedTasks{0};
};

inline TaskManager& Tasks = TaskManager::Instance();
