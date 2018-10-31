#pragma once
#include <condition_variable>
#include <mutex>

namespace TaskSystem
{
	class ITask
	{
	public:
		ITask();
		virtual ~ITask();

		void Schedule(std::shared_ptr<ITask> ptr);

		bool IsComplete();
		void SetIsComplete();

		void BlockTillComplete();

		virtual void Execute() = 0;

	private:
		volatile bool _isComplete;
		bool _isBlocked;
		std::condition_variable _cond;
		std::mutex _mutex;
	};
}
