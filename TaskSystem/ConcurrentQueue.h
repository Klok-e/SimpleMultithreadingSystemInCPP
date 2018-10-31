#pragma once
#include <queue>
#include <mutex>

namespace TaskSystem
{
	template<typename T>
	class ConcurrentQueue
	{
	public:
		ConcurrentQueue<T>() :
			_queue(),
			_mutex()
		{
		}
		~ConcurrentQueue<T>()
		{
		}

		T Dequeue()
		{
			std::lock_guard<std::mutex> l(_mutex);
			if (!_queue.empty())
			{
				auto item = _queue.front();
				_queue.pop();
				return item;
			}
			else
				return T();
		}

		void Enqueue(const T& item)
		{
			std::lock_guard<std::mutex> l(_mutex);
			_queue.push(item);
		}

		bool Empty()
		{
			std::lock_guard<std::mutex> l(_mutex);
			return _queue.empty();
		}

	private:
		std::queue<T> _queue;
		std::mutex _mutex;
	};
}