#include<thread>
#include <iostream>
#include<string>
//std::ref函数
using namespace std;
//成员函数指针做线程函数
class A
{
public:
int m_i;
	A(int a) :m_i(a) { cout << "调用了构造函数" <<" "<<std::this_thread::get_id()<< endl; }
	A(const A& aa) :m_i(aa.m_i) { cout << "调用拷贝构造函数" << " " << std::this_thread::get_id() <<endl; }
	~A() { cout << "析构函数" << " " << std::this_thread::get_id <<endl; }
	void thread_work(int num)
	{
		cout << "子线程执行了" << endl;
	}
};
void myprint(const int &i, A& pmybuf)//这么写感觉OK，，string &pmybuf，但是还是有问题，什么时候隐士转换的呢，如果在主线程中就隐私转换，就不会影响子线程。
{//事实上存在，主线程执行完了，还没有隐士转换的可能性
	pmybuf.m_i = 199;
	cout << i << endl;//分析认为i并不是mvar的引用，实际上是值传递，可能是拷贝的引用，所以即便主线程结束了，子线程的i还是可以运行的
	//但是不推荐用引用
	cout << &pmybuf << endl;//指针再detach子线程时，绝对有问题，因为传进来的指针指向的是主线成中的地址
	//cout << pmybuf.c_str()<< endl;
	cout << "子线程ID" << std::this_thread::get_id()<<endl;
}
int main()
{
  //一传递临时对象作为参数
	//要避免的陷阱
	/*
	A aa(10);
	int mvar = 1;
	int &myvar = mvar;
	char mybuf[] = "this a test!";
	thread myobj(myprint, mvar, std::ref(aa));//这里我们直接将mybuf转换成stirng临时对象，这是一个可以保证线程中用的肯定有效的方法
	myobj.join();
	cout << aa.m_i << endl;//std::ref(aa)就是让thread不调用拷贝构造，即，传的是真的引用，然而，形参不要带const，主要作用是传入真正的引用，可以修改主函数的值
	cout << "主线程ID" << std::this_thread::get_id() << endl;
	*/


	//成员函数指针做线程函数
	A my(10);
	std::thread myobj(&A::thread_work, my, 15);//第一个参数是成员函数地址，第二个参数是类对象，后面是成员函数参数
	myobj.join();
	
	return 0;
}

