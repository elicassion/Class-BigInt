#ifndef bigint_h
#define bigint_h


#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<cstring>
#include<cassert>
#include<string>
#include<cmath>
#include<algorithm>
#include<cstdio>
#include<vector>
using namespace std;
class BigInt{
public:
    string number_; //natrual
    bool sgn_;//sign 0 pos 1 neg
    static const int BASE=10;

	BigInt(string s="0"):number_(""),sgn_(0)
	{
	    if (s[0]=='-') { sgn_=1; s.erase(0,1); }
	    s.erase(0,s.find_first_not_of('0'));
        int len=s.length();
        if (len==0) { number_="0"; sgn_=0; }
        else number_=s;
    }

	BigInt(const BigInt &b) { number_=b.number_; sgn_=b.sgn_; }
	~BigInt() { }


    static int abs_cmp(string a,string b)
    {
        int a_l=a.length();
        int b_l=b.length();
        if(a_l>b_l) return 1;
        else if(a_l<b_l)  return -1;
        else return a.compare(b);
    }
    string add(string a, string b) const
    {
        string res="";
        int a_l=a.length();
        int b_l=b.length();
        if(a_l<b_l)
        {
            for(int i=1;i<=b_l-a_l;i++)
            a="0"+a;
        }
        else
        {
            for(int i=1;i<=a_l-b_l;i++)
            b="0"+b;
        }
        a_l=a.length();
        int g=0;
        int x;
        for(int i=a_l-1;i>=0;i--)
        {
            x=a[i]-'0'+b[i]-'0'+g;
            g=x/10;
            res=char(x%10+'0')+res;
        }
        if(g!=0)
            res=char(g+'0')+res;
        return res;
    }

    string sub(string a, string b) const
    {
        string res;
        int del_l=a.length()-b.length();
        int g=0;
        for(int i=b.length()-1;i>=0;i--)
        {
            if(a[del_l+i]<b[i]+g)
            {
                res=char(a[del_l+i]-b[i]-g+'0'+10)+res;
                g=1;
            }
            else
            {
                res=char(a[del_l+i]-b[i]-g+'0')+res;
                g=0;
            }
        }
        for(int i=del_l-1;i>=0;i--)
        {
            if(a[i]-g>='0')
            {
                res=char(a[i]-g)+res;
                g=0;
            }
            else
            {
                res=char(a[i]-g+10)+res;
                g=1;
            }
        }
        res.erase(0,res.find_first_not_of('0'));
        return res;
    }


    string mul(string a,string b) const
    {
        string res;
        int a_l=a.length();
        int b_l=b.length();
        string temp_res;
        for(int i=b_l-1;i>=0;i--)
        {
            temp_res="";
            int x=b[i]-'0';
            int t=0;
            int g=0;
            if(x!=0)
            {
                for(int j=1;j<=b_l-1-i;j++)
                temp_res+="0";
                for(int j=a_l-1;j>=0;j--)
                {
                    t=(x*(a[j]-'0')+g)%10;
                    g=(x*(a[j]-'0')+g)/10;
                    temp_res=char(t+'0')+temp_res;
                }
                if(g!=0) temp_res=char(g+'0')+temp_res;
            }
            res=add(res,temp_res);
        }
        res.erase(0,res.find_first_not_of('0'));
        return res;
    }

