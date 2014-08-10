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


struct SuffixInfo { //means 'sa' array element in the paper
	size_t i; //string index 
	size_t key[2];//two ranking values
	bool operator < (const SuffixInfo& s) const
	{   
		return key[0] < s.key[0] || 
			key[0] == s.key[0] && key[1] < s.key[1]; 
	}
};


void sort_suffix_array(SuffixInfo* in, size_t n, int key, SuffixInfo* out, size_t *rank) { //radix sort is stable
	size_t* cnt = rank;
	memset( cnt, 0, sizeof(size_t) * (n + 1) );
	for (size_t i = 0; i < n; i++)
		cnt[ in[i].key[key] ]++; //get count for every ranking value
	for (size_t i = 1; i <= n; i++)
		cnt[i] += cnt[i - 1]; //accumulate
	for (long long i = n - 1; i >= 0; i--){
		out[ --cnt[ in[i].key[key] ] ] = in[i]; //sort
	}
}

size_t getLCP(const wchar_t* a, const wchar_t* b)
{ 
	size_t l=0;  
	while(*a && *b && *a==*b) 
	{ l++; a++; b++; }  
	return l;
}

void getLCP(const wchar_t* text, SuffixInfo* sfx, size_t len, size_t* lcp, size_t *rank) {
	for (size_t i=0, r=0; i < len; i++, r++)
		rank[ sfx[i].i ] = r;
	lcp[0] = 0;
	if (rank[0])
		lcp[ rank[0] ] = getLCP( text, text + sfx[ rank[0]-1 ].i );
	for (size_t i = 1; i < len; i++) {
		if ( !rank[i] )
			continue;
		if (lcp[ rank[i - 1] ] <= 1)
			lcp[ rank[i] ] = getLCP( text+i, text+sfx[ rank[i]-1 ].i );
		else{
			size_t L = lcp[ rank[i - 1] ] - 1;
			lcp[rank[i]] = L+getLCP(text+i+L, text+sfx[rank[i]-1].i+L);
		}
	}
}

//Build a suffix array from string 'text' whose length is 'len'.
//write the result into global array 'sa'.
void buildSA(const wchar_t* text, const size_t len, size_t *lcp, SuffixInfo *sa) {
	SuffixInfo *temp = new SuffixInfo[len];
	size_t *rank = new size_t[len + 1]; //'rank' array
	for (size_t i = 0; i < len; i++){ //initialize
		sa[i].i = i; //from 0 to len-1
		sa[i].key[1] = i;  
		sa[i].key[0] = text[i]; 
	}
	sort(sa, sa + len);
	for (size_t i = 0; i < len; i++)
		sa[i].key[1] = 0;
	size_t wid = 1;
	while (wid < len) {
		rank[ sa[0].i ] = 1; //the rank is from 1 to len
		for (size_t i = 1; i < len; i++){ //sa is sorted
			rank[ sa[i].i ] = rank[ sa[i - 1].i ];
			if ( sa[i-1] < sa[i] ) 
				rank[ sa[i].i ]++;
		}
		for (size_t i = 0; i < len; i++){ //update the two ranking values
			sa[i].i = i;
			sa[i].key[0] = rank[i];
			sa[i].key[1] = i + wid < len? rank[i + wid]: 0;
		} 
		sort_suffix_array(sa, len, 1, temp, rank); //sort by the second key
		sort_suffix_array(temp, len, 0, sa, rank); //sort by the first key
		wid *= 2; //double increase
	}
	getLCP(text, sa, len, lcp, rank);
	delete[] rank;
	delete[] temp;
}


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

struct IntervalInfo{
	size_t start;
	size_t end;
	size_t repr;
	size_t lbl;
	size_t sil;
	IntervalInfo():start(0),end(0),repr(0),lbl(0),sil(0){}
	IntervalInfo(size_t i, size_t j, size_t k, size_t l, size_t s):start(i),end(j),repr(k),lbl(l),sil(s){}
};

