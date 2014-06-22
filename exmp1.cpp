#include <iostream>

using namespace std;

class Exmp1
{
    public:
        Exmp1()
        {
            cout << "Exmp1()" << endl;
        }
        Exmp1(string s):name(s)
        {
            cout << "Exmp1(string s)" << endl;
        }
        Exmp1(const Exmp1&)
        {
            cout << "Exmp1(const Exmp1&)" << endl;
        }
        ~Exmp1()
        {
            cout << "~Exmp1()" << endl;
        }
        Exmp1& operator=(const Exmp1 &rhs)
        {
            cout << "Exmp1& operator=(const Exmp1 &)" << endl;
            name = rhs.name;
            return *this;
        }
    private:
        string name;
};

int test1(Exmp1 e)
{
    cout << "In func: int test1(Exmp1 e)" << endl;
    return 0;
}
int test2(Exmp1 &e)
{
    cout << "In func: int test2(Exmp1 &e)" << endl;
    return 0;
}
Exmp1 test3()
{
    cout << "In func: Exmp1 test3()"<< endl;
    Exmp1 e;
    return e;
}
Exmp1& test4()
{
    cout << "In func: Exmp1& test4()"<< endl;
    Exmp1 *p = new Exmp1();
    return *p;
}

int main(int argc,char **argv)
{
    //Exmp1 *ptr = new Exmp1();
    Exmp1 obj;
    //Exmp1 item1(*ptr),item2(obj);
    //Exmp1 item3;
    //item3 = obj;
    cout << "Call test1()" << endl;
    test1(obj);
    cout << "----Finish calling\n" << endl;

    cout << "Call test2()" << endl;
    test2(obj);
    cout << "----Finish calling\n" << endl;

    cout << "Call test3()" << endl;
    //Exmp1 x = test3();
    test3();
    cout << "----Finish calling\n" << endl;

    cout << "Call test4()" << endl;
    //Exmp1 y = test4();
    Exmp1 &x = test4();
    cout << "----Finish calling\n" << endl;
    Exmp1 *p = &x;
    delete p;
    //delete ptr;
    return 0;
}
