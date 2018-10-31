#pragma once
#include <thread>
#include <condition_variable>
#include <mutex>

namespace TaskSystem
{
	class WorkerThread
	{
	public:
		WorkerThread();

		~WorkerThread();

		void StartThread();

		void WakeUp();

	private:
		std::thread _thread;
		std::mutex _mutex;
		std::condition_variable _condVar;
		bool _isRunning;
		bool _workAvailable;

		void ThreadFunction();
	};
}
