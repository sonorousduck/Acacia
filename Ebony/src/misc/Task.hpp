#pragma once

#include <functional>
#include <cstdint>

// Referenced work by Dean Mathias
namespace Ebony
{
	class Task
	{
	public:
		Task(std::uint64_t graphId, bool isIO, std::function<void()> job, std::function<void()> onComplete = nullptr);

		std::uint64_t getId() const { return m_Id; }
		std::uint64_t getGraphId() const { return m_GraphId; }

		bool isIo() { return m_isIO; }
		void execute();

	private:
		std::uint64_t m_Id{ 0 };
		std::uint64_t m_GraphId{ 0 };
		bool m_isIO = false;
		std::function<void()> m_Job;
		std::function<void()> m_OnComplete;

	};

}