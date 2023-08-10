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

#include "ConcurrentTaskGraph.hpp"

#include <algorithm>
#include <atomic>
#include <cassert>

namespace Ebony
{
	ConcurrentTaskGraph::ConcurrentTaskGraph(std::function<void(void)> onComplete) :
		m_OnComplete(onComplete)
	{
		static std::atomic_uint64_t currentId{ 1 };

		m_Id = currentId.load();
		currentId++;
	}

	void ConcurrentTaskGraph::add(std::shared_ptr<Task> node)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);

		assert(m_Finalized == false);

		assert(m_AdjacencyList.find(node->getId()) == m_AdjacencyList.end());

		m_Nodes[node->getId()] = node;
		m_AdjacencyList[node->getId()] = {};
		m_PredecessorCount[node->getId()] = 0;
	}

	void ConcurrentTaskGraph::declarePredecessor(std::uint64_t predecessor, std::uint64_t node)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);

		assert(m_Finalized == false);
		assert(m_Nodes.contains(node));
		assert(m_Nodes.contains(predecessor));
		assert(m_AdjacencyList.contains(predecessor));
		// NOTE: If there are a lot of dependencies, this assert could get close
		assert(std::find(m_AdjacencyList[predecessor].begin(), m_AdjacencyList[predecessor].end(), node) == m_AdjacencyList[predecessor].end());

		// Add the directed edge, assuming the edge doesn't already exist
		m_AdjacencyList[predecessor].push_back(node);
	}

	// ------------------------------------------------------------------
	//
	// This method traverses the graph using a topological ordering, placing
	// the values into the final item/task queue based upon the graph
	// dependencies.  Once the graph is finalized, the 'add' methods should
	// not be called again.
	//
	// ------------------------------------------------------------------
	void ConcurrentTaskGraph::finalize()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);

		assert(m_Finalized == false);

		// Step 1: Count the predecessors
		for (auto&& [id, predecessor] : m_AdjacencyList)
		{
			for (auto nodeId : predecessor)
			{
				m_PredecessorCount[nodeId]++;
			}
		}

		// Step 2: For each node with a predecessor count of 0, add to the available task queue
		for (auto&& [id, count] : m_PredecessorCount)
		{
			if (count == 0)
			{
				m_QueueExecutable.enqueue(id);
				m_CountEnqueued++;
			}
		}

		// Step 3: Performed in the taskComplete method below. We have to wait for the task that have 0 predecessors
		// to execute before continuing with the topological ordering any further
		m_Finalized = true;
	}

	void ConcurrentTaskGraph::taskComplete(std::uint64_t taskId)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);

		// If both the graph and the task queue are empty, that means this was the last task in the graph to finish.
		// Thus, the onComplete function needs to be called
		if (m_OnComplete && queueEmpty() && graphEmpty())
		{
			m_OnComplete();
		}
		
		// Based on the completed execution of this node, work through its
		// successor nodes, updating predecessor counts and adding new nodes into the
		// available task queue as appropriate
		
		// For each successor of this node, subtract one from the predecessor count.
		// Then, if that predecessor count is 0, add it to the available task queue
		for (auto successorId : m_AdjacencyList[taskId])
		{
			m_PredecessorCount[successorId]--;
			if (m_PredecessorCount[successorId] == 0)
			{
				m_QueueExecutable.enqueue(successorId);
				m_CountEnqueued++;
			}
		}
	}

	std::shared_ptr<Task> ConcurrentTaskGraph::dequeue()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		assert(!queueEmpty());

		return m_Nodes[m_QueueExecutable.dequeue().value()];
	}

	bool ConcurrentTaskGraph::queueEmpty()
	{
		return m_QueueExecutable.size() == 0;
	}

	bool ConcurrentTaskGraph::graphEmpty()
	{
		return m_CountEnqueued == m_PredecessorCount.size();
	}

}