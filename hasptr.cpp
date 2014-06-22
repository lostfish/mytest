#include <iostream>

using namespace std;

class HasPtr
{
    public:
        HasPtr(const int &p, int i): ptr(new int(p)), val(i){}
        HasPtr(const HasPtr &orig): ptr(new int(*orig.ptr)), val(orig.val){ cout << "In copy construction function" << endl;}
        HasPtr& operator=(const HasPtr&);
        ~HasPtr() {delete ptr;}
        int get_ptr_val() const { return *ptr;}
        int get_int() const { return val;}
        void set_ptr(int *p){ ptr = p;}
        void set_int(int i) { val = i;}
        int *get_ptr() const { return ptr;}
        void set_ptr_val(int p) const { *ptr = p;}
    private:
        int *ptr;
        int val;
};

HasPtr& HasPtr::operator=(const HasPtr &rhs) //值型类指针
{
    cout << "In operator= function" << endl;
    *ptr = *rhs.ptr; //copy the value pointed to
    val = rhs.val;
    return *this;
}

int main(int argc,char **argv)
{
    HasPtr x(1,1);
    cout << "x:" << x.get_ptr_val() << '\t' << x.get_int() << endl;
    int i = 2;
    HasPtr y(i,i);
    cout << "y:" << y.get_ptr_val() << '\t' << y.get_int() << endl;
    HasPtr z(y);
    cout << "z:" << z.get_ptr_val() << '\t' << z.get_int() << endl;
    cout << "\n";
    z = x;
    cout << x.get_ptr() <<  " vs " <<  z.get_ptr() << endl; 
    cout << x.get_ptr_val() <<  " vs " <<  z.get_ptr_val() << endl; 
    return 0;
}
