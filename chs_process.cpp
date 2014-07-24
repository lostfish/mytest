#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

int convert_to_string(const wchar_t * wstr, string &out_str,const char *encoding){
	if (!wstr)
		return 1;
	char* str = NULL;
	int size = 0;
	string loc = setlocale(LC_ALL,NULL);
	setlocale(LC_ALL, encoding);
	size = wcstombs( NULL, wstr, 0);
	str = new char[size + 1];
	wcstombs(str, wstr, size);
	str[size] = '\0';
	out_str = str;
	setlocale(LC_ALL, loc.c_str());
	return 0;
}

int convert_to_wstring(const char* str, wstring &out_wstr, const char *encoding){
	if (!str)
		return 1;
	wchar_t* wcs = NULL;
	int size = 0;
	string loc = setlocale(LC_ALL,NULL);
	setlocale(LC_ALL, encoding);
	size = mbstowcs(NULL,str,0);
	cout << "size:" << size << endl;
	wcs = new wchar_t[size+1];
	size = mbstowcs(wcs, str, size+1);
	wcs[size] = 0;
	out_wstr = wcs;
	delete[] wcs;
	setlocale(LC_ALL, loc.c_str());
	return 0;
}

/**
 * @brief get the character bytes
 *
 * @param[in] src: input string
 *
 * @return len: the character bytes
 */
int get_char_length(const char* str)
{
	if (!str)
		return 0;
	unsigned char mask = 0x80;
	if (!(str[0] & mask)) //ASCII
		return 1;
	int len = 0;
	while (str[0] & mask)
	{
		len++;
		mask = mask >> 1;
	}
	return len;
}


/**
 * @brief get the string size
 *
 * @param[in] src: input string
 *
 * @return the string size
 */
int get_string_size(const char* str)
{
	if (!str)
		return 0;
	int size = 0;
	int i = 0;
	int len = 0;
	while(*(str+i))
	{
		len = get_char_length(str+i);
		i += len;
		size++;
	}
	return size;
}

int main(int argc, char* argv[])
{
	//locale loc("zh_CN.gbk");
	//locale::global(loc);
	//setlocale(LC_ALL, "zh_CN.utf8");
	
	string s("cs专业。");
	cout << s << "(size:" << get_string_size(s.c_str()) << ")" << endl;;

	wstring wstr;
	convert_to_wstring(s.c_str(), wstr,"en_US.utf8");
	cout <<"wstring size:" << wstr.size() << endl;
	for(size_t i = 0; i < wstr.size(); ++i)
		cout << hex << wstr[i] << ' ';
	cout << dec << endl;

	string new_str;
	convert_to_string(wstr.c_str(), new_str, "en_US.utf8");
	cout << new_str << "(size:" << new_str.size() << ")" << endl;;

	return 0;
}
