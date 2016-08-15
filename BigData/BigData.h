//#pragma once 
//#include<iostream>
//using namespace std;

#ifndef BIG_DATA_H           //与#pragma once区别
#define BIG_DATA_H

#include<iostream>
using namespace std;
#include<string>
#include<assert.h>

#define UN_INIT 0xCCCCCCCCCCCCCCCC
#define MAX_INT64 0x7FFFFFFFFFFFFFFF
#define MIN_INT64 0x8000000000000000
typedef long long INT64;
//内置类型
class BigData
{
public:
	BigData(INT64 value=UN_INIT)
		:_value(value)
	{
		INT64ToString();
	}
	BigData(const char* pData)
	{
		//"-","+","+123456";"345qwer","1234567"," ","00001234"
		if (NULL == pData)
		{
			assert(false);
			return;
		}

		char *pStr = (char*)pData;
		char cSymbol = pData[0];
		if ('+' == cSymbol || '-' == cSymbol)
			pStr++;
		else if (cSymbol >= '0' && cSymbol <= '9')
			cSymbol = '+';
		else
			return;
		while (*pStr == '0')         //"0001234"
			pStr++;

		_strData.resize(strlen(pData)+1);
		_strData[0] = cSymbol;

		_value = 0;                        //"123qwe"
		int iCount = 1;
		while (*pStr>='0' && *pStr<='9')
		{
			//转化
			_value = _value * 10 + (*pStr - '0');
			_strData[iCount++] = *pStr;
			pStr++;
		}
		_strData.resize(iCount);

		if (cSymbol == '-')
			_value = 0 - _value;
	}
	BigData operator+(const BigData& bigData)
	{//1.溢出   2.没溢出(两个都没溢出,结果溢出；参数有溢出)
		if (NotINT64overflow() && bigData.NotINT64overflow())  //参数都不溢出
		{
			if (_strData[0] != bigData._strData[0])     //异号
			{
				return BigData(_value + bigData._value);
			}
			else      //同号且结果不溢出
			{   
				if ((_value >= 0 && MAX_INT64 - _value >= bigData._value) ||
					(_value < 0 && MIN_INT64 - _value >= bigData._value))
				{
					return BigData(_value + bigData._value);
				}
			}
		}
		//至少一个溢出   结果溢出
		if (_strData[0] == bigData._strData[0]) //同号且结果溢出
		{				
			return BigData(Add(_strData, bigData._strData).c_str());
		}
		return BigData(Sub(_strData, bigData._strData).c_str());
		
	}
	BigData operator-(const BigData& bigData)
	{
		if (NotINT64overflow() && bigData.NotINT64overflow())//都不溢出
		{
			if (_strData[0] == bigData._strData[0])//都不溢出且同号
				return BigData(_value - bigData._value);
			else //都不溢出且异号
			{
				if ((_value >= 0 && MAX_INT64 + bigData._value >= _value)||
					(_value<0 && MIN_INT64+bigData._value <=_value))
				{
					return BigData(_value - bigData._value);
				}
			}
		}
		if (_strData[0] != bigData._strData[0])
			return BigData(Add(_strData, bigData._strData).c_str());
		return BigData(Sub(_strData, bigData._strData).c_str());
		
	}
	BigData operator*(const BigData& bigData)
	{
		//1.两个都不溢出 结果不溢出   
		//2.两个中有一个溢出
		if (NotINT64overflow() && bigData.NotINT64overflow())//都不溢出
		{
			if (_value == 0 || bigData._value == 0)
				return BigData(INT64(0));
			if (_strData[0] == bigData._strData[0])//同号
			{    //结果不溢出
				if ((_value > 0 && MAX_INT64 / _value >= bigData._value) ||
					(_value < 0 && MAX_INT64 / _value <= bigData._value))
					return BigData(_value*bigData._value);
			}
		}
		else
		{
			//结果不溢出
			if ((_value > 0 && MIN_INT64 / _value >= bigData._value) ||					
				(_value < 0 && MIN_INT64 / _value >= bigData._value))
				return BigData(_value* bigData._value);
		}
		return BigData(Mul(_strData,bigData._strData).c_str());
	}
	BigData operator/(const BigData& bigData)
	{
		//1.除数不能为0  两个都没溢出，直接除
		//1)L<R=0 2)R=+-1 3)L==+-R 4)其他
		if (bigData._strData[1] == '0')  //除数为零
		{
			assert(false);
		}
		if (NotINT64overflow() && bigData.NotINT64overflow())//都不溢出
			return BigData(_value / bigData._value);
		if ((_strData.size() < bigData._strData.size()) ||
			(_strData.size() == bigData._strData.size() &&
			strcmp(_strData.c_str() + 1, bigData._strData.c_str() + 1) < 0))//被除数<除数
			return BigData(INT64(0));

		if (bigData._strData == "+1" || bigData._strData == "-1")//除数==+-1
		{
			string ret = _strData;
			if (_strData[0] != bigData._strData[0])
			{
				ret[0] = '-';
			}
			return BigData(ret.c_str());
		}
		if (/*_strData.size() == bigData._strData.size() &&*/
			strcmp(_strData.c_str() + 1, bigData._strData.c_str() + 1) == 0)//被除数==除数
		{
			string ret = "+1";
			if (_strData[0] != bigData._strData[0])
				ret[0] = '-';
			return BigData(ret.c_str());
		}

		return BigData(Div(_strData, bigData._strData).c_str());
	}

private:
	string Add(string left, string right)
	{
		int iLSize = left.size();
		int iRSize = right.size();
		if (iLSize < iRSize)
		{
			swap(left, right);
			swap(iLSize, iRSize);
		}
		string sRet;
		sRet.resize(iLSize + 1);
		sRet[0] = left[0];
		char step = 0;

		for (int index = 1; index<iLSize; ++index)
		{
			char cRet = left[iLSize-index]-'0'+step;
			if (index < iRSize)
			{
				cRet += (right[iRSize - index] - '0');
			}
			
			sRet[iLSize-index+1] = cRet % 10+'0';
			step = cRet / 10;
		}
		sRet[1] += step+'0';

		return sRet;
	}
	string Sub(string left, string right)
	{//1.都不溢出（同号 直接返回；异号 正-负，负-正）
	 //2.至少有一个溢出  结果溢出
		int iLSize = left.size();
		int iRSize = right.size();
		char cSymbol = left[0];
		if (iLSize < iRSize || (iLSize==iRSize && left<right))
		{
			swap(left, right);
			swap(iLSize, iRSize);
			if (cSymbol == '+')
				cSymbol = '-';
			else
				cSymbol = '+';
		}

		//保存结果
		string sRet;
		sRet.resize(left.size());
		sRet[0] = cSymbol;
		//1.低到高 取left
		//2.低到高，取right
		//3.减   判断是否借位
		for (int index = 1; index < iLSize; ++index)
		{
			char cRet = left[iLSize - index] - '0';
			if (index<iRSize)
				cRet -= (right[iLSize - index] - '0');

			if (cRet < 0)
			{
				left[iLSize - index - 1] -= 1;
				cRet += 10;
			}
			sRet[iLSize - index] = cRet + '0';
		}
		return sRet;
	}
	string Mul(string left, string right)
	{
		char cSymbol = '+';
		if (left[0] != right[0])
			cSymbol = '-';

		int iLSize = left.size();
		int iRSize = right.size();
		if (iLSize > iRSize)
		{
			swap(left, right);
			swap(iLSize, iRSize);
		}

		string sRet;
		//sRet.resize(iLSize + iRSize-1);
		sRet.assign(iLSize+iRSize-1,'0');
		sRet[0] = cSymbol;
		int iDataLen = sRet.size();
		int ioffset = 0;

		for (int iL = 1; iL < iLSize; ++iL)
		{
			char cL = left[iLSize - iL] - '0';
			char cStep = 0;
			if (cL == 0)
			{
				ioffset++;
				continue;
			}

			for (int iR = 1; iR < iRSize; ++iR)
			{
				char cR = cL*(right[iRSize - iR] - '0')+ cStep;
				cR += sRet[iDataLen - iR - ioffset] - '0';
				sRet[iDataLen - iR -ioffset] = (cR % 10)+'0';
				cStep = cR / 10;
			}
			sRet[iDataLen - iRSize -ioffset] += cStep;
			ioffset++;
		}

		return sRet;
	}
	string Div(string left, string right)
	{
		string sRet;
		sRet.append(1, '+');
		if (left[0] != right[0])
		{
			sRet[0] = '-';
		}
		char* pLeft = (char*)(left.c_str()+1);
		char* pRight = (char*)(right.c_str() + 1);
		int LSize = left.size() - 1;
		int DataLen = right.size() - 1;

		for (int index = 0; index < left.size() - 1;)
		{
			if (*pLeft == '0')
			{
				sRet.append(1, '0');
				pLeft++;
				index++;
				continue;
			}
			if (!IsLeftBig(pLeft, DataLen, pRight, right.size()-1))
			{
				sRet.append(1, '0');
				DataLen++;
				if (DataLen + index > LSize)
					break;
			}
			else
			{
				sRet.append(1,SubLoop(pLeft, DataLen, pRight, right.size()-1));
				while(*pLeft=='0' && DataLen>0)
				{
					pLeft++;
					index++;

					DataLen--;
				}
				DataLen++;
				if (DataLen + index > LSize)
					break;
			}
		}
		
		return sRet;
	}

