#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <fstream>
#include <time.h> //clock()
#include <string.h> //memset
#include <math.h> //log
#include <stdlib.h> //atof, atoi

using namespace std;


int convert_to_string(const wchar_t * wstr, string &out_str){ 
	if (!wstr)
		return 1;
	char* str = NULL; 
	size_t size = 0; 
	string loc = setlocale(LC_ALL,NULL);
	setlocale(LC_ALL, "zh_CN.utf8"); 
	size = wcstombs( NULL, wstr, 0); 
	str = new char[size + 1]; 
	wcstombs(str, wstr, size); 
	str[size] = '\0'; 
	out_str = str;
	delete[] str;
	setlocale(LC_ALL, loc.c_str()); 
	return 0; 
}

///
struct WordInfo{
	string word;
	vector<wchar_t> leftchars;
	vector<wchar_t> rightchars;
	double alpha; //自由程度
	double beta; //内聚程度
	size_t freq; //词出现频率
	double prob; //词出现概率
	bool choosed;

	WordInfo():word(""),alpha(0.0),beta(0.0),freq(0),prob(0.0),choosed(true){}
	void print_info(){
		cout << word << '\t' << alpha << '\t' << beta << '\t' << freq << '\t' << prob << endl;
	}
};

template<typename T>
double calc_entropy(const vector<T> &vec){
	map<T,int> count_map;
	for(size_t i = 0; i < vec.size(); ++i)
	{
		if (count_map.find(vec[i]) != count_map.end())
			count_map[vec[i]] += 1;
		else
			count_map.insert(typename map<T,int>::value_type(vec[i],1));
	}
	size_t n = vec.size();
	double entropy = 0.0;
	double prob = 0.0;
	typename map<T,int>::iterator itr = count_map.begin();
	for( ;itr != count_map.end(); ++itr)
	{
		prob = double(itr->second)/n;
		entropy += -prob*log(prob)/log(2.0);
	}
	return entropy;
}

void calc_alpha(map<string, WordInfo> &resultMap)
{
	map<string, WordInfo>::iterator itr;
	double word_count = resultMap.size();
	itr =  resultMap.begin();
	for( ; itr != resultMap.end(); ++itr)
	{
		WordInfo *cur_info = &(itr->second);
		cur_info->prob = cur_info->freq/word_count;	
		double x =  calc_entropy(cur_info->leftchars);
		double y =  calc_entropy(cur_info->rightchars);
		cur_info->alpha = x < y ? x:y;
	}
}

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

void calc_beta(map<string, WordInfo> &resultMap)
{
	map<string, WordInfo>::iterator itr = resultMap.begin();
	string s1,s2;
	for( ; itr != resultMap.end(); ++itr)
	{
		if (!itr->second.choosed)
			continue;
		string cur_word = itr->second.word;
		vector<double> values;
		const char *pstr = cur_word.c_str();
		size_t len = 0;
		while(*(pstr+len))
		{
			len += get_char_length(pstr+len);
			s1 = cur_word.substr(0,len);
			s2 = cur_word.substr(len);
			if (resultMap.find(s1) != resultMap.end() && resultMap.find(s2) != resultMap.end())
			{
				values.push_back(resultMap[s1].prob*1e7*resultMap[s2].prob);
			}
		}
		if (values.size() > 0)
		{
			double max = -100;
			for (size_t j = 0; j < values.size(); ++j)
				if (values[j] > max)
					max = values[j];
			itr->second.beta =  itr->second.prob*1e7/max;
		}
		else
			itr->second.beta = itr->second.prob;
	}
}
int convert_to_wstring(const char* str, wstring &out_wstr){ 
	if (!str)
		return 1;
	wchar_t* wcs = NULL; 
	int size = 0; 
	string loc = setlocale(LC_ALL,NULL);
	setlocale(LC_ALL, "zh_CN.utf8"); 
	size = mbstowcs(NULL,str,0); 
	wcs = new wchar_t[size+1]; 
	size = mbstowcs(wcs, str, size+1); 
	wcs[size] = 0; 
	out_wstr = wcs;
	delete[] wcs;
	setlocale(LC_ALL, loc.c_str()); 
	return 0;
}

int extract(const string &input,const int d, const double t1, const double t2)
{
	map<string, WordInfo> resultMap;
	size_t size = 0;
	size_t len = 0;
	size_t pos = 0;
	const char* pstr = input.c_str();
	vector<pair<size_t,size_t> > charVec; //position and length
	while(*(pstr+pos)){
		len = get_char_length(pstr+pos);
		charVec.push_back(pair<size_t,size_t>(pos,len));
		pos += len;
		size++;	
	}
	//cout << "size:" << size << endl;
	//get word statistic info
	for(size_t i = 0; i < size-d; ++i){
		string cur_word;
		for(size_t j = 1; j <=d; ++j){
			cur_word = input.substr(charVec[i].first, charVec[i+j].first-charVec[i].first);
			//cout << i << '\t' << j << '\t' <<  cur_word << endl;
			wstring c; //only one character
			wchar_t left_neighbor;
			wchar_t right_neighbor;
			if (i > 0){
				convert_to_wstring(input.substr(charVec[i-1].first,charVec[i-1].second).c_str(), c);
				left_neighbor = c[0];
			}
			convert_to_wstring(input.substr(charVec[i+j].first,charVec[i+j].second).c_str(), c);
			right_neighbor = c[0];  
			if (resultMap.find(cur_word) == resultMap.end()){
				WordInfo wf;
				wf.word = cur_word;
				wf.freq = 1;
				resultMap.insert(map<string,WordInfo>::value_type(cur_word, wf));
			}
			else{
				resultMap[cur_word].freq += 1;
			}
			if(i >0){
				resultMap[cur_word].leftchars.push_back(left_neighbor);
			}
			resultMap[cur_word].rightchars.push_back(right_neighbor);
		}
	}
	//calculate 
	calc_alpha(resultMap);
	map<string, WordInfo>::iterator itr = resultMap.begin();
	for( ; itr != resultMap.end(); ++itr)
	{
		if (itr->second.alpha < t1)
			itr->second.choosed = false;
	}

	calc_beta(resultMap);
	itr = resultMap.begin();
	for( ; itr != resultMap.end(); ++itr)
	{
		if (itr->second.beta > t2)
			itr->second.print_info();
	}
	return 0;	
}

int main(int argc, char* argv[]) {
	//string str("四是四十是十十四是十四四十是四十");
	if (argc != 5){
		cout << "Usage:./a.out [infile] maxLength threshold1 threshold2" << endl;
		cout << "e.g.:./a.out 1.txt 5 0.9 80" << endl;
		return 0;
	}
	clock_t start = clock();
	ifstream infile(argv[1]);
	if (!infile){
		cout << "open file error:" << argv[1] << endl;
		return 0;
	}
	string line;	
	string input;
	while (getline(infile, line)){
		input += line;
	}
	infile.close();
	int d = atoi(argv[2]); 
	input += string(d,'$'); //for convenience
	extract(input, d, atof(argv[3]), atof(argv[4]));
	clock_t end = clock();
	cout<<"Run time: "<<(double)(end - start) / CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}
