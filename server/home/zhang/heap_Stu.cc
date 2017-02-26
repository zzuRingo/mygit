 ///
 /// @file    Student.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2017-02-20 14:40:09
 ///
 
#include <stdlib.h>
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

//只能生成堆对象

class Student
{
public:
	Student(int no, const char * name)
	: _no(no)
	, _name(new char[strlen(name) + 1])
	{
		strcpy(_name, name);
	}
	void print()
	{
		cout << "学号:" << _no << endl;
		cout << "姓名:" << _name << endl;
	}

	void destroy()
	{
		delete this;
	}

private:
	~Student()
	{
		cout << "~Student()" << endl;
		delete [] _name;
	}

private:
	int _no;
	char * _name;
};


int main(void)
{
	Student * s1 = new Student(100, "Mike");
	s1->print();
	//delete s1;
	s1->destroy();

	//Student s2(101, "Jackie");//不能通过编译

	return 0;
}