	bool NotINT64overflow() const;
	void INT64ToString();
	bool IsLeftBig(const char* pLeft, int LSize, const char* pRight, int RSize);
	char SubLoop( char* pLeft, int LSize, const char* pRight, int RSize);

	friend ostream& operator<<(ostream& _count, const BigData& bigData);

private:
	INT64 _value;
	string _strData;
};
ostream& operator<<(ostream& _cout, const BigData& bigData)
{
	if (bigData.NotINT64overflow())
	{
		_cout << bigData._value << endl;
	}
	else
	{
		char *pData = (char*)bigData._strData.c_str();
		if (pData[0] == '+')
			++pData;
		_cout << pData << endl;
	}
	return _cout;
}
bool BigData::NotINT64overflow() const
{
	//7fffffffffffffff     9223372036854775807
	string tmp("+9223372036854775807");
	if (_strData[0] == '-')
	{
		tmp = "-9223372036854775808";
	}
	if (_strData.size() < tmp.size())
		return true;
	else if ((_strData.size() == tmp.size()) && _strData <= tmp)
		return true;

	return false;
}
void BigData::INT64ToString()
{
	INT64 tmp = _value;
	char cSymbol = '+';
	if (tmp < 0)
		cSymbol = '-';

	_strData.append(1,cSymbol);                     //追加
	while (tmp)
	{
		int c = tmp % 10;
		if (c < 0)
			c = 0 - c;
		_strData.append(1, c + '0');
		tmp /= 10;
	}

	char *pleft = (char*)_strData.c_str()+1;
	char *pright = (char*)pleft + _strData.size()-2;

	while (pleft < pright)
	{
		char cTemp = *pleft;
		*pleft = *pright;
		*pright = cTemp;
		pleft++;
		pright--;
	}
}
bool BigData::IsLeftBig(const char* pLeft, int LSize, const char* pRight, int RSize)
{
	if (LSize > RSize || (LSize == RSize && strcmp(pLeft, pRight) >= 0))
		return true;

	return false;
}
char BigData::SubLoop( char* pLeft, int LSize, const char* pRight, int RSize)
{
	assert(pLeft != NULL && pRight != NULL);
	char cRet = '0';
	while (true)
	{
		if (!IsLeftBig(pLeft, LSize, pRight, RSize))
			break;

		int LDataLen = LSize-1;
		int RDataLen = RSize - 1;
		while (LDataLen>=0 && RDataLen>=0)
		{
			char ret = pLeft[LDataLen]-'0';
			ret -= pRight[RDataLen] - '0';
			if(ret < 0)
			{
				pLeft[LDataLen - 1] -= 1;
				ret += 10;
			}
			pLeft[LDataLen] = ret +'0';	
			LDataLen--;
			RDataLen--;
		}

		while (*pLeft == '0' && LSize > 0)
		{
			pLeft++;
			LSize--;
		}

		cRet++;
	}
	return cRet;
}

#endif