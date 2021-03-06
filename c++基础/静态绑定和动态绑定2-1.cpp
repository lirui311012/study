#include <iostream>
#include <typeinfo>
using namespace std;

/*
总结一：
一个类里面定义了虚函数，那么“编译阶段”(注意不是运行，是编译哦)，编译器给这个类类型产生一个唯一
的vftable虚函数表，虚函数表中主要存储的内容就是RTTI(tun-time ttype information运行时的类型信息)
指针和虚函数的地址。当程序运行时，每一张虚函数表都会加载到内存的.rodata区(常量区)。
下面就是虚函数表：以下面的Base类为例子
|----------------|
|	&RTTI        |   ---> 可以理解为指向类型字符串，谁产生的这个表，就存那个类型 如 "Base"
|----------------|
|	  0  	     |
|----------------|   <<<<<<--------start
|&Base::show()   |   --->虚函数的地址
|----------------|
|&Base::show(int)|
|----------------|

总结二：
一个类里面定义了函数数，那么这个类定义的对象，其运行时，内存中开始部分，多存储一个vfptr虚函数指针，
指向相应类型的虚函数表vftable。一个类型定义的n个对象，它们的额vfptr指向的都是《《同一张虚函数表》》
对象的前4个字节是一个指针vfptr，该指针指向虚函数表的虚函数的起始位置。即上面的图中的<<<<=====start位置
Base b1;  //sizeof(Base) == 8字节
Base b2;

总结三：
一个类里面虚函数的个数，不影响对象内存大小（vfptr），影响的是虚函数表的大小


*/

class Base
{
public:
	Base(int data = 10) :ma(data) {}

	 virtual void show() { cout << "Base::show()" << endl; }

	 virtual void show(int) { cout << "Base::show(int)" << endl; }
protected:
	int ma;
};
class Derive : public Base
{
public:
	Derive(int data = 20) :Base(data), mb(data) {}

	/*
	总结四：
	如果派生类中的方法，和基类继承来的某个方法，返回值、函数名、参数列表都相同，
	而且基类的方法是virtual虚函数，那么派生类的这个方法，自动处理成虚函数
	重写《=》覆盖
	*/
	//这叫左覆盖的关系
	/*virtual*/ void show() { cout << "Derive::show()" << endl; }
private:
	int mb;
};
//Derive类，由于从Base继承过来了虚函数，所以编译阶段也会生成虚函数表
/*Derive的虚函数表
|----------------|
|	&RTTI        |   ---> 可以理解为指向类型字符串，谁产生的这个表，就存那个类型 如 "Derive"
|----------------|
|	  0  	     |
|----------------|   <<<<<<--------start
|&Base::show()   |   --->虚函数的地址
|----------------|
|&Base::show(int)|
|----------------|
由于派生对对于void show()函数进行重写，所以虚函数表变化为如下：
|----------------|
|	&RTTI        |   ---> 可以理解为指向类型字符串，谁产生的这个表，就存那个类型 如 "Derive"
|----------------|
|	  0  	     |
|----------------|   <<<<<<--------start
|&Derive::show() |   --->虚函数的地址
|----------------|
|&Base::show(int)|
|----------------|
所以==》覆盖指的是，虚函数表中，虚函数地址的覆盖
Derive d1;
Derive d2; //sizeof(Derive) == 12
前四个字节就是vfptr指针，指向虚函数表

*/

int main()
{
	Derive d(50);
	Base* pb = &d;
	pb->show();  //Derive::show()


	pb->show(10); //Base::show(int)


	cout << sizeof(Base) << endl;   //8
	cout << sizeof(Derive) << endl;  //12

	cout << typeid(pb).name() << endl;   //class Base *
	
	cout << typeid(*pb).name() << endl;    //class Derive

	return 0; 
}