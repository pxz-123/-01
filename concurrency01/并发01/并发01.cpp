/*
1.0:thread
1.1:join()
1.2:detach()
1.3:joinable()
2.0:其他创建线程的手段
2.1：用类，以及一个问题范例
2.2用lambda表达式
*/
#include<thread>
#include <iostream>
using namespace std;
void mythread01()
{
	cout << "我的线程1开始执行了" << endl;
	cout << "我的线程2执行完毕了" << endl;
	cout << "我的线程3执行完毕了" << endl;
	cout << "我的线程4执行完毕了" << endl;
	cout << "我的线程5执行完毕了" << endl;
	cout << "我的线程6执行完毕了" << endl;
	cout << "我的线程7执行完毕了" << endl;

}
/*
int main()
{
  //一范例演示线程的结束和开始
	//线程从一个函数开始，一旦这个函数运行完毕，就代表着我们这个线程运行结束
	//整个进程是否执行完毕的标志是，主线程是否执行完毕。

	std::thread myobj01(mythread01);
	cout << "aa" << endl;
	myobj01.join();
   cout << "主线程" << endl;
	return 0;
}
*/
/*

int main()
{
	//detach分离,一旦detach以后不能join了可以用joinable，一个线程要么用detach要么用join
	thread myobj(mythread01);
	myobj.detach();
	if (myobj.joinable())
	{
		cout << "还可以join或者detach" << endl;
	}
	else
	{
		cout << "不能了" << endl;
	}
	cout << "主线程" << endl;
	return 0;
}
*/
//其他创建线程的方法01
class TA
{
public:
	int m_i;
	TA(int i) :m_i(i) { cout << "调用了构造函数" << endl; };
	void operator()()
	{
		cout << "类-线程" <<m_i<< endl;
		cout << "类-线程" << m_i << endl;
		cout << "类-线程" << m_i << endl;
		cout << "类-线程" << m_i << endl;
		
	}
	TA(const TA &a)
	{
		this->m_i = a.m_i;
		cout << "调用了拷贝构造" << endl;
	}
	~TA()
	{
		cout << "调用了析构函数" << endl;
	}
};
int main()
{
	//类创建对象
	int s = 9;
	TA ta(s);//注意s可能再线程执行时被销毁，就会产生不可预料的后果，不能用引用传递，
	std::thread myobj(ta);
	
	myobj.detach();
	cout << "主函数" << endl;
	//lambda表达式
	//std::thread myobj02([] {cout << "a" << endl; });
	auto mylam = [] {
		cout << "aaaa" << endl; 
	cout << "aaaa" << endl;
	cout << "aaaa" << endl;
	cout << "aaaa" << endl;
	
	};
	thread myobj02(mylam);
	cout << "bb" << endl;
	cout << "bb" << endl;
	cout << "bb" << endl;
	cout << "bb" << endl;
	cout << "bb" << endl;
	cout << "bb" << endl;
	cout << "bb" << endl;

	myobj02.join();
	//myobj02.join();
	return 0;
}

