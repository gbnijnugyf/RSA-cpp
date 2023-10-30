#include <iostream>
#include <random>
#include <cmath>
#include <time.h>
#include <string>
#include <algorithm>

#define n 10
#define LL long long int
using namespace std;
// 去掉前导0
string delzero(string a)
{
    while (a.length() > 1 && a[0] == '0')
        a.erase(0, 1);
    return a;
}
// 大数相乘
string bigIntPlus(string plusM1, string plusM2)
{

    string ret;
    if (plusM1.length() < plusM2.length())
    {
        string tmp = plusM1;
        plusM1 = plusM2;
        plusM2 = tmp;
    }

    int len1 = plusM1.length(), len2 = plusM2.length();
    for (int i = len2 - 1; i >= 0; --i)
    {

        string tmp(len1, '0'); // 存放相乘的中间结果
        int goBit = 0;
        for (int j = len1 - 1; j >= 0; --j)
        {

            int mid = (plusM1[j] - '0') * (plusM2[i] - '0') + goBit;
            tmp[j] = mid % 10 + '0';
            goBit = mid / 10;
        }
        if (goBit != 0)
            tmp.insert(0, string(1, goBit + '0'));

        for (int m = 0; m < len2 - 1 - i; ++m)
            tmp.push_back('0'); // 补位

        // 相乘后就相加  大数相加
        if (i == len2 - 1)
            ret = tmp;
        else
        {

            int goBit2 = 0;
            string s(tmp.length() - ret.length(), '0');
            ret = s + ret;
            for (int m = tmp.length() - 1; m >= 0; --m)
            {

                int mid = (tmp[m] - '0') + (ret[m] - '0') + goBit2;
                ret[m] = mid % 10 + '0';
                goBit2 = mid / 10;
            }

            if (goBit2 != 0)
                ret.insert(0, string(1, goBit + '0'));
        }
    }

    return delzero(ret);
}
// 大数相减，sub1 > sub2
string bigIntSub(string sub1, string sub2)
{
    string res;
    int len1 = sub1.length();
    int len2 = sub2.length();
    for (int i = len2 - 1, j = len1 - 1; i >= 0 && j >= 0; --i, --j)
    {
        // 判断是否发生借位
        if (sub1[j] >= sub2[i])
        {
            res.push_back(sub1[j] - sub2[i] + '0');
        }
        else
        {
            sub1[j - 1] -= 1;
            res.push_back(sub1[j] + 10 - sub2[i] + '0');
        }
    }
    for (int j = len1 - len2 - 1; j >= 0; --j)
    {
        res.push_back(sub1[j]);
    }
    reverse(res.begin(), res.end()); // 翻转
    int num = 0;
    for (int i = 0; i < res.size(); i++)
    {
        if (res[i] == '0')
        {
            num++;
        }
        else
        {
            res.erase(0, num); // 删除翻转s3之后前面的0
            break;
        }
    }
    return delzero(res);
}
int compare(string a, string b) // 判断两个正数的大小
{
    if (a.length() > b.length())
        return 1;
    if (a.length() < b.length())
        return -1;
    long int i;
    for (i = 0; i < a.length(); i++)
    {
        if (a.at(i) > b.at(i))
            return 1;
        if (a.at(i) < b.at(i))
            return -1;
    }
    return 0;
}
// 大数相加
string bigIntAdd(string add1, string add2)
{
    string strSum;
    int len1 = add1.size() - 1;
    int len2 = add2.size() - 1;
    int bit = 0; // 保存进位

    // 从结尾开始每位相加
    while (len1 >= 0 && len2 >= 0)
    {
        // 求每位的和(要把进位也加上)
        int tmpSum = add1[len1] - '0' + add2[len2] - '0' + bit;
        // 保存进结果
        strSum += tmpSum % 10 + '0';
        // 求进位
        bit = tmpSum / 10;
        --len1;
        --len2;
    }

    // 两个整数长度不相等(肯定有一个已经加完了,不需要再额外加if来判断，因为while就可以判断)
    while (len1 >= 0)
    {
        // 和上个while循环一样
        int tmpSum = add1[len1] - '0' + bit;
        strSum += tmpSum % 10 + '0';
        bit = tmpSum / 10;
        --len1;
    }
    while (len2 >= 0)
    {
        // 和上个while循环一样
        int tmpSum = add2[len2] - '0' + bit;
        strSum += tmpSum % 10 + '0';
        bit = tmpSum / 10;
        --len2;
    }

    // 最高位有进位
    if (bit != 0)
        strSum += bit + '0';

    // 反转
    reverse(strSum.begin(), strSum.end());

    return delzero(strSum);
}
// 大数相除
string bigIntDivide(string a, string b)
{
    if (b.length() == 1 && b.at(0) == 48)
        return "error";
    LL i, j;
    string c1, c2, d, e;
    // 两数相等
    if (compare(a, b) == 0)
        return "1";
    // 结果小于0，输出0
    if (compare(a, b) == -1)
    {
        return "0";
    }
    c1 = delzero(a);
    c2 = delzero(b);
    d = "";
    e = "";
    for (i = 0; i < c1.length(); i++)
    {
        j = 0;
        d = d + c1.at(i);
        d = delzero(d);
        while (compare(d, b) >= 0)
        {
            d = bigIntSub(d, b);
            d = delzero(d);
            j++;
        }
        // 四舍五入
        // if (i == c1.length() - 1 && ((float)stoi(d) / (float)stoi(c2)) >= 0.5)
        // {
        //     j++;
        // }
        e = e + "0";
        e.at(i) = j;
    }
    // 结果转换为字符串
    for (i = 0; i < e.length(); i++)
    {
        if (e.at(i) >= 10)
            e.at(i) += 87;
        if (e.at(i) < 10)
            e.at(i) += 48;
    }
    return delzero(e);
}
string bigIntMod(string a, string b) // 取模运算
{
    long int i, j = 0;
    string c1, c2, c3, d;
    if (a.at(0) == '-')
        j = 1;
    if (compare(a, b) == 0)
        return "0";
    if (compare(a, b) == -1)
    {
        return a;
    }
    c1 = delzero(a);
    c2 = delzero(b);
    d = "";
    for (i = 0; i < c1.length(); i++)
    {
        d = d + c1.at(i);
        d = delzero(d);
        while (compare(d, b) >= 0)
        {
            d = bigIntSub(d, b);
        }
    }
    if (j == 1)
        d = bigIntSub(b, d);
    return delzero(d);
}

