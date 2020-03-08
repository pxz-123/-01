#include<thread>
#include <iostream>
#include<mutex>
using namespace std;
//static的作用主要有两种
//1：第一个作用事限定作用域
//2：第二个作用事保持变量内容的持久化
std::mutex my_mutex;
std::once_flag g_flag;
class A//这是一个单例类
{
public:
	static A *GetInstance()
	{//提高效率
		//if (m_instance == NULL)//双重锁定
		//{
			//std::unique_lock<std::mutex>mymutex(my_mutex);
			//if (m_instance == NULL)
			//{
				//m_instance = new A();
				//static huishou cl;
			//}
	//	}
		std::call_once(g_flag,createinstance);
		return m_instance;
	}
	static void createinstance()//只被调用一次的函数
	{
		m_instance = new A();
		static huishou cl;
	}
	class huishou//类中套类，用来释放对象//设计比较巧妙
	{
	public:
		~huishou()//类的析构函数中
		{
			if (m_instance != NULL)
			{
				delete A::m_instance;
				A::m_instance = NULL;

			}
		}
	};
	void test()
	{
		cout << "测试" << endl;
	}
private:
	A() {};//私有化构造函数
private:
	static A *m_instance;//静态成员变量
	int i;

};
//类静态变量初始化
A *A::m_instance = NULL;

void mythread()//线程入口函数
{
	cout << "我的线程开始执行了" << endl;
	A *p_aa = A::GetInstance();
	cout << "我的线程执行完毕了" << endl;

}
int main()
{
	//一：设计模式大概谈
	//“设计模式”：代码的一些写法（这些写法跟常规写法不怎么一样）：程序灵活，维护起来可能方便，但是别人接管和阅读代码都会很痛苦，
	//二：单例设计模式
	//单例设计模式，使用的平吕比较高；
	//单例：只能创建一个属于该类的对象
	//A *p_a = A::GetInstance();
	//p_a->test();
	//A::GetInstance()->test();
	//三：单例设计模式共享数据问题分析，解决
	//面临问题：需要再我们自己创建的线程中来创建这个单例类对象，我们可能面临getinstance函数要互斥
	std::thread myobj1(mythread);
	std::thread myobj2(mythread);
	myobj1.join();
	myobj2.join();
	return 0;
}

