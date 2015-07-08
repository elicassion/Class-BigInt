#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<string>
#include<cmath>
#include<algorithm>
#include<cstdio>
#include<queue>
#include<stack>
#include<list>
#include<vector>
#include<set>
#include<map>
#define FOR(i,bg,ed) for(int i=bg;i<=ed;++i)
#define RFOR(i,bg,ed) for(int i=bg;i>=ed;--i)
#define FOR_S(i,bg,ed,step) for(int i=bg;i<=ed;i+=step)
#define RFOR_S(i,bg,ed,step) for(int i=bg;i>=ed;i-=step)
#define MSET(a,i) memset(a,i,sizeof(a))
#define CIa1(a,i) cin>>a[i]
#define CIa2(a,i,j) cin>>a[i][j]
#define COa1(a,i) cout<<a[i]<<' '
#define COa2(a,i,j) cout<<a[i][j]<<' '
#define SCIa1(a,i) scanf("%d",&a[i])
#define SCIa2(a,i,j) scanf("%d",&a[i][j])
#define SCOa1(a,i) printf("%d ",a[i])
#define SCOa2(a,i,j) printf("%d ",a[i][j])
#define RFF(s) freopen(s,"r",stdin)
#define WFF(s) freopen(s,"w",stdout)
#define LL long long int
#define SPACE printf(" ")
#define ENTER printf("\n")
using namespace std;
class BigInt{
    //流输入输出运算符
    friend ostream& operator << (ostream &out, const BigInt& x)
    {
        int x_size = x.s.size();
        if (x.s.back() && x_size>=1)
            out << x.s.back();
        else if (!x.s.back() && x_size ==1)
            out << x.s.back();
        for (int i=x_size-2;i>=0;--i)
        {
            char buf[40];
            sprintf(buf, "%08d", x.s[i]);
            for (int j=0;j<strlen(buf);++j) out << buf[j];
        }
        return out;
    }
    friend istream& operator >> (istream &in, BigInt& x)
    {
        string s;
        if (!(in >> s)) return in;
        x = s;
        return in;
    }

    friend BigInt max(const BigInt& x, const BigInt& y)
    {
        if (x>y)
            return x;
        else return y;
    }

    public:
    static const int BASE = 100000000;
    static const int WIDTH = 8;
    vector<int> s;

    //构造函数
    BigInt(long long int num = 0) {*this = num;}
    BigInt(const BigInt& b)
    {
        int b_size=b.s.size();
        for (int i=0;i<b_size;++i)
            s.push_back(b.s[i]);
    }

    //赋值运算符
    BigInt operator = (long long num)
    {
        s.clear();
        do{
            s.push_back(num % BASE);
            num /= BASE;
        }while(num > 0);
        while (!s.back() && !s.empty())
                s.pop_back();
        if (s.empty())
            s.push_back(0);
        return *this;
    }
    BigInt operator = (const string& str)
    {
        s.clear();
        int x;
        int len = (str.length() - 1) / WIDTH + 1;
        for (int i=0;i<len;++i)
        {
            int end = str.length() - i*WIDTH;
            int start = max(0, end - WIDTH);
            sscanf(str.substr(start, end-start).c_str(), "%d", &x);
            s.push_back(x);
        }
        while (!s.back() && !s.empty())
                s.pop_back();
        if (s.empty())
            s.push_back(0);
        return *this;
    }

    //四则运算运算符
    BigInt operator + (const BigInt& b) const
    {
        BigInt c;
        c.s.clear();
        int this_size = s.size(), b_size = b.s.size();
        for (int i = 0, g = 0; ; ++i)
        {
            if (g == 0 && i >= this_size && i >= b_size) break;
            int x = g;
            if (i < this_size) x += s[i];
            if (i < b_size) x += b.s[i];
            c.s.push_back(x % BASE);
            g = x / BASE;
        }
        return c;
    }
    BigInt operator += (const BigInt& b)
    {
        *this=*this+b;
        return *this;
    }
    BigInt operator - (const BigInt& b) const
    {
        BigInt c;
        c.s.clear();
        int this_size = s.size(), b_size = b.s.size();
        if (*this == b) {c = 0; return c; }
        else if (*this > b)
        {
            for(int i = 0, g = 0; ; ++i)
            {
                if (g == 0 && i >= this_size && i >= b_size) break;
                int x = g;
                if (i < this_size) x += s[i];
                if (i < b_size) x -= b.s[i];
                if (x < 0) {x += BASE; g = -1; }
                c.s.push_back(x);
            }
            while (!c.s.back())
                c.s.pop_back();
            if (c.s.empty())
                c.s.push_back(0);
            return c;
        }

        else
        {
            for(int i = 0, g = 0; ; ++i)
            {
                if (g == 0 && i >= this_size && i >= b_size) break;
                int x = g;
                if (i < b_size) x += b.s[i];
                if (i < this_size) x -= s[i];
                if (x < 0) {x += BASE; g = -1; }
                c.s.push_back(x);
            }
            c.s.back() = 0 - c.s.back();
            while (!c.s.back())
                c.s.pop_back();
            if (c.s.empty())
                c.s.push_back(0);
            return c;
        }

    }

