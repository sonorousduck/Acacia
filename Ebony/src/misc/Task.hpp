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

#include <functional>
#include <cstdint>

namespace Ebony
{
	class Task final
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