#include "ITask.h"
#include "ThreadScheduler.h"
#include <iostream>

TaskSystem::ITask::ITask() :
	_isBlocked(false),
	_isComplete(false),
	_cond(),
	_mutex()
{
}

TaskSystem::ITask::~ITask()
{
}

void TaskSystem::ITask::Schedule(std::shared_ptr<ITask> ptr)
{
	ThreadScheduler::GetInstance().Schedule(ptr);
}

bool TaskSystem::ITask::IsComplete()
{
	return _isComplete;
}

void TaskSystem::ITask::SetIsComplete()
{
	_isComplete = true;
	_cond.notify_one();
}

void TaskSystem::ITask::BlockTillComplete()
{
	_isBlocked = true;
	std::unique_lock<std::mutex> lock(_mutex);
	_cond.wait(lock, [this]
	{
		return _isComplete;
	});
	_isBlocked = false;
}

