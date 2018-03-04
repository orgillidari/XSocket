#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

const int tCount = 5;

std::mutex m;
std::atomic_int sum = 0;

void OtherThread(int id)
{
	for (int i = 0; i < 2000000; ++i)
	{
		//std::lock_guard<std::mutex> lg(m);
		//m.lock();
		sum++;
		//m.unlock();
	}
}

/****************************************************************************************************************
 *
 *    Brief   : ���̵߳�����ִ�з�ʽ�ıȽϡ�
		* detach() �����߳�ͬʱ���С�
		* join() �ȴ��߳�ִ����ɺ�ִ�����̡߳�

		1- ʹ������ʱ����Ҫʹ��join()��ʽ��������֤���������ʱ��û��ʹ����������ر����������ʹ��join()��ʽ����Ҫ��֤���������ʱ��û��ʹ������
		2- ʹ������ʱ����ʹ�ٽ���С�������൱�ڲ����ö��̡߳���ʹ�첽�������ͬ��������
		3- ���߳�ִ�е�ʱ���̼߳��ִ��˳��ȷ����
		4- ���̲߳��м����ʱ�������ʹ���ٽ������������ȷ����
		5- ���Լ������������ģ��ڴ��ģ����ʱƵ���ļ�������������Ĵ��������ܡ�debug��10�δ�Լ2.2���롣
		6- ʹ���Խ��������ܱ���ͨ�����Ĵ�һ�㣬���Ǹ���ȫ��
		7- detach()��join()���������̡߳��߳�һ���������Զ�ִ���ˡ�
		8- ���߳���ʹ�û������ݵ�ʱ�����ʹ��Ժ�Ӳ���������ܡ�
 *
 ****************************************************************************************************************/
int main()
{
	std::thread t[tCount];
	for (int i = 0; i < tCount; ++i)
	{
		t[i] = std::thread(OtherThread, i);
	}

	//ʹ�ö��̲߳���-����
	std::chrono::time_point<std::chrono::high_resolution_clock> begin1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < tCount; ++i)
	{
		//t[i].detach();
		t[i].join();
	}
	std::chrono::time_point<std::chrono::high_resolution_clock> end1 = std::chrono::high_resolution_clock::now();
	std::chrono::microseconds time1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);
	std::cout << "time = " << time1.count() * 0.000001 << "  :  sum = " << sum << std::endl;

	//ʹ�õ��̲߳���-������
	sum = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> begin2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000000; ++i)
	{
		sum++;
	}
	std::chrono::time_point<std::chrono::high_resolution_clock> end2 = std::chrono::high_resolution_clock::now();
	std::chrono::microseconds time2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2);
	std::cout << "time = " << time2.count() * 0.000001 << "  :  sum = " << sum << std::endl;

	//���߳�����
	std::cout << "Hello main thread!" << std::endl;
	
	return 0;
}