#include "WorkerThread.h"
#include "ThreadScheduler.h"
#include "ITask.h"
#include <iostream>

TaskSystem::WorkerThread::WorkerThread() :
	_workAvailable(false),
	_mutex(),
	_condVar(),
	_isRunning(false)
{
}

TaskSystem::WorkerThread::~WorkerThread()
{
	_isRunning = false;
	_condVar.notify_one();
	_thread.join();
}

void TaskSystem::WorkerThread::StartThread()
{
	_isRunning = true;
	_thread = std::thread(&WorkerThread::ThreadFunction, this);
}

void TaskSystem::WorkerThread::WakeUp()
{
	_workAvailable = true;
	_condVar.notify_one();
}

void TaskSystem::WorkerThread::ThreadFunction()
{
	while (_isRunning)
	{
		std::shared_ptr<ITask> task(ThreadScheduler::GetInstance().GetSomeWork());

		if (task == nullptr)
		{
			_workAvailable = false;
			std::unique_lock<std::mutex> l(_mutex);
			_condVar.wait(l, [this]
			{
				return _workAvailable || !_isRunning;
			});
			continue;
		}

		try
		{
			task->Execute();
		}
		catch (std::exception e)
		{
			std::cout << e.what();
		}
		task->SetIsComplete();
	}
}
