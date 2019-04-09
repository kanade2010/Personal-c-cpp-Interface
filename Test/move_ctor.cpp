#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
using namespace std;

class Str{
    public:
        char *str;
        Str(char value[])
        {
            cout<<"普通构造函数..."<<endl;
            str = NULL;
            int len = strlen(value);
            str = (char *)malloc(len + 1);
            memset(str,0,len + 1);
            strcpy(str,value);
        }
        Str(const Str &s)
        {
            cout<<"拷贝构造函数..."<<endl;
            str = NULL;
            int len = strlen(s.str);
            str = (char *)malloc(len + 1);
            memset(str,0,len + 1);
            strcpy(str,s.str);
        }
        Str(Str &&s)
        {
            cout<<"移动构造函数..."<<endl;
            str = NULL;
            str = s.str;
            s.str = NULL;
        }
        ~Str()
        {
            cout<<"析构函数"<<endl;
            if(str != NULL)
            {
                free(str);
                str = NULL;
            }
        }
};

static Str g_s("hello");

Str fun(){

  return move(g_s);

}

int main()
{
    cout << &g_s << g_s.str << endl;
    Str tm_s = fun();
    cout << &tm_s << tm_s.str << endl;

    /*char value[] = "I love zx";
    Str s(value);
    vector<Str> vs;
    //vs.push_back(move(s));
    vs.push_back(std::move(s));
    cout<<vs[0].str<<endl;
    if(s.str != NULL)
        cout<<s.str<<endl;
    */
    return 0;
}