    void div(string a,string b,string &quotient,string &residue) const
    {
        quotient=residue="";
        if(b=="0")
        {
            quotient=residue="ERROR";
            return;
        }
        if(a=="0")
        {
            quotient=residue="0";
            return;
        }
        int res=abs_cmp(a,b);
        if(res<0)
        {
            quotient="0";
            residue=a;
            return;
        }
        else if(res==0)
        {
            quotient="1";
            residue="0";
            return;
        }
        else
        {
            int a_l=a.length();
            int b_l=b.length();
            string tempstr;
            tempstr.append(a,0,b_l-1);
            for(int i=b_l-1;i<a_l;i++)
            {
                tempstr=tempstr+a[i];
                tempstr.erase(0,tempstr.find_first_not_of('0'));
                if(tempstr.empty())
                    tempstr="0";
                for(char ch='9';ch>='0';ch--)
                {
                    string str,tmp;
                    str=str+ch;
                    tmp=mul(b,str);
                    if(abs_cmp(tmp,tempstr)<=0)
                    {
                        quotient=quotient+ch;
                        tempstr=sub(tempstr,tmp);
                        break;
                    }
                }
            }
            residue=tempstr;
        }
        quotient.erase(0,quotient.find_first_not_of('0'));
        if(quotient.empty()) quotient="0";
    }


	operator bool() { return !(number_=="0"); }

	operator double()
	{
	    double res=0;
	    int num_len = number_.length();
	    for (int i=0; i<num_len; ++i)
            res = res*BASE + (number_[i] - '0');
        if (sgn_) res=0.0-res;
        return res;
	}

	bool operator!=(const BigInt &b) const { return !((number_==b.number_) && (sgn_==b.sgn_)); }
	bool operator==(const BigInt &b) const { return !(*this!=b); }
	bool operator<(const BigInt &b) const
	{
	    if (sgn_ != b.sgn_) return !(sgn_ < b.sgn_);
	    else
        {
            if (!sgn_)
            {
                int a_l=number_.length();
                int b_l=b.number_.length();
                if (a_l!=b_l) return (a_l<b_l);
                else
                {
                    for (int i=0; i<a_l; ++i)
                    {
                        if (number_[i]!=b.number_[i]) return number_[i]<b.number_[i];
                    }
                    return false;
                }
            }
            else
            {
                int a_l=number_.length();
                int b_l=b.number_.length();
                if (a_l!=b_l) return !(a_l<b_l);
                else
                {
                    for (int i=0; i<a_l; ++i)
                    {
                        if (number_[i]!=b.number_[i]) return !(number_[i]<b.number_[i]);
                    }
                    return false;
                }
            }
        }
	}

	BigInt &operator=(const BigInt &b)
	{
	    sgn_=b.sgn_;
	    number_=b.number_;
	    return *this;
	}

	BigInt operator+(const BigInt &obj) const
	{
	    if(number_=="0" && obj.number_=="0") { return BigInt("0"); }
	    BigInt res;
	    string a="",b="";
        a=number_;
        b=obj.number_;
        if(sgn_ && obj.sgn_)
        {
            res.number_=add(a,b);
            res.sgn_=1;
        }
        else if(!sgn_ && !obj.sgn_)
        {
            res.number_=add(a,b);
            res.sgn_=0;
        }
        else if(sgn_ && !obj.sgn_)
        {
            if(abs_cmp(a,b)==1)
            {
                res.number_=sub(a,b);
                res.sgn_=1;
            }
            else if(abs_cmp(a,b)==0)
            {
                res.number_="0";
                res.sgn_=0;
            }
            else if(abs_cmp(a,b)==-1)
            {
                res.number_=sub(b,a);
            }
        }
        else if(!sgn_ && obj.sgn_)
        {
            if(abs_cmp(a,b)==1)
            {
                res.number_=sub(a,b);
            }
            else if(abs_cmp(a,b)==0)
            {
                res.number_="0";
                res.sgn_=0;
            }
            else if(abs_cmp(a,b)==-1)
            {
                res.number_=sub(b,a);
                res.sgn_=1;
            }
        }
        return res;
	}


	BigInt operator-(const BigInt &obj) const
	{
	    if(number_=="0" && obj.number_=="0") { return BigInt("0"); }
	    string a="",b="";
	    BigInt res;
        a=number_;
        b=obj.number_;
        if(!sgn_ && !obj.sgn_)
        {
            if(abs_cmp(a,b)==1)
            {
                res.number_=sub(a,b);
            }
            if(abs_cmp(a,b)==0)
            {
                res.number_="0";
                res.sgn_=0;
            }
            if(abs_cmp(a,b)==-1)
            {
                res.number_=sub(b,a);
                res.sgn_=1;
            }
        }
        else if(sgn_ && obj.sgn_)
        {
            if(abs_cmp(a,b)==1)
            {
                res.number_=sub(a,b);
                res.sgn_=1;
            }
            if(abs_cmp(a,b)==0)
            {
                res.number_="0";
                res.sgn_=0;
            }
            if(abs_cmp(a,b)==-1)
            {
                res.number_=sub(b,a);
            }

        }
        else if(!sgn_ && obj.sgn_)
        {
            res.number_=add(a,b);
        }
        else if(sgn_ && !obj.sgn_)
        {
            res.number_=add(a,b);
            res.sgn_=1;
        }
        return res;
	}

	BigInt operator*(const BigInt &obj) const
	{
	    if(number_=="0" || obj.number_=="0") { return BigInt("0"); }
	    string a="",b="";
	    BigInt res;
        a=number_;
        b=obj.number_;
        if( !((sgn_)^(obj.sgn_)) ) { res.number_=mul(a,b); }
        else if( (sgn_)^(obj.sgn_) ) { res.number_=mul(a,b); res.sgn_=1; }
        return res;
	}

	BigInt operator/(const BigInt &obj) const
	{
        assert(obj.number_!="0" && "divided by zero");
        if(number_=="0") { return BigInt("0"); }
        string a="",b="",resi="";
        BigInt res;
        a=number_;
        b=obj.number_;
        if( !((sgn_)^(obj.sgn_)) ) { div(a,b,res.number_,resi); }
        else if( (sgn_)^(obj.sgn_) ) { div(a,b,res.number_,resi); res.sgn_=1; }
        if (res.number_=="0" || res.number_=="") { res.number_="0"; res.sgn_=0; }
        return res;
    }

	BigInt operator%(const BigInt &obj) const
	{
	    assert(obj.number_!="0" && "divided by zero");
        if(number_=="0") { return BigInt("0"); }
        string a="",b="",quo="";
        BigInt res;
        a=number_;
        b=obj.number_;
        if( !((sgn_)^(obj.sgn_)) ) { div(a,b,quo,res.number_); }
        else if( (sgn_)^(obj.sgn_) ) { div(a,b,quo,res.number_); res.sgn_=1; }
        if (res.number_=="0" || res.number_=="") { res.number_="0"; res.sgn_=0; }
        return res;
	}

	friend BigInt max(const BigInt &a, const BigInt &b)
	{
        BigInt res;
        if (!a.sgn_ && !b.sgn_) res = (abs_cmp(a.number_,b.number_)==1)?a:b;
        else if (!a.sgn_ && b.sgn_) res = a;
        else if (a.sgn_ && !b.sgn_) res = b;
        else if (a.sgn_ && b.sgn_) res = (abs_cmp(a.number_,b.number_)==-1)?a:b;
        return res;
	}

	friend BigInt min(const BigInt &a, const BigInt &b)
	{
	    BigInt res;
        if (!a.sgn_ && !b.sgn_) res = (abs_cmp(a.number_,b.number_)==-1)?a:b;
        else if (!a.sgn_ && b.sgn_) res = b;
        else if (a.sgn_ && !b.sgn_) res = a;
        else if (a.sgn_ && b.sgn_) res = (abs_cmp(a.number_,b.number_)==1)?a:b;
        return res;
	}

	void print() const
	{
	    if (sgn_) cout<<'-';
	    cout<<number_;
	}

    BigInt minus()
    {
        BigInt tmp=*this;
        if (tmp.number_!="0")
            tmp.sgn_=!tmp.sgn_;
        else tmp.sgn_=0;
        return tmp;
    }

    BigInt abs()
    {
        BigInt tmp=*this;
        tmp.sgn_=0;
        return tmp;
    }
};


#endif // bigint_h
