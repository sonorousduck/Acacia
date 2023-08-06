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

#pragma once

#include "ConcurrentQueue.hpp"
#include "Task.hpp"

class ThreadPool;


#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>



namespace Ebony
{

	// ------------------------------------------------------------------
	//
	// This class provides Topological ordering for Tasks.  At one point
	// I had it as template code, but eventually realized the only thing
	// it is going to be used for is Tasks, so made it a concrete class instead.
	//
	// ------------------------------------------------------------------
	class ConcurrentTaskGraph
	{
	public:
		ConcurrentTaskGraph(std::function<void(void)> onComplete = nullptr);

		std::uint64_t getId() const { return m_Id; }
		void add(std::shared_ptr<Task> node);
		void declarePredecessor(std::uint64_t predecessor, std::uint64_t node);

		std::shared_ptr<Task> dequeue();
		bool queueEmpty();
		bool graphEmpty();

		void finalize();
		void taskComplete(std::uint64_t taskId);

	private:

		std::uint64_t m_Id;
		std::function<void(void)> m_OnComplete;
		std::mutex m_Mutex;
		bool m_Finalized{ false };
		std::unordered_map<std::uint64_t, std::shared_ptr<Task>> m_Nodes;
		std::unordered_map<std::uint64_t, std::vector<std::uint64_t>> m_AdjacencyList;
		std::unordered_map<std::uint64_t, std::uint16_t> m_PredecessorCount;
		ConcurrentQueue<std::uint64_t> m_QueueExecutable;
		std::uint16_t m_CountEnqueued{ 0 };


		friend class ThreadPool;

	};


}