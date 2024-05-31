#include "stdafx.h"
#include "ThreadDemo.h"

//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

void ThreadDemo::Initialize()
{
	//Loop();
	//MultiThread();
	//SharedResource();
	//LoopQuater();
	//SetTimer();

	int a[10000];
	for (int i = 0; i < 10000; i++)
		a[i] = Math::Random(0, 9999);

	Performance p;
	p.Start();
	{
		sort(a, a + 10000);
	}
	printf("RunningTime : %f\n", p.End());

	for (int i = 0; i < 10000; i++)
	{
		if (i % 5 == 0)
			printf("\n");

		printf("%4d\t", a[i]);
	}
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
	lock_guard<mutex> lock(m);
	//m.lock();
	{
		for (int i = 0; i < 1000000; i++)
			count++;
	}
	//m.unlock();
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

void ThreadDemo::SetTimer()
{
	timer[0].Start([]() { printf("난 2초마다 2번 실행되지\n"); }, 2000, 2);
	timer[1].Start([]() { printf("난 1초마다 계속 실행되지\n"); }, 1000);
}