    BigInt operator -= (const BigInt& b)
    {
        *this = *this - b;
        return *this;
    }

    BigInt operator * (const BigInt& b) const
    {
        BigInt c;
        c.s.clear();
        c = 0;
        int this_size = s.size(), b_size = b.s.size();
        for (int i=0;i<b_size;++i)
        {
            BigInt tmp;
            tmp.s.clear();
            if (i != 0)
            {
                string sufzero = "";
                for (int j=1;j<=i;++j)  sufzero += "00000000";
                tmp = sufzero;
            }
            long long int g = 0;
            for (int j = 0; ; ++j)
            {
                if (g == 0 && j >= this_size ) break;
                long long int x = g;
                if (j < this_size) x += (long long int)s[j] * b.s[i];
                tmp.s.push_back(x % BASE);
                g = x / BASE;

            }
            c += tmp;
        }
        return c;
    }

    BigInt operator *= (const BigInt& b)
    {
        *this = *this * b;
        return *this;
    }



    //比较运算符
    bool operator < (const BigInt& b) const
    {
        if (s.size() != b.s.size()) return (s.size() < b.s.size() );
        for (int i=s.size()-1;i>=0;--i) if(s[i] != b.s[i]) return (s[i] < b.s[i]);
        return false;
    }
    bool operator > (const BigInt& b) const { return b < *this; }
    bool operator <= (const BigInt& b) const { return !(b < *this); }
    bool operator >= (const BigInt& b) const { return !(*this < b); }
    bool operator != (const BigInt& b) const { return (*this < b) || (*this > b); }
    bool operator == (const BigInt& b) const { return !((*this < b) || (*this > b)); }
    bool operator ! () const { return *this==0; }


    //除二
    BigInt div_two() const
    {
        if (*this<2) return 0;
        int this_size=s.size();
        vector<int> tmp;
        tmp.push_back(0);
        int res=0;
        for (int i=this_size-1;i>=0;--i)
        {
            int ttmp=res*BASE+s[i];
            tmp.push_back(ttmp/2);
            res=ttmp%2;
            ++tmp[0];
        }
        int non_zero=1;
        while (!tmp[non_zero])
            ++non_zero;
        BigInt c;
        c.s.clear();
        for (int i=tmp[0];i>=non_zero;--i)
            c.s.push_back(tmp[i]);
        return c;
    }


    //高精度除以高精度(二分试商)
    BigInt operator / (const BigInt& b) const
    {
        BigInt tmp_b = b.abs();
        BigInt tmp_this = this->abs();
        //cout<<tmp_this<<' '<<tmp_b<<endl;
        bool MINUS=(b<0)^(*this<0); //0 positive
        if (tmp_this<tmp_b) return 0;
        int this_size = tmp_this.s.size();
        int b_size = tmp_b.s.size();
        BigInt left=1;
        BigInt right = tmp_this;
        BigInt tmpsum = left+right;
        BigInt mid = tmpsum.div_two();
        while (!(tmp_this >= mid*tmp_b && tmp_this < (tmp_b+mid*tmp_b)))
        {

            if (mid*tmp_b < tmp_this) left=mid+1;
            else right=mid;
            tmpsum = left+right;
            mid = tmpsum.div_two();
            //cout<<mid<<endl;
            //system("pause");
        }
        if (!MINUS)
            return mid;
        else return mid.minus();
    }

    //取余
    BigInt operator % (const BigInt& b) const
    {
        BigInt c = *this-(*this/b)*b;
        return c;
    }

    //void print() const { cout<<*this; }
    BigInt abs () const
    {
        BigInt tmp=*this;
        if (*this>=0)
            return tmp;
        else
        {
            tmp.s.back()=-tmp.s.back();
            return tmp;
        }
    }

    BigInt minus () const
    {
        BigInt tmp_this = *this;
        tmp_this.s.back()=-tmp_this.s.back();
        return tmp_this;
    }


};
int main()
{
    BigInt a,c,d,e,f,g,h,i,j,k,l,m,n=0;
    a="12345678901234567890";
    //b="99999999999999999999";
    d="11111111111111111111";
    e="22222222222222222222";
    f=0;
    g=222;
    h="9999999999999999";
    i="9999999999999999";
    m="654987321354987987";
    int q=999;
    cout<<a%3<<endl;
    cout<<d/e<<endl;
    cout<<!f<<endl;
}
