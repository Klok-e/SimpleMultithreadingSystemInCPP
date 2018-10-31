#include <iostream>
#include "ThreadScheduler.h"
#include "ITask.h"
#include <vector>
#include <string>

void sortBubble(int * arr, int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

class SomeWork : public TaskSystem::ITask
{
public:
	std::string name;

	~SomeWork() override
	{
		delete[] arr;
	}

	// Inherited via ITask
	virtual void Execute() override
	{
		arr = new int[s];

		for (int i = 0; i < s; i++)
		{
			arr[i] = s - i;
		}
		sortBubble(arr, s);
	}

	const int s = 60000;
	int* arr;
};

int main()
{
	TaskSystem::ThreadScheduler::GetInstance().Initialize();

	std::vector<std::shared_ptr<SomeWork>> tasks;

	const int num = 10;

	for (int i = 0; i < num; i++)
	{
		std::shared_ptr<SomeWork> t = std::make_shared<SomeWork>();
		t->name = "work " + std::to_string(i) + '\n';

		tasks.push_back(t);
		t->Schedule(t);
	}
	for (int i = 0; i < num; i++)
	{
		auto tsk = tasks.back();
		tasks.pop_back();

		std::cout << "block\n";
		tsk->BlockTillComplete();
		std::cout << "unblock\n";
	}

	std::cin.ignore();
}
