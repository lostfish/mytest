#include <iostream>
#include <stdlib.h>

using namespace std;


#ifdef _DEBUG
#define dbg(format,...) printf("File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)
#else
#define dbg(format,...)
#endif

void swap(int &a,int &b)
{
	int c = a;
	a = b;
	b = c;
}

void display(int *a, const int n)
{
	for (size_t i = 0; i < n; i++)
		cout << a[i] << ' ';
	cout << endl;
}

int partition(int *a, int start, int end)
{
	int sentinal = a[end];
	int j = start-1;
	for(size_t i = start; i<= end-1; i++)
	{
		if (a[i] < sentinal)
		{
			j++;
			swap(a[i],a[j]); //left less, right greater
		}
	}
	swap(a[j+1],a[end]);
	return j+1;
}
void quick_sort_internal(int *a, int start, int end ,int depth=0)
{
	if (start < end)
	{
		int pivot = partition(a,start,end);
		//cout << "depth: " << depth << '\t' << start << '\t' << end  << '\t' << pivot << '\t' << a[start] << '\t' << a[end]<< endl;
		quick_sort_internal(a,start,pivot-1,depth+1);
		quick_sort_internal(a,pivot+1,end,depth+1);
	}
}

void quick_sort(int *a, int n) 
{
	quick_sort_internal(a,0,n-1);
}

void bubble_sort(int *a, int n)
{
	for (size_t i = 0; i < n-1; i++)
		for (size_t j = i+1; j < n; j++) //choose the smallest item and put it at a[i] 
		{
			if (a[i] > a[j])
				swap(a[i],a[j]);
		}
}


void bubble_sort2(int *a, int n)
{
	for (size_t i = 1; i < n; ++i) 
	{
		for (size_t j = 0; j < n-i; ++j)
			if (a[j] > a[j+1])
				swap(a[j],a[j+1]);
	}
}

void select_sort(int *a, int n)
{
	for (size_t i = 0; i < n; i++)
	{
		int min = i;
		for (size_t j = i+1; j < n; j++) 
			if ( a[min] > a[j])
				min = j;
		if (min != i)
			swap(a[min], a[i]);
	}
}

void insert_sort(int *a, int n)
{
	for (size_t i = 1; i < n; i++)
	{
		int tmp = a[i];
		size_t j;
		for (j = i-1; j >= 0; j--) 
		{
			if (tmp < a[j])
			{
				a[j+1] = a[j];
			}
			else
				break;
		}
		if (j != i-1)
			a[j+1] = tmp;
	}

}

void shell_sort(int *a, int n)
{
	for (int gap = n/2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < n; i += gap)
		{
			int tmp = a[i];
			int j;
			for (j = i-gap; j >= 0; j -= gap) 
			{
				if (tmp < a[j])
				{
					a[j+gap] = a[j];
				}
				else
					break;
			}
			if (j != i-gap)
				a[j+gap] = tmp;
		}
	}

}

void merge(int *a,int left,int mid, int right)
{
	const int m = right+1;
	int b[m];
	size_t i,j,k;
	for(i = left; i < m; i++)
		b[i] = a[i]; //use the auxiliary array
	
	i = left;
	j = mid+1;
	k = left;
	while( i <= mid && j <= right)
	{
		if (b[i] < b[j])
			a[k++] = b[i++];
		else
			a[k++] = b[j++];
	}
	while (i <= mid)
		a[k++] = b[i++];
	while (j <= right)
		a[k++] = b[j++];
}

void merge2(int *a,int left,int mid, int right)
{
	for (size_t i = mid+1; i <= right; i++) //same as insert_sort
	{
		int tmp = a[i];
		size_t j;
		for (j = i-1; j >= left; j--) 
		{
			if (tmp < a[j])
			{
				a[j+1] = a[j];
			}
			else
				break;
		}
		if (j != i-1)
			a[j+1] = tmp;
	}
}

void merge_sort_internal(int *a,int left, int right)
{
	if(left < right)
	{
		int mid = (left+right)/2;
		//cout << "merge_sort_internal: " <<  left << '\t' << right << endl;
		merge_sort_internal(a,left,mid);
		merge_sort_internal(a,mid+1,right);
		merge2(a, left, mid, right);
	}
}
void merge_sort(int *a, int n)
{
	merge_sort_internal(a,0,n-1);
}

void siftdown(int *a, int last)
{
	int i = 0;
	int next;
	while(1)
	{
		next = 2*i+1;	
		if (next > last) 
			break;
		if (next+1 <= last && a[next+1] > a[next])
			next++;
		if (a[i] >= a[next])
			break;
		swap(a[i],a[next]);
		i = next;
	}
}

void siftup(int *a, int last)
{
	int prev;
	int i = last;
	while(i > 0)
	{
		prev = (i-1)/2;	
		if (a[prev] >= a[i])
			break;
		swap(a[prev], a[i]);
		i = prev;
	}
}

void heap_sort(int *a, int n) 
{
	int i;
	//build the max heap, a[i] -> a[2*i+1],a[2*i+2]
	for(i = 1; i < n; i++)
		siftup(a,i);
	//sort
	for(i = n-1; i > 0; i--)
	{
		swap(a[0],a[i]);
		siftdown(a,i-1);
	}
}

void reset(int *a,const int n)
{
	srand(0);
	for (size_t i = 0; i < n; i++)
		a[i] = rand()%10000;
}
void generate(int m, int n)
{
	for(int i = 0; i < n; ++i)
		if (rand()%(n-i) < m)
		{
			cout << i << endl;
			--m;
		}
}


int main()
{
	const int n = 100;
	int a[n];

	reset(a,n);
	display(a,n);

	cout << "quick_sort:" << endl;
	quick_sort(a,n);
	display(a,n);

	cout << "bubble_sort:" << endl;
	reset(a,n);
	bubble_sort(a,n);
	display(a,n);

	cout << "bubble_sort2:" << endl;
	reset(a,n);
	bubble_sort2(a,n);
	display(a,n);

	cout << "insert_sort:" << endl;
	reset(a,n);
	insert_sort(a,n);
	display(a,n);

	cout << "shell_sort:" << endl;
	reset(a,n);
	shell_sort(a,n);
	display(a,n);

	cout << "merge_sort:" << endl;
	reset(a,n);
	merge_sort(a,n);
	display(a,n);

	cout << "heap_sort:" << endl;
	reset(a,n);
	heap_sort(a,n);
	display(a,n);
	
	//size_t x = -1;
	//cout << x <<  '\t' << sizeof(x) << '\t' << (x+1) << '\t' << (x+2) << endl;
	//cout << 0xFFFFFFFFFFFFFFFF << endl;
	return 0;
}
