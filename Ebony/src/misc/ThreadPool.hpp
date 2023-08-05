#pragma once
/*
Copyright (c) 2022 James Dean Mathias

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <unordered_map>

#include "Task.hpp"
#include "ConcurrentQueue.hpp"
#include "WorkerThread.hpp"

namespace Ebony
{



	class ThreadPool
	{
	public:
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;


		static ThreadPool& instance();
		static void terminate();

		void enqueueTask(std::shared_ptr<Task> task);
		void notifyEmpty(std::function<void(void)> onEmpty);


		// TODO: Missing createTaskGraph function
		std::shared_ptr<Task> createTask(std::function<void(void)> job, std::function<void(void)> onComplete = nullptr);
		std::shared_ptr<Task> createIOTask(std::function<void(void)> job, std::function<void(void)> onComplete = nullptr);
		// TODO: Missing createTask and createIOTask when you pass in the graph as well

		//void submitTaskGraph(std::shared_ptr<ConcurrentTaskGraph> graph);

	protected:
		ThreadPool(std::uint16_t sizeInitial);

	private:
		std::function<void(void)> m_OnEmpty{ nullptr };
		std::atomic_uint32_t m_ActiveTasks{ 0 };
		std::mutex m_MutexTaskComplete;

		std::set<std::shared_ptr<WorkerThread>> m_Threads;
		ConcurrentQueue<std::shared_ptr<Task>> m_WorkQueue;
		std::condition_variable m_EventWorkQueue;
		std::mutex m_MutexWorkQueueEvent;
		//std::unordered_map<std::uint64_t, std::shared_ptr<ConcurrentTaskGraph>> m_TaskGraphs;

		// Separate threading system for IO as well, don't want to bog down systems with this
		std::shared_ptr<WorkerThread> m_IoThread;
		ConcurrentQueue<std::shared_ptr<Task>> m_IoWorkQueue;
		std::condition_variable m_IoEventWorkQueue;
		std::mutex m_IoMutexWorkQueueEvent;

		//void enqueueAvailableGraphTasks(std::shared_ptr<ConcurrentTaskGraph> graph);
		void taskComplete(const std::shared_ptr<Task>& task);


		friend class WorkerThread;

	};
}