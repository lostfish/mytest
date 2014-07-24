#ifndef _SUFFIX_TREE_W_H_
#define _SUFFIX_TREE_W_H_

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#include <stdio.h>

using namespace std;

#ifdef _DEBUG
//#define dbg(format,...) printf("Line:%05d\t"format"", __LINE__, ##__VA_ARGS__)
#define dbg(format,...) printf(format,##__VA_ARGS__)
#else
#define dbg(format,...)
#endif

class Node{
	public:
		int first_char_index;
		int last_char_index;
		int type;
		Node* suffix_link;
		Node* parent_node;
		vector<Node*> children;
		Node(){}
		Node(int i, int j, Node* parent,int flag=1){ //1 means the it's not root node or dumb node
			first_char_index = i;
			last_char_index = j;
			type = flag;
			suffix_link = NULL;
			parent_node = parent;
			if(parent_node)
				parent_node->add_child(this); 
		}
		~Node(){}
		void add_child(Node *child){
			children.push_back(child);
		}
		void remove_child(Node *child){
			vector<Node*>::iterator itr = find(children.begin(), children.end(), child);
			if (itr != children.end())
				children.erase(itr);
		}
};

class SuffixTree{
	public:
		Node* origin_node;
		Node* root_node;
		Node* dumb_node;
		int first_char_index;
		int last_char_index;
		wstring str;
		Node* parent_node;
		Node* last_parent_node;
		SuffixTree(){}
		~SuffixTree();
		SuffixTree(wstring s):str(s+L'$'){ //!!Note that add '$' to the end of the string
			first_char_index = 0;
			last_char_index = -1;
			dumb_node = new Node(0,0,NULL,-1); //node type is -1
			root_node = new Node(0,0,NULL,0); //type is 0
			origin_node = root_node; 
			parent_node = NULL;
			last_parent_node = NULL;
		}
		Node* find_end_node(Node* from, wchar_t c);
		Node* split_edge(Node* edge_end_node); //split origin_node
		void canonize();
		void update(int cur_char_index);
		void print_tree();
		void build_tree();
		string to_string(const wchar_t * wStr);
};

Node* SuffixTree::split_edge(Node* edge_end_node){ //split origin_node
	if (!edge_end_node)
	{
		dbg("edge_end_node is null\n");
		return NULL;
	}
	Node* parent = edge_end_node->parent_node;
	int start = edge_end_node->first_char_index;
	Node* internal_node = new Node(start, start+this->last_char_index-this->first_char_index, parent);
	internal_node->add_child(edge_end_node);
	internal_node->suffix_link = origin_node;

	edge_end_node->first_char_index = internal_node->last_char_index+1;		
	edge_end_node->parent_node = internal_node;
	parent->remove_child(edge_end_node);
	return internal_node;
} 

Node* SuffixTree::find_end_node(Node* from, wchar_t c){
	if (!from)
	{
		dbg("From node is null\n");
		return NULL;
	}
	vector<Node*>::iterator itr = from->children.begin();
	for(; itr != from->children.end(); ++itr)
	{
		Node* cur_child = *itr;
		if (str[cur_child->first_char_index] == c)
			return cur_child;
	}
	return root_node;
}

void SuffixTree::canonize()
{
	if (last_char_index >= first_char_index)
	{
		Node* edge_end_node = find_end_node(origin_node,str[first_char_index]);
		int span = edge_end_node->last_char_index - edge_end_node->first_char_index; 
		while(span <= (last_char_index-first_char_index))
		{
			first_char_index += span + 1;
			origin_node = edge_end_node;
			if (first_char_index <= last_char_index)
			{
				Node* edge_end_node = find_end_node(origin_node,str[first_char_index]);
				span = edge_end_node->last_char_index - edge_end_node->first_char_index; 
			}
		}
	}
}

void SuffixTree::update(int cur_char_index)
{
	last_parent_node = dumb_node;
	while(1)
	{
		dbg("New loop: %x\t%d\t%d\n", (unsigned int)(origin_node-root_node), first_char_index,last_char_index);
		parent_node = this->origin_node;
		if (this->last_char_index < this->first_char_index) //Explicit node
		{
			Node* end_node = find_end_node(this->origin_node,str[cur_char_index]);
			if (end_node->type != 0) //not root node
				break;
		}
		else //Implicit node
		{
			Node* end_node = find_end_node(origin_node,str[first_char_index]);
			int span = last_char_index - first_char_index;
			if (str[end_node->first_char_index+span+1] == str[cur_char_index])
				break;
			parent_node = split_edge(end_node); 
		}
		Node* leaf_node = new Node(cur_char_index, str.size(),parent_node);
		if (last_parent_node->type == 1)
			last_parent_node->suffix_link = parent_node;

		last_parent_node = parent_node;

		if (origin_node->type == 0)
			this->first_char_index++;
		else
			this->origin_node = this->origin_node->suffix_link;
		canonize();
	}
	if (last_parent_node->type == 1)
		last_parent_node->suffix_link = parent_node;
	this->last_char_index++;
	canonize();
}

void SuffixTree::build_tree(){
	for (int i = 0; i < str.size(); ++i)
	{
		dbg("-->Phase %d\n", i);
		string cur_str = to_string(str.substr(0,i+1).c_str());
		dbg("Current string: %s\n", cur_str.c_str());
		update(i);
	}
}

string SuffixTree::to_string(const wchar_t * wStr){ 
	char* pStr = NULL; 
	int size = 0; 
	string loc = setlocale(LC_ALL,NULL);
	setlocale(LC_ALL, "zh_CN.utf8"); 
	size = wcstombs( NULL, wStr, 0); 
	pStr = new char[size + 1]; 
	wcstombs( pStr, wStr, size); 
	pStr[size] = '\0'; 
	setlocale(LC_ALL, loc.c_str()); 
	string tmp(pStr);
	delete[] pStr;
	return tmp;
}

SuffixTree::~SuffixTree(){ //traverse by level
	queue<Node*> q;
	q.push(root_node);
	Node *cur_node;
	while(!q.empty())
	{
		cur_node = q.front();
		vector<Node*>::iterator itr = cur_node->children.begin();
		for( ; itr != cur_node->children.end(); ++itr)
			if ((*itr)->type == 1)
				q.push(*itr);
		q.pop();
		delete cur_node;
	}
	root_node = NULL;

	if (dumb_node){
		delete dumb_node;
		dumb_node = NULL;
	}
}

void SuffixTree::print_tree(){
	stack<pair<Node*,int> > st;
	st.push(pair<Node*,int>(root_node, 0));
	Node *cur_node;
	int offset = 0;
	while(!st.empty()){
		pair<Node*, int> p = st.top();
		st.pop();
		cur_node  = p.first;
		offset = p.second;
		wstring sub = this->str.substr(cur_node->first_char_index,cur_node->last_char_index-cur_node->first_char_index+1); 
		string gap(offset,'-');
		if (cur_node == root_node)
			cout << gap << "root" << endl;
		else
			cout << gap << to_string(sub.c_str()) << endl; 
		for(int i = cur_node->children.size()-1; i >= 0; --i)
			if (cur_node->children[i]->type == 1)
				st.push(pair<Node*,int>(cur_node->children[i],offset+5));
	}
}

#endif
