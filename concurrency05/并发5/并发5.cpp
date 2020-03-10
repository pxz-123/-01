
#include <iostream>
#include<future>
#include<vector>
using namespace std;
class A
{
public:
	int mythread(int mypar)
	{
		cout << mypar << endl;
		cout << "start" << "thread id=" << std::this_thread::get_id() << endl;
		std::chrono::microseconds dura(50000);//定义5秒
		std::this_thread::sleep_for(dura);//休息一定屎长
		cout << "end" << "thread id=" << std::this_thread::get_id() << endl;
		cout << "continue.....!" << endl;
		cout << "continue.....!" << endl;
		cout << "continue.....!" << endl;
		cout << "continue.....!" << endl;
		return 5;
	}

};
int mythread1(int mypar)
{
	cout << mypar << endl;
	cout << "start" << "thread id=" << std::this_thread::get_id() << endl;
	std::chrono::microseconds dura(50000);//定义5秒
	std::this_thread::sleep_for(dura);//休息一定屎长
	cout << "end" << "thread id=" << std::this_thread::get_id() << endl;
	cout << "continue.....!" << endl;
	cout << "continue.....!" << endl;
	cout << "continue.....!" << endl;
	cout << "continue.....!" << endl;
	return 5;
}
vector<std::packaged_task<int(int)>>mytasks;
void mythread2(std::promise<int>&tmpp,int calc)
{
	//做一系列复杂的操作
	calc++;
	calc *= 10;
	std::chrono::milliseconds dura(5000);
	std::this_thread::sleep_for(dura);
	int result = calc;
	tmpp.set_value(result);//结果保存在对象中
}
int main()
{
	//一：std::async,std::future创建后台任务并返回值
	//希望线程返回一个结果
	//std::async是个函数模板，用来启动一个异步任务之后，启动一个异步任务之后，他返回一个std::future对象
	//std::future是个类模板
	//什么叫启动一个异步任务，就是自动创建一个线程并开始执行对应得线程入口函数，它返回一个std::future对象
	//std::future对象里面含有线程入口函数所返回得结果（线程返回得结果），我们可以通过调用future对象得成员函数get()函数来获取结果
	//我们通过额外向std::async()传递一个参数，该参数类型是std::lunnch类型（枚举类型）来达到一些特殊目的
	//std::launch::deferred:表示线程入口函数调用被延迟到std::future的wait或者get函数时再执行（不创建线程，只是主线程，调用函数）
	//std::launch::async:表示再调用async函数的时候就开始创建线程(立即执行不会等到wait或者get)
	//系统默认是std::launch::async标记
	//如果wait或者get没有被调用，那么线程会执行吗？不会，实际上线程都没有创建
	A a;
	int tmppar = 12;
	std::cout << "main" << "threadid=" << std::this_thread::get_id() << endl;
	std::future<int>result = std::async(std::launch::async,&A::mythread,&a,tmppar);//创建一个线程并开始执行，绑定关系，流程并不卡再这
	                                                            //第二个参数是对象引用，才能保证线程里用得是同一个对象
	//std::launch::deferred不管有没有get或者wait都不创建线程，只是调用
	int def = 0;
	cout << "I LOVE CHINA" << endl;
	cout << "I LOVE CHINA" << endl;
	cout << "I LOVE CHINA" << endl;
	cout << "I LOVE CHINA" << endl;
	cout << "I LOVE CHINA" << endl;
	cout << "I LOVE CHINA" << endl;
	cout << result.get() << endl;//只能执行一次
	//result.wait();//等待线程返回，但是不返回结果；
	cout << "I LOVE CHINA" << endl;

	cout << "std::packaged_task" << endl;
	std::packaged_task<int(int)>mypt(mythread1);//<返回值（参数）>我们把函数包装起来
	//std::thread t1(std::ref(mypt), 1);
	std::packaged_task<int(int)>mypt1([](int mypar)->int{
		cout << mypar << endl;
		cout << "start" << "thread id=" << std::this_thread::get_id() << endl;
		std::chrono::microseconds dura(50000);//定义5秒
		std::this_thread::sleep_for(dura);//休息一定屎长
		cout << "end" << "thread id=" << std::this_thread::get_id() << endl;
		return 5;
	});
	mytasks.push_back(std::move(mypt1));//入容器，这里用了移动语义，入进去之后，mypt1为空
	std::packaged_task<int(int)>mypt2;
	auto iter = mytasks.begin();
	mypt2 = std::move(*iter);
//	std::thread t2(std::ref(mypt1), 1);
	//t2.join();
	mypt1(502);
	std::future<int>result1 = mypt1.get_future();
	cout << result1.get() << endl;
	std::promise<int>myprom;//声明一个std::promis对象，保存的值类型为int
	//总结：通过promise保存一个值，在将来某个时刻我们通过把一个future绑定到这个promise
	std::thread t3(mythread2, std::ref(myprom), 180);
	t3.join();
	std::future<int>fu11 = myprom.get_future();
	auto reslut = fu11.get();
	cout << "result=" << reslut << endl;

	return 0;
}
//二：std::packaged_task:打包任务，把任务打包起来
//是个类模板。它的模板参数是，各种可调用对象；通过std::packaged_task来把各种可调用对象封装起来方便将来作为线程入口函数使用
//三：std::promise，类模板
//我们能够再某个线程中给他赋值，然后我们再其他线程把这个值取出来
