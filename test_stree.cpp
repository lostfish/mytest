#include <iostream>
#include <string>
//#include "suffix_tree.h"
#include "suffix_tree_w.h"

using namespace std;

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

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "en_US.utf8");
	string s("吃葡萄不吐葡萄皮");
	//string s("mississippi");
	//cout << "Input a string to buid suffix tree:\n";
	//while(getline(cin,s)){
		wstring wstr;
		convert_to_wstring(s.c_str(), wstr, "en_US.utf8");
		SuffixTree stree(wstr);
		stree.build_tree();
		cout << "The suffix tree is:\n";
		stree.print_tree();
		//cout << "Input a string to buid suffix tree:\n";
	//}
	return 0;
}
