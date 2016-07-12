#include"HuffmanTree.h"
#include<windows.h>
#include<assert.h>

typedef long long LongType;

struct FileInfo
{
	unsigned char _ch;
	LongType _count;
	string _code;

	FileInfo(unsigned char ch= 0)
		:_ch(ch)
		, _count(0)
	{}
	FileInfo operator+(FileInfo& fi)
	{
		FileInfo tmp;
		tmp._count = _count + fi._count;
		return tmp;
	}
	bool operator<(FileInfo& fi)
	{
		return _count < fi._count;
	}
	bool operator != (const FileInfo& fi) const
	{
		return _count != fi._count;
	}
};

template<class T>
class FileCompress
{
public:
	FileCompress()
	{
		for (int i = 0; i < 256; ++i)
			_infos[i] = i;
	}
public:
	bool Compress(const char* filename)
	{
		//1、读取文件中字符出现的次数
		long long charCount = 0;            //文件中所有字符个数
		assert(filename);
		FILE* fOut = fopen(filename, "rb");
		assert(fOut);

		char ch = fgetc(fOut);

		while (ch != EOF)
		{
			_infos[(unsigned char)ch]._count++;
			ch = fgetc(fOut);
			charCount++;
		}
		//构建HuffmanTree
		GenerateHuffmanCode();
		
		//压缩文件
		string compressFile = filename;
		compressFile += ".compress";
		FILE* fwCompress = fopen(compressFile.c_str(), "wb");

		fseek(fOut, 0, SEEK_SET);//基准：SEEK_SET->0 SEEK_CUR->1 SEEK_END->2
		ch = fgetc(fOut);
		char inch = 0;
		int index = 0;
		while (ch != EOF)
		{
			string& code = _infos[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); ++i)
			{
				inch = inch << 1;
				if (code[i] == '1')
					inch |= 1;
				if (++index == 8)
				{
					fputc(inch, fwCompress);
					inch = 0;
					index = 0;
				}
			}
			ch = fgetc(fOut);
		}
		if (index)
		{
			inch = inch << (8 - index);
			fputc(inch, fwCompress);
		}
		//配置文件
		string configFile = filename;
		configFile += ".config";
		FILE *fconfig = fopen(configFile.c_str(), "wb");
		assert(fconfig);

		char CountStr[128];
		_itoa(charCount , CountStr, 10);//charCount--->CountStr  10进制
		fputs(CountStr, fconfig);
		fputc('\n', fconfig);

		FileInfo invalid;
		for (int i = 0; i < 256; ++i)
		{
			if (_infos[i] != invalid)
			{
				_itoa(_infos[i]._count,CountStr, 10);
				fputc(_infos[i]._ch, fconfig);
				fputc(',', fconfig);
				fputs(CountStr, fconfig);
				fputc('\n', fconfig);
			}
		}

		fclose(fOut);
		fclose(fwCompress);
		fclose(fconfig);

		return true;
	}

	bool UnCompress(const char* filename)
   {
		string configfile = filename;
		configfile += ".config";
		FILE* outConfig = fopen(configfile.c_str(), "rb");
		assert(outConfig);

		char ch;
		long long charCount = 0;
		string line;
		ReadLine(outConfig,line);
		charCount = atoi(line.c_str());
		line.clear();

		while (ReadLine(outConfig,line))
		{
			if (!line.empty())
			{
				ch = line[0];
				_infos[(unsigned char)ch]._count = atoi(line.substr(2).c_str());//从2开始复制子字符串
				line.clear();
			}
			else
				line += '\n';
		 }
	
		FileInfo invalid;
		HuffmanTree<FileInfo> ht(_infos, 256, invalid);

		HuffmanNode<FileInfo>* root = ht.GetRootNode();

		string  UnCompressFile = filename;
		UnCompressFile += ".uncompress";
		FILE* fOut = fopen(UnCompressFile.c_str(), "wb");
		
		string CompressFile = filename;
		CompressFile += ".compress";
		FILE* fIn = fopen(CompressFile.c_str(), "rb");

		int pos = 8;
		HuffmanNode<FileInfo>* cur = root;
		ch = fgetc(fIn);
		
		while ((unsigned char)ch != EOF)
		{

			if (cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch, fOut);
				cur = root;
				if (--charCount == 0)
					break;
			}
			--pos;
			if ((unsigned char)ch &(1 << pos))
				cur = cur->_right;
			else
				cur = cur->_left;

			if (pos == 0)
			{
				ch = fgetc(fIn);
				pos = 8;
			}
		}
		fclose(outConfig);
		fclose(fIn);
		fclose(fOut);
		return true;
	}

protected:
	bool ReadLine(FILE* file, string &line)
	{
		char ch = fgetc(file);
		if (ch == EOF)
			return false;
		while (ch != EOF&&ch != '\n')
		{
			line += ch;
			ch = fgetc(file);
		}
		return true;
	}
	void GenerateHuffmanCode()
	{
		FileInfo invalid;
		HuffmanTree<FileInfo> hft(_infos, 256, invalid);
		_GenerateHuffmanCode(hft.GetRootNode());
	}
	void _GenerateHuffmanCode(HuffmanNode<FileInfo>* root)
	{
		if (root == NULL)
			return;
		_GenerateHuffmanCode(root->_left);
		_GenerateHuffmanCode(root->_right);
		if (root->_left == NULL && root->_right == NULL)
		{
			HuffmanNode<FileInfo>* cur = root;
			HuffmanNode<FileInfo>* parent = cur->_parent;
			string& code = _infos[cur->_weight._ch]._code;

			while (parent)
			{
				if (parent->_left == cur)
					code += '0';
				else if (parent->_right == cur)
					code += '1';
				cur = parent;
				parent = cur->_parent;
			}
			reverse(code.begin(), code.end());
		}
	}
private:
	FileInfo _infos[256];
};