void get_interval(const size_t lcp[], const size_t n, vector<IntervalInfo> &intervalVec,SuffixInfo *sa, int d){ //get lcp delimited interval
	stack<size_t> stack_i;
	stack<size_t> stack_k;
	stack_i.push(0);
	stack_k.push(0);
	//cout << "Debug info:\n";
	size_t lbl;
	for(size_t j = 0; j < n; ++j){
		lbl =  lcp[j]>lcp[j+1] ? lcp[j] : lcp[j+1];
		if (lbl < d){
			IntervalInfo tmp;
			tmp.start = tmp.end = j;
			tmp.lbl =  lcp[j]>lcp[j+1] ? lcp[j] : lcp[j+1];
			intervalVec.push_back(tmp); //trivial class, if suffix length is less than lbl, the class is empty
		}
		//cout << "j = " << j << endl;
		size_t k = stack_k.top();
		while (lcp[j+1] < lcp[k]){
			size_t i = stack_i.top();
			lbl =  lcp[i]>lcp[j+1] ? lcp[i] : lcp[j+1];
			//cout << "\t" << i << '\t' << j << '\t' << k << '\t' << lbl << '\t' <<  lcp[k] <<  endl;
			if (lbl < lcp[k] && lbl < d) //notrivial class
			{
				intervalVec.push_back(IntervalInfo(i,j,k,lbl,lcp[k]));
				//cout << "i = " << i << '\t' << "j = " << j << endl;
			}
			stack_i.pop();
			stack_k.pop();
			k = stack_k.top();
		}
		stack_i.push(k);
		stack_k.push(j+1);
	}
	/*
	cout << "i\tj\tk\tlbl\tsil\ttf\n";
	for (size_t m = 0; m < intervalVec.size(); ++m){
		size_t i = intervalVec[m].start;
		size_t j = intervalVec[m].end;
		size_t k = intervalVec[m].repr;
		size_t lbl = intervalVec[m].lbl;
		size_t sil=  intervalVec[m].sil;
		size_t tf = j-i+1;
		if (i > j)
			cout << "WARNING: i>j " << endl;
		else if (i < j)
			cout << i << '\t' << j << '\t' << k << '\t' << lbl << '\t' << sil << '\t' << tf << endl;
		else if ( (n-sa[i].i-lbl) >= 1)
			cout << i << '\t' << j << '\t' << k << '\t' << lbl << "\t-\t" << tf << endl;
	}
	*/
}

