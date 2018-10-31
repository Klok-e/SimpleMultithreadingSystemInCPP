#include "ThreadScheduler.h"


TaskSystem::ThreadScheduler & TaskSystem::ThreadScheduler::GetInstance()
{
	static ThreadScheduler s;
	return s;
}

void TaskSystem::ThreadScheduler::Initialize(int threads)
{
	for (int i = 0; i < threads; i++)
	{
		auto ptr = std::shared_ptr<WorkerThread>(new WorkerThread());
		_workers.push_back(ptr);
	}
	for (int i = 0; i < threads; i++)
	{
		_workers[i]->StartThread();
	}
	_isInitialized = true;
}

void TaskSystem::ThreadScheduler::Schedule(std::shared_ptr<ITask> const task)
{
	_tasks.Enqueue(task);
	if (!_isInitialized)
		throw std::exception("Thread Scheduler wasn't initialized. Call TaskSystem::ThreadScheduler::Initialize(int threads) before calling Schedule.");
	int s = _workers.size();
	for (int i = 0; i < s; i++)
	{
		_workers[i]->WakeUp();
	}
}

std::shared_ptr<TaskSystem::ITask> const TaskSystem::ThreadScheduler::GetSomeWork()
{
	if (!_tasks.Empty())
		return _tasks.Dequeue();
	else
		return nullptr;
}



TaskSystem::ThreadScheduler::ThreadScheduler() :
	_isInitialized(false),
	_workers(),
	_tasks()
{
}

TaskSystem::ThreadScheduler::~ThreadScheduler()
{
}
