#pragma once
#include <vector>
#include "WorkerThread.h"
#include "ITask.h"
#include "ConcurrentQueue.h"

namespace TaskSystem
{
	class ThreadScheduler
	{
	public:
		static ThreadScheduler & GetInstance();

		void Initialize(int threads = std::thread::hardware_concurrency() - 1);

		void Schedule(std::shared_ptr<ITask> const task);

		std::shared_ptr<ITask> const GetSomeWork();

	private:
		bool _isInitialized;
		std::vector<std::shared_ptr<WorkerThread>> _workers;
		ConcurrentQueue<std::shared_ptr<ITask>> _tasks;

		ThreadScheduler();
		~ThreadScheduler();

		ThreadScheduler(ThreadScheduler const&) = delete;
		ThreadScheduler& operator= (ThreadScheduler const&) = delete;
	};
}