///
struct WordInfo{
	string word;
	double alpha; //自由程度
	double leftEntroy;
	double rightEntroy;
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
		double x =  cur_info->leftEntroy;
		double y =  cur_info->rightEntroy;
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

int extract(wstring &input,const int d, const double t1, const double t2)
{
	map<string, WordInfo> resultMap;
	size_t n = input.size(); //input size
	size_t  *lcp = new size_t[n+1];
	lcp[n] = 0;
	SuffixInfo *sa = new SuffixInfo[n];
	vector<IntervalInfo> intervalVec;
	cout <<"Build suffix array, string size:" << n << endl;
	buildSA(input.c_str(), n,lcp,sa);
	cout << "get interval..." << endl;
	get_interval(lcp,n,intervalVec, sa, d);
	//cout << "n=" << n << endl;
	vector<wchar_t> rightchars;
	for (size_t m = 0; m < intervalVec.size(); ++m)
	{
		size_t i = intervalVec[m].start;
		size_t j = intervalVec[m].end;
		//size_t k = intervalVec[m].repr;
		size_t lbl = intervalVec[m].lbl;
		size_t sil=  intervalVec[m].sil;
		if (i == j){
			sil = n-sa[i].i;
			if ((sil -lbl) < 1)
			 continue;
		}
		size_t tf = j-i+1;
		for(size_t len = lbl+1; len <= sil; ++len){
			if ( len > d )
				continue;
			string cur_word;
			convert_to_string(input.substr(sa[i].i,len).c_str(), cur_word);
			WordInfo tmp_info;
			tmp_info.word = cur_word;
#ifdef _DEBUG
			cout << "cur_word:" << cur_word << endl;
			cout << "right:";
#endif
			rightchars.clear(); //must clear
			for (size_t xx = i; xx <= j; ++xx){
				wchar_t right_neighbor = input[sa[xx].i+len];
#ifdef _DEBUG
				cout << right_neighbor << '\t';
#endif
				rightchars.push_back(right_neighbor);
			}
#ifdef _DEBUG
			cout << endl;
#endif
			//tmp_info.leftchars.push_back(left_neighbor);
			tmp_info.rightEntroy = calc_entropy(rightchars);
			tmp_info.freq = tf;
			resultMap.insert(map<string,WordInfo>::value_type(cur_word,tmp_info));
		}
	}
	//get left neighbor
	vector<wchar_t> leftchars;
	reverse(input.begin(),input.end()); //reverse the input string
	//string s;
	//convert_to_string(input.c_str(), s);
	//cout << s << endl;
	//memset(lcp,sizeof(size_t)*(n+1),0);
	intervalVec.clear();
	cout <<"Build suffix array for the reversed string" << endl;
	buildSA(input.c_str(), n,lcp,sa);
	cout << "get interval for the reversed string" << endl;
	get_interval(lcp,n,intervalVec, sa, d);
	for (size_t m = 0; m < intervalVec.size(); ++m){
		size_t i = intervalVec[m].start;
		size_t j = intervalVec[m].end;
		size_t lbl = intervalVec[m].lbl;
		size_t sil=  intervalVec[m].sil;
		if (i == j){
			sil = n-sa[i].i;
			if ((sil -lbl) < 1)
			 continue;
		}
		for(size_t len = lbl+1; len <= sil; ++len){
			if ( len > d )
				continue;
			string cur_word;
			wstring tmp_wstr = input.substr(sa[i].i,len);
			reverse(tmp_wstr.begin(), tmp_wstr.end());
			convert_to_string(tmp_wstr.c_str(), cur_word);
#ifdef _DEBUG
			cout << "cur_word:" << cur_word << endl;
			cout << "left:";
#endif
			leftchars.clear();
			if (resultMap.find(cur_word) != resultMap.end()){
				for (size_t xx = i; xx <= j; ++xx){
					wchar_t left_neighbor = input[sa[xx].i+len];
#ifdef _DEBUG
					cout << left_neighbor << '\t';
#endif
					leftchars.push_back(left_neighbor);
				}
#ifdef _DEBUG
				cout << endl;
#endif
				resultMap[cur_word].leftEntroy = calc_entropy(leftchars);
			}
		}
	}
	delete[] lcp;
	delete[] sa;
	cout << "start calculation" << endl;
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

int main(int argc, char* argv[]) {
	//wstring input(L"四是四十是十十四是十四四十是四十");
	clock_t start = clock();
	
	if (argc != 5){
		cout << "Usage:./a.out [infile] maxLength threshold1 threshold2" << endl;
		cout << "e.g.:./a.out 1.txt 5 0.9 80" << endl;
		return 0;
	}
	ifstream infile(argv[1]);
	if (!infile){
		cout << "open file error:" << argv[1] << endl;
		return 0;
	}
	string line;	
	string s;
	wstring input;
	while (getline(infile, line)){
		s += line;
	}
	convert_to_wstring(s.c_str(),input);
	infile.close();
	
	//cout << "size:" << input.size() << endl;
	//extract(input, 6, 0, 1);
	extract(input, atoi(argv[2]), atof(argv[3]), atof(argv[4]));
	clock_t end = clock();
	cout<<"Run time: "<<(double)(end - start) / CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}
