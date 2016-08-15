//#include"BigData.h"

#include<iostream>
#include"BigData.h"
using namespace std;

void Test()
{
	BigData b1(" ");
	cout <<"b1:"<< b1 << endl;
	BigData b2("+");
	cout << "b2:"<<b2 << endl;
	BigData b3("123");
	cout << "b3:"<<b3 << endl;
	BigData b4("00012");
	cout << "b4:"<<b4 << endl;
	BigData b5("+12");
	cout << "b5:"<<b5 << endl;
	BigData b6("qwer123");
	cout << "b6:"<<b6 << endl;
	BigData b7("1234qwe");
	cout << "b7:"<<b7 << endl;
	BigData b8("-1234");
	cout << "b8:"<<b8 << endl;
}
void TestAdd()
{
	BigData left(1234);
	BigData right(4321);
	cout << "left + right:"<<left + right << endl;

	BigData left1(9223372036854775807);
	BigData right1(2);
	cout << "left1 + right1:" << left1 + right1 << endl;

	//BigData left2("-9223372036854775808");
	BigData left2(MIN_INT64);
	BigData right2("-2");
	cout << "left2 + right2:" << left2 + right2 << endl;

	BigData left3("11111111111111111111111111111");
	BigData right3("99999999999999999999999999999");
	cout << "left3 + right3:" << left3 + right3 << endl;
}
void TestSub()
{
	BigData left(1234);
	BigData right(34);
	cout << "left-right:" << left - right << endl;

	BigData left1(9223372036854775807);
	BigData right1(-999);
	cout << "left1-right1:" << left1 - right1 << endl;

	BigData left2("-9223372036854775808");
	BigData right2(999);
	cout << "left2-right2:" << left2 - right2 << endl;

	BigData left3("11111111111111111111111111111");
	BigData right3("99999999999999999999999999999");
	cout << "left3 - right3:" << left3 - right3 << endl;
}
void TestMul()
{
	BigData left(1234);
	BigData right(34);
	cout << "left*right:" << left * right << endl;

	BigData left1(9223372036854775807);
	BigData right1(-999);
	cout << "left1*right1:" << left1 * right1 << endl;

	//BigData left2("-9223372036854775808");
	BigData left2(MIN_INT64);
	BigData right2(999);
	cout << "left2*right2:" << left2 * right2 << endl;

	//BigData left2("10000000000000001");
	//BigData right2(999);
	//cout << "left2*right2:" << left2 * right2 << endl;

	//BigData left2("99");
	//BigData right2(999);
	//cout << "left2*right2:" << left2 * right2 << endl;

	BigData left3("11111111111111111111111111111");
	BigData right3("99999999999999999999999999999");
	cout << "left3 * right3:" << left3 * right3 << endl;
}
void TestDiv()
{
	BigData left(1234);
	BigData right(34);
	cout << "left/right:" << left / right << endl;

	BigData left1(9223372036854775807);
	BigData right1(-999);
	cout << "left1/right1:" << left1 / right1 << endl;

	BigData left2("-9223372036854775808");
	BigData right2(999);
	cout << "left2/right2:" << left2 / right2 << endl;

	BigData left3("222222222222222222222222222222");
	BigData right3("33");
	cout << "left3/right3:" << left3 / right3 << endl;
}
int main()
{
//	Test();
//	TestAdd();
//	TestSub();
//	TestMul();
	TestDiv();

	return 0;
}