#include<thread>
#include <iostream>
#include<vector>
#include<list>
#include<mutex>//互斥量头文件
using namespace std;
/*
vector<int>v = { 1,2,3 };
//只读数据
void myprint(int num)
{
	cout << "id为" << std::this_thread::get_id() << " 的线程 打印inum的值为：" << v[0] <<v[1]<<v[2]<<endl;
}

int main()
{//一：创建和等待多个线程

	vector<thread>mythreads;
	//创建10线程线程入口函数统一myprint
	for (int i = 0; i < 10; i++)
	{
		mythreads.push_back(thread(myprint,i));//创建了10个线程
	}
	for (auto it = mythreads.begin(); it != mythreads.end(); it++)
	{
		it->join();//等待10个线程都返回
	}
	cout << "主线程" << endl;


	return 0;
}
*/
//二共享数据问题
//只读数据共享是安全稳定的，直接读就可以
//有读有写
//两个线程写，8个线程写，如果没有特别的处理程序肯定崩溃
//最简单的不崩溃处理，读的时候不能写，写的时候不能读
//两个线程不能同时写，8个线程不能同时读
//其他案例
//数据共享；
//火车买票系统、
//网络服务器。一个线程收集玩家命令，并写到队列中，另一个从这个队列中取出发送来的命令，

class A
{
public:
	void inmsg()
	{
		for (int i = 0; i < 10000; i++)
		{
			{
				cout << "inmsg()执行插入一个元素" << i << endl;
				std::lock_guard<std::mutex>my_guard(my_mutex,std::adopt_lock);//在构造函数中不调用lock(),用std::adopt_lock
				std::lock_guard<std::mutex>my_guard1(my_mutex,std::adopt_lock);
				//my_mutex.lock();
				//my_mutex1.lock();
				std::lock(my_mutex, my_mutex1);
				msglist.push_back(i);
			}
			//my_mutex.unlock();
			//my_mutex1.unlock();
		}
	}

	bool out(int &commad)
	{
		//std::lock_guard<std::mutex>my_guard(my_mutex);
		
		 my_mutex.lock();
		 my_mutex1.lock();
		if (!msglist.empty())
		{
			 commad = msglist.front();
			msglist.pop_front(); 
			
			my_mutex1.unlock(); my_mutex.unlock();
			
				return true;
		}
		else
		{
			my_mutex1.unlock();
			my_mutex.unlock();
			return false;
		}
		
	}
	void outmsg()
	{
		int command = 0;
		for (int i = 0; i < 10000; i++)
		{
			bool result = out(command);
			if (result == true)
			{
				cout << "outmsg()执行，消息队列不为空" <<command << endl;
			}
			else
			{
				cout << "outmsg()执行，消息队列为空" <<endl;
			}
		}
		cout << "end" << endl;
	}
private:
	list<int>msglist;
	
	mutex my_mutex;//创建了一个互斥量
	mutex my_mutex1;
};
//互斥量的基本概念
//互斥量是个类对象，理解成一把锁，多个线程尝试用lock()函数来加锁这把锁头，只有一个线程能锁定成功
//成功的标志是函数返回了
//如果没锁成功，那么流程卡在lock()这里不断尝试区索这把锁头，
//std::lock_guard类模板。直接取代lock(),unlock(),用了lovk_guard就不用lock（）了；
int main()
{
	A myobj;
	std::thread myout(&A::outmsg, &myobj);
	
	std::thread myin(&A::inmsg, &myobj);
	myout.join();
	
	myin.join();
	return 0;
}
//死锁，至少两个互斥量才能产生
//std：：lock()函数模板
//能力：前提：两个以上一次索住两个或者多个互斥量
//不存在这种因为在多个线程中，因为锁的顺序问题导致死锁的风险问题，
