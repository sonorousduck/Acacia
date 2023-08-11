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

#include "ThreadPool.hpp"


namespace Ebony
{

	// -----------------------------------------------------------------
	//
	// Using the Meyer's Singleton technique...this is thread safe
	//
	// -----------------------------------------------------------------
	ThreadPool& ThreadPool::instance()
	{
		// This generates a threadpool of the size of the allowed amount of threads on your current hardware
		static ThreadPool instance{ static_cast<std::uint16_t>(std::thread::hardware_concurrency()) };
		return instance;
	}

	// -----------------------------------------------------------------
	//
	// Shuts down all of the thread pool worker threads and removes
	// the thread pool instance.
	//
	// -----------------------------------------------------------------
	void ThreadPool::terminate()
	{
		// Tell each of the worker threads in the waiting queue to terminate
		for (auto& thread : instance().m_Threads)
		{
			thread->terminate();
		}

		instance().m_IoThread->terminate();

		// Signal the event so that any non-working threads will wake up and see they are finished
		instance().m_EventWorkQueue.notify_all();
		instance().m_IoEventWorkQueue.notify_all();

		// Wait for all the threads to complete
		for (auto& thread : instance().m_Threads)
		{
			thread->join();
		}

		instance().m_IoThread->join();
	}

	// -----------------------------------------------------------------
	//
	// The constructor creates the worker threads the thread pool will use
	// to process tasks.
	//
	// -----------------------------------------------------------------
	ThreadPool::ThreadPool(std::uint16_t sizeInitial)
	{
		for (std::uint16_t thread = 0; thread < sizeInitial; thread++)
		{
			auto worker = std::make_shared<WorkerThread>(m_WorkQueue, m_EventWorkQueue, m_IoMutexWorkQueueEvent);
			m_Threads.insert(worker);
		}

		m_IoThread = std::make_shared<WorkerThread>(m_WorkQueue, m_EventWorkQueue, m_IoMutexWorkQueueEvent);
	}


	// -----------------------------------------------------------------
	//
	// This places a new task on the work queue.  An event will be signaled
	// to one of the waiting threads.
	//
	// -----------------------------------------------------------------
	void ThreadPool::enqueueTask(std::shared_ptr<Task> source)
	{
		m_ActiveTasks++;

		if (!source->isIo())
		{
			m_WorkQueue.enqueue(source);
			// Notify a thread something was added to the queue, so it can be picked up and worked on
			m_EventWorkQueue.notify_one();
		}
		else
		{
			m_IoWorkQueue.enqueue(source);
			m_IoEventWorkQueue.notify_one();
		}
	}

	void ThreadPool::notifyEmpty(std::function<void(void)> onEmpty)
	{
		m_OnEmpty = onEmpty;
	}

	std::shared_ptr<ConcurrentTaskGraph> ThreadPool::createTaskGraph(std::function<void(void)> onComplete)
	{
		auto graph = std::make_shared<ConcurrentTaskGraph>(onComplete);
		
		m_TaskGraphs[graph->getId()] = graph;
		return graph;
	}


	std::shared_ptr<Task> ThreadPool::createTask(std::function<void(void)> job, std::function<void(void)> onComplete)
	{
		return std::make_shared<Task>(0, false, job, onComplete);
	}

	std::shared_ptr<Task> ThreadPool::createIOTask(std::function<void(void)> job, std::function<void(void)> onComplete)
	{
		return std::make_shared<Task>(0, true, job, onComplete);
	}

	std::shared_ptr<Task> ThreadPool::createTask(std::shared_ptr<ConcurrentTaskGraph>& graph, std::function<void(void)> job, std::function<void(void)> onComplete)
	{
		auto task = std::make_shared<Task>(graph->getId(), false, job, onComplete);
		graph->add(task);

		return task;
	}

	std::shared_ptr<Task> ThreadPool::createIOTask(std::shared_ptr<ConcurrentTaskGraph>& graph, std::function<void(void)> job, std::function<void(void)> onComplete)
	{
		auto task = std::make_shared<Task>(graph->getId(), true, job, onComplete);
		graph->add(task);

		return task;
	}

	// -----------------------------------------------------------------
	//
	// Pullable out any tasks that can be computed right now.  Dependent tasks
	// will get added as predecessor tasks complete.
	//
	// -----------------------------------------------------------------
	void ThreadPool::submitTaskGraph(std::shared_ptr<ConcurrentTaskGraph> graph)
	{
		graph->finalize();
		enqueueAvailableGraphTasks(graph);
	}

	// -----------------------------------------------------------------
	//
	// Pullable out any tasks that can be computed right now and add them all
	// at once, to ensure none complete while others from the same graph
	// are still being added.
	//
	// -----------------------------------------------------------------
	void ThreadPool::enqueueAvailableGraphTasks(std::shared_ptr<ConcurrentTaskGraph> graph)
	{
		// We do this so that we don't notify worker threads there might be tasks when there aren't.
		// This issue is that if there aren't any taks, things that will notify on empty will receive a call as a result
		if (graph->queueEmpty())
		{
			return;
		}

		std::vector<std::shared_ptr<Task>> tasks;
		std::vector<std::shared_ptr<Task>> tasksIO;

		while (!graph->queueEmpty())
		{
			m_ActiveTasks++;
			auto task = graph->dequeue();
			if (!task->isIo())
			{
				tasks.push_back(task);
			}
			else
			{
				tasksIO.push_back(task);
			}
		}

		m_WorkQueue.enqueue(tasks);
		m_EventWorkQueue.notify_all();

		m_IoWorkQueue.enqueue(tasksIO);
		m_IoEventWorkQueue.notify_all();
	}

	// -----------------------------------------------------------------
	//
	// When a task completes, need to update the graph it is associated with
	// then pull out any tasks that can now be executed.  If the graph
	// no longer has any more tasks available, and the task queue is
	// empty, remove the graph from any further tracking.
	//
	// -----------------------------------------------------------------
	void ThreadPool::taskComplete(const std::shared_ptr<Task>& task)
	{
		// Because of the use of non-synchronized containers, this method needs to be synchronized itself
		std::lock_guard<std::mutex> lock(m_MutexTaskComplete);

		m_ActiveTasks--;

		// If the associated graph doesn't exist, there are no more tasks possible from the graph
		// so no need to look at the graph again.
		if (m_TaskGraphs.contains(task->getGraphId()))
		{
			auto graph = m_TaskGraphs[task->getGraphId()];
			graph->taskComplete(task->getId());

			// This can only be true if all tasks in the graph have no predecessors and are
			// either done with their execution or in the available execution task queue on
			// the thread pool itself.
			if (graph->graphEmpty() && graph->queueEmpty())
			{
				m_TaskGraphs.erase(graph->getId());
			}

			enqueueAvailableGraphTasks(graph);
		}

		if (m_ActiveTasks.load() == 0 && m_OnEmpty)
		{
			m_OnEmpty();
			m_OnEmpty = nullptr;
		}

	}
}