string sqrtBigNumber(string num)
{
    string left = "1";
    string right = num;

    while (left != right)
    {
        string mid = bigIntDivide(bigIntAdd(left, right), "2");
        string square = bigIntPlus(mid, mid);

        if (square == num)
        {
            return mid;
        }
        else if (square < num)
        {
            left = mid;
        }
        else
        {
            right = bigIntSub(mid, "1");
        }
    }

    return left;
}
string multiply(string a,string b)//整数
{
	long int i,j,k,yao=0,kai;
	string c1,c2;
	string c3=a+b;
	if(a.at(0)=='-')
	{
		a.erase(0,1);
		yao++;
	}
	if(b.at(0)=='-')
	{
		b.erase(0,1);
		yao++;
	}
	a=delzero(a);
	b=delzero(b);
	if(a.at(0)==48||b.at(0)==48) return "0";
	if(a.length()>b.length())
	{
		c1=a;
		c2=b;
	}
	else
	{
		c1=b;
		c2=a;
	}
	reverse(c1.begin(),c1.end());
	reverse(c2.begin(),c2.end());
	for(i=0;i<c2.length();i++)
	{
		if(c2.at(i)>=48&&c2.at(i)<=57) c2.at(i)-=48;
		if(c2.at(i)>=97&&c2.at(i)<=122) c2.at(i)-=87;
	}
	for(i=0;i<c1.length();i++)
	{
		if(c1.at(i)>=48&&c1.at(i)<=57) c1.at(i)-=48;
		if(c1.at(i)>=97&&c1.at(i)<=122) c1.at(i)-=87;
	}
	for(i=0;i<c3.length();i++) c3.at(i)=0;
	for(i=0;i<c2.length();i++)
	{
		for(j=0;j<c1.length();j++)
		{
			kai=c2.at(i)*c1.at(j);
			c3.at(i+j+1)+=kai/n;
			c3.at(i+j)+=kai%n;
			for(k=i+j;k<c3.length()-1;k++)
			{
				if(c3.at(k)>=n) 
				{
					c3.at(k+1)+=c3.at(k)/n;
					c3.at(k)=c3.at(k)%n;
				}
				else
				{
					break;
				}
			}
		}
	}
	for(i=c3.length()-1;i>=0;i--)
	{
		if(c3.at(i)>0) break;
	}
	c3.erase(i+1,c3.length());
	for(i=0;i<c3.length();i++)
	{
		if(c3.at(i)>=10) c3.at(i)+=87;
		if(c3.at(i)<10) c3.at(i)+=48;
	}
	reverse(c3.begin(),c3.end());
	if(yao==1) c3="-"+c3;
	return c3;
}
