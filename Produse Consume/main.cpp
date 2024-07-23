#include <queue>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <chrono>


using namespace std;

queue<int> Q;
condition_variable cond;
mutex m;

void Produce()
{
	while (true)
	{
		int data = rand() % 11;
		this_thread::sleep_for(chrono::milliseconds(rand() % 3000));
		unique_lock<mutex> lck(m);
		Q.push(data);
		cond.notify_one();
	}
}

void Consume()
{
	while (true)
	{
		unique_lock<mutex> lck(m);
		cond.wait(lck, []() {return !Q.empty(); });
		int data = Q.front();
		Q.pop();
		lck.unlock();
		cout << data << endl;
	}
}

int main()
{
	thread t1([]()
	{
		Produce();
	});
	thread t2([]()
	{
		Consume();
	});
	t1.join();
	t2.join();
	system("pause");
	return 0;
}