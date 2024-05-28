#include "stdafx.h"
#include "ThreadDemo.h"

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

void ThreadDemo::Initialize()
{
	//Loop();
	//MultiThread();
	//SharedResource();
	LoopQuater();
}

void ThreadDemo::Update()
{
	progress += 0.1f;
	ImGui::ProgressBar(progress / 1000.f);
}

void ThreadDemo::Loop()
{
	for (int i = 0; i < 100; i++)
		printf("i : %d\n", i);
	printf("Loop1 Completed\n");

	for (int i = 0; i < 100; i++)
		printf("i : %d\n", i);
	printf("Loop2 Completed\n");
}

void ThreadDemo::Loop1()
{
	for (int i = 0; i < 100; i++)
		printf("Loop1 : %d\n", i);
	printf("Loop1 Completed\n");
}

void ThreadDemo::Loop2()
{
	for (int i = 0; i < 100; i++)
		printf("Loop2 : %d\n", i);
	printf("Loop2 Completed\n");
}

void ThreadDemo::MultiThread()
{
	thread t1(bind(&ThreadDemo::Loop1, this));
	thread t2(bind(&ThreadDemo::Loop2, this));

	t2.join();
	printf("t2 is joined\n");

	t1.join();
	printf("t1 is joined\n");
}

void ThreadDemo::SharedResource()
{
	thread t([&]()
	{
		while (true)
		{
			Sleep(100);

			printf("Progress : %f\n", progress);

			if (progress >= 1000)
			{
				printf("Completed!\n");
				break;
			}
		}
	});

	t.detach();
}

void ThreadDemo::RaceCondition(int& count)
{
	for (int i = 0; i < 1000000; i++)
		count++;
}

void ThreadDemo::LoopQuater()
{
	int count = 0; //<- Shared Resource

	vector<thread> threads;

	for (int i = 0; i < 4; i++)
	{
		function<void(int&)> delegation = bind(&ThreadDemo::RaceCondition, this, placeholders::_1);
		threads.push_back(thread(delegation, ref(count)));
	}

	for (int i = 0; i < 4; i++)
		threads[i].join();

	printf("count : %d\n", count);
}
