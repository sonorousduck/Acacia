#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <unordered_map>


class ThreadPool
{
public:
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;


	static ThreadPool& instance;
	static void terminate();

	void enqueueTask(std::shared_ptr<Task> task);


};