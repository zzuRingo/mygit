 ///
 /// @file    io1.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2017-02-20 15:26:31
 ///
 
#include <iostream>
#include <limits>
using std::cout;
using std::endl;
using std::cin;


void printCin()
{
	cout << "cin's badbit = " << cin.bad() << endl;
	cout << "cin's failbit = " << cin.fail() << endl;
	cout << "cin's eofbit = " << cin.eof() << endl;
	cout << "cin's goodbit = " << cin.good() << endl;
}

int main(void)
{
	int num;
	printCin();
	while(cin >> num) {
		cout << "num = " << num << endl;
	}
	printCin();
	cout << endl;
	cin.clear();//重置状态
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//清空缓冲区
	printCin();
	cout << endl;

	cin >> num;
	printCin();

	return 0;
}


