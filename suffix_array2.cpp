/////////////////////////////////////////////////////////////////
//Constructing Suffix Array with Doubling Algorithm, O(n log n).
/////////////////////////////////////////////////////////////////
#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>

using namespace std;


struct Sfx { //means 'sa' array element in the paper
	int i; //string index 
	int key[2];//two ranking values
	bool operator < (const Sfx& s) const
	{   
		return key[0] < s.key[0] || 
			key[0] == s.key[0] && key[1] < s.key[1]; 
	}
};

const int MAX_SFX = 210000;
int g_buf[MAX_SFX + 1]; //'rank' array
Sfx g_tempSfx[2][MAX_SFX];
Sfx *g_sa = g_tempSfx[0];

void cSort(Sfx* in, int n, int key, Sfx* out) { //radix sort is stable
	int* cnt = g_buf;
	memset( cnt, 0, sizeof(int) * (n + 1) );
	for (int i = 0; i < n; i++)
		cnt[ in[i].key[key] ]++; //get count for every ranking value
	for (int i = 1; i <= n; i++)
		cnt[i] += cnt[i - 1]; //accumulate
	for (int i = n - 1; i >= 0; i--){
		out[ --cnt[ in[i].key[key] ] ] = in[i]; //sort
	}
}

//Build a suffix array from string 'text' whose length is 'len'.
//write the result into global array 'g_sa'.
void buildSA(wchar_t* text, int len) {
	Sfx *temp = g_tempSfx[1];
	int* rank = g_buf;
	for (int i = 0; i < len; i++){ //initialize
		g_sa[i].i = i; //from 0 to len-1
		g_sa[i].key[1] = i;  
		g_sa[i].key[0] = text[i]; 
	}
	sort(g_sa, g_sa + len);
	for (int i = 0; i < len; i++)
		g_sa[i].key[1] = 0;
	int wid = 1;
	while (wid < len) {
		rank[ g_sa[0].i ] = 1; //the rank is from 1 to len
		for (int i = 1; i < len; i++){ //g_sa is sorted
			rank[ g_sa[i].i ] = rank[ g_sa[i - 1].i ];
			if ( g_sa[i-1] < g_sa[i] ) 
				rank[ g_sa[i].i ]++;
		}
		for (int i = 0; i < len; i++){ //update the two ranking values
			g_sa[i].i = i;
			g_sa[i].key[0] = rank[i];
			g_sa[i].key[1] = i + wid < len? rank[i + wid]: 0;
		} 
		cSort(g_sa, len, 1, temp); //sort by the second key
		cSort(temp, len, 0, g_sa); //sort by the first key
		wid *= 2; //double increase
	}
}

int getLCP(wchar_t* a, wchar_t* b)
{ 
	int l=0;  
	while(*a && *b && *a==*b) 
	{ l++; a++; b++; }  
	return l;
}

void getLCP(wchar_t* text, Sfx* sfx, int len, int* lcp) {
	int* rank = g_buf;
	for (int i=0, r=0; i < len; i++, r++)
		rank[ sfx[i].i ] = r;
	lcp[0] = 0;
	if (rank[0])
		lcp[ rank[0] ] = getLCP( text, text + sfx[ rank[0]-1 ].i );
	for (int i = 1; i < len; i++) {
		if ( !rank[i] )
			continue;
		if (lcp[ rank[i - 1] ] <= 1)
			lcp[ rank[i] ] = getLCP( text+i, text+sfx[ rank[i]-1 ].i );
		else{
			int L = lcp[ rank[i - 1] ] - 1;
			lcp[rank[i]] = L+getLCP(text+i+L, text+sfx[rank[i]-1].i+L);
		}
	}
}

int convert_to_string(const wchar_t * wstr, string &out_str){ 
	if (!wstr)
		return 1;
	char* str = NULL; 
	int size = 0; 
	string loc = setlocale(LC_ALL,NULL);
	setlocale(LC_ALL, "zh_CN.utf8"); 
	size = wcstombs( NULL, wstr, 0); 
	str = new char[size + 1]; 
	wcstombs(str, wstr, size); 
	str[size] = '\0'; 
	out_str = str;
	setlocale(LC_ALL, loc.c_str()); 
	return 0; 
}

struct interval_t{
	size_t start;
	size_t end;
	size_t repr;
	size_t lbl;
	size_t sil;
	interval_t():start(0),end(0),repr(0),lbl(0),sil(0){}
	interval_t(size_t i, size_t j, size_t k, size_t l, size_t s):start(i),end(j),repr(k),lbl(l),sil(s){}
};

void get_interval(int lcp[], const int n, vector<interval_t> &intervalVec){ //get lcp delimited interval
	stack<size_t> stack_i;
	stack<size_t> stack_k;
	stack_i.push(0);
	stack_k.push(0);
	//cout << "Debug info:\n";
	for(size_t j = 0; j < n; ++j){
		interval_t tmp;
		tmp.start = tmp.end = j;
		tmp.lbl =  lcp[j]>lcp[j+1] ? lcp[j] : lcp[j+1];
		intervalVec.push_back(tmp); //trivial class, if suffix length is less than lbs, the class is empty
		//cout << "j = " << j << endl;
		size_t k = stack_k.top();
		while (lcp[j+1] < lcp[k]){
			size_t i = stack_i.top();
			size_t lbs =  lcp[i]>lcp[j+1] ? lcp[i] : lcp[j+1];
			//cout << "\t" << i << '\t' << j << '\t' << k << '\t' << lbs << '\t' <<  lcp[k] <<  endl;
			if (lbs < lcp[k]) //notrivial class
				intervalVec.push_back(interval_t(i,j,k,lbs,lcp[k]));
			stack_i.pop();
			stack_k.pop();
			k = stack_k.top();
		}
		stack_i.push(k);
		stack_k.push(j+1);
	}
	cout << "i\tj\tk\tlbs\tsil\ttf\n";
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
		else if ( (n-g_sa[i].i-lbl) >= 1)
			cout << i << '\t' << j << '\t' << k << '\t' << lbl << "\t-\t" << tf << endl;
	}
}

int main() {
	//wchar_t str[] = L"aabbaa\0ababab";
	wchar_t str[] = L"to_be`or`not_to_be`";
	//wchar_t str[] = L"嘻嘻哈哈嘻嘻\0嘻哈嘻哈嘻哈";
	//int from[] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
	wstring wstr(str);
	const int n = wstr.size();
	cout << "n=" << n << endl;
	int lcp[n+1];
	lcp[n] = 0;
	buildSA(str, n);
	getLCP(str, g_sa, n, lcp);
	string output;
	for (int i=0; i<n; i++)//The first suffix is useless (empty).
	{ 
		convert_to_string(str+g_sa[i].i, output);
		//cout<<from[g_sa[i].i]<<' '<< output <<' '<<lcp[i]<<endl; 
		cout<< i << '\t' << output <<'\t'<<lcp[i]<<endl; 
	}
	vector<interval_t> intervalVec;
	get_interval(lcp, n, intervalVec);	
	return 0;
}
