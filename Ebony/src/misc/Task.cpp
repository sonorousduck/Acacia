#include "Task.hpp"
#include <atomic>

// Referenced work by Dean Mathias
namespace Ebony
{
	Task::Task(std::uint64_t graphId, bool isIO, std::function<void()> job, std::function<void()> onComplete) :
		m_GraphId(graphId), m_isIO(isIO), m_Job(job), m_OnComplete(onComplete)
	{
		static std::atomic_uint64_t currentId{ 1 };

		m_Id = currentId.load();
		currentId++;
	}


	void Task::execute()
	{
		m_Job();
		
		if (m_OnComplete)
		{
			m_OnComplete();
		}
	}


}