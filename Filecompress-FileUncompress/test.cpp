#pragma once
#include"FileCompress.h"

using namespace std;

void TestCompress()
{

	FileCompress<FileInfo> fc;

	int begin = GetTickCount();
	fc.Compress("Input.BIG");
	//fc.Compress("test.txt");
	int end = GetTickCount();
	cout << "Time Of Compress:" << end - begin << endl;
}
void TestUnCompress()
{
	FileCompress<FileInfo> fc;

	int Unbegin = GetTickCount();
	fc.UnCompress("Input.BIG");
	//fc.UnCompress("test.txt");
	int Unend = GetTickCount();
	cout << "Time Of UnCompress:" << Unend - Unbegin << endl;
}

int main()
{
	//TestHeap();
	//TestHuffman();
	//TestCompress();
	TestUnCompress();
	return 0;
}