// 实验 加密与解密
// 实验要求：编程实现 RSA 加密，解密算法
// 算法介绍：RSA 算法由两个密钥，即公钥和私钥组成。
// 1）准备两个非常大的素数 p 和 q（转换成二进制后位数越多越难破解）
// 2）利用字符串模拟计算大素数 p 和 q 的乘积 n = pq；
// 3）同样方法计算 m = (p − 1)(q − 1)，这里 m 为 n 的欧拉函数；
// 4）找到一个数 e(1 < e < m)，满足 gcd(m, e) = 1；
// 5）计算 e 在模 m 域上的逆元 d（即满足 ed mod m = 1）；
// 6）公钥私钥生成完毕：(n, e)为公钥，(n, d)为私钥。
// RSA 加密：
// 对于明文x，用公钥(n, e)对x 加密的过程，就是将x 转换成数字（字符串的话取其ASCII码或者Unicode
// 值），然后通过幂取模计算出 y，其中 y 就是密文：
// RSA 解密：
// 对于密文 y，用私钥(n, d)对 y 解密的过程和加密类似，同样是计算幂取模：

#include <iostream>
#include <random>
#include <cmath>
#include <time.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#define N 10
#define LL long long int
using namespace std;

LL PrimeMIN = 100000000;  // 最小素数范围
LL PrimeMAX = 1000000000; // 最大素数范围
int PublicEMIN = 10;      // 最小e范围
int PublicEMAX = 100;     // 最大e范围

// 去掉前导0
string delzero(string a)
{
    while (a.length() > 1 && a[0] == '0')
        a.erase(0, 1);
    return a;
}
string multiply(string a, string b) // 整数
{
    long int i, j, k, yao = 0, kai;
    string c1, c2;
    string c3 = a + b;
    if (a.at(0) == '-')
    {
        a.erase(0, 1);
        yao++;
    }
    if (b.at(0) == '-')
    {
        b.erase(0, 1);
        yao++;
    }
    a = delzero(a);
    b = delzero(b);
    if (a.at(0) == 48 || b.at(0) == 48)
        return "0";
    if (a.length() > b.length())
    {
        c1 = a;
        c2 = b;
    }
    else
    {
        c1 = b;
        c2 = a;
    }
    reverse(c1.begin(), c1.end());
    reverse(c2.begin(), c2.end());
    for (i = 0; i < c2.length(); i++)
    {
        if (c2.at(i) >= 48 && c2.at(i) <= 57)
            c2.at(i) -= 48;
        if (c2.at(i) >= 97 && c2.at(i) <= 122)
            c2.at(i) -= 87;
    }
    for (i = 0; i < c1.length(); i++)
    {
        if (c1.at(i) >= 48 && c1.at(i) <= 57)
            c1.at(i) -= 48;
        if (c1.at(i) >= 97 && c1.at(i) <= 122)
            c1.at(i) -= 87;
    }
    for (i = 0; i < c3.length(); i++)
        c3.at(i) = 0;
    for (i = 0; i < c2.length(); i++)
    {
        for (j = 0; j < c1.length(); j++)
        {
            kai = c2.at(i) * c1.at(j);
            c3.at(i + j + 1) += kai / N;
            c3.at(i + j) += kai % N;
            for (k = i + j; k < c3.length() - 1; k++)
            {
                if (c3.at(k) >= N)
                {
                    c3.at(k + 1) += c3.at(k) / N;
                    c3.at(k) = c3.at(k) % N;
                }
                else
                {
                    break;
                }
            }
        }
    }
    for (i = c3.length() - 1; i >= 0; i--)
    {
        if (c3.at(i) > 0)
            break;
    }
    c3.erase(i + 1, c3.length());
    for (i = 0; i < c3.length(); i++)
    {
        if (c3.at(i) >= 10)
            c3.at(i) += 87;
        if (c3.at(i) < 10)
            c3.at(i) += 48;
    }
    reverse(c3.begin(), c3.end());
    if (yao == 1)
        c3 = "-" + c3;
    return c3;
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
// string sqrtBigNumber(string num)
// {
//     string guess = num;
//     string prevGuess;

//     while (guess != prevGuess)
//     {
//         prevGuess = guess;
//         string quotient = bigIntDivide(bigIntAdd(guess, bigIntDivide(num, "2")), "2");
//         guess = bigIntDivide(bigIntAdd(quotient, bigIntDivide(num, "2")), "2");
//     }

//     return guess;
// }

// 判断素数
bool isPrime(int n)
{
    if (n <= 1)
    {
        return false;
    }

    int sqrtN = sqrt(n);
    for (int i = 2; i <= sqrtN; ++i)
    {
        if (n % i == 0)
        {
            return false;
        }
    }

    return true;
}
// 判断素数
// bool isPrime_(string n)
// {
//     if (n.length() <= 1)
//     {
//         return false;
//     }

//     int sqrtN = sqrt(n);
//     for (int i = 2; i <= sqrtN; ++i)
//     {
//         if (bigIntMod(n, to_string(i)) == "0")
//         {
//             return false;
//         }
//     }

//     return true;
// }
// 生成素数
LL generateLargePrime(LL min, LL max, LL last = 0)
{
    srand((unsigned)time(NULL));

    LL prime = 0;
    do
    {
        prime = min + (rand() % (max - min));
    } while (!isPrime(prime) || prime == last);

    return prime;
}
// 生成素数
// string generateLargePrime_(string min, string max, string last = "0")
// {
//     srand((unsigned)time(NULL));

//     string prime = "0";
//     do
//     {
//         prime = bigIntAdd(min, bigIntMod(to_string(rand()), bigIntSub(max, min)));
//     } while (!isPrime_(prime) || prime == last);

//     return prime;
// }

string power(string base, string exponent, string n)
{
    // 终止条件：指数为0，返回1
    if (exponent == "0")
    {
        return "1";
    }

    // 递归计算base^(exponent/2)
    string result = power(base, bigIntDivide(exponent, "2"), n);
    result = bigIntMod(result, n);
    // 将结果平方
    result = bigIntPlus(result, result);
    result = bigIntMod(result, n);

    // 如果指数是奇数，再乘以base
    if ((exponent.back() - '0') % 2 == 1)
    {
        result = bigIntPlus(result, base);
        result = bigIntMod(result, n);
    }

    return result;
}

string quickpower(string x, string e, string n) // 快速指数算法x的e次方mod n
{
    // 进制转换——将指数e转换为二进制
    string temp;
    LL i;
    i = 0;
    while (1)
    {
        if (e.length() == 1 && e.at(0) == 48)
            break;
        temp = temp + "0";
        temp.at(i) = bigIntMod(e, "2").at(0);
        e = bigIntDivide(e, "2");
        i++;
    }
    reverse(temp.begin(), temp.end());

    // 快速指数算法
    e = temp;
    string d = "1";
    for (i = 0; i < e.length(); i++)
    {
        if (e.at(i) == 49) // 当前位为1时
            d = multiply(d, x);
        if (i != e.length() - 1) // 不是指数e的最后一位
            d = multiply(d, d);
        d = bigIntMod(d, n);
    }
    return d;
}
string bigIntPowerMode(string a, string b, string n)
{
    string r = a;
    string ex = b;
    while (ex != "1")
    {
        ex = bigIntSub(ex, "1");
        r = bigIntPlus(r, a);
        r = bigIntMod(r, n);
    }
    return r;
}
// 选择一个公钥指数 e：
// 公钥指数 e 是一个小于 m 且与 m 互质的正整数。通常情况下，选择一个较小的素数作为公钥指数，常见的选择是使用 65537（0x10001）。
int generatePublicE(int min, int max)
{
    return generateLargePrime(min, max);
}

string minuStr(string str)
{
    delzero(str);
    if (str == "0")
        return str;
    else
    {
        return "-" + str;
    }
}
bool isMinu(string str)
{
    if (str.at(0) == '-')
    {
        return true;
    }
    return false;
}

string *testExt_gcd(string a, string b, string arr[3]) // a>b
{
    if (b == "0")
    {
        // arr对应x、y、a
        arr[0] = "1";
        arr[1] = "0";
        arr[2] = a;
        return arr;
    }
    else
    {
        string aModb = bigIntMod(a, b);
        string *tempArr;
        tempArr = testExt_gcd(b, aModb, arr);

        string x = tempArr[0];
        // x = y
        tempArr[0] = tempArr[1];

        // y = x - (a // b) * y
        string aDvibPlusy;
        if (isMinu(tempArr[1]))
        {
            tempArr[1].erase(0, 1);
            aDvibPlusy = bigIntPlus(bigIntDivide(a, b), tempArr[1]);
            aDvibPlusy = minuStr(aDvibPlusy);
        }
        else
        {
            aDvibPlusy = bigIntPlus(bigIntDivide(a, b), tempArr[1]);
        }
        if (isMinu(x) && isMinu(aDvibPlusy)) // 均为负数 (a // b) * y - x
        {
            x.erase(0, 1);
            aDvibPlusy.erase(0, 1);
            if (compare(aDvibPlusy, x) >= 0)
            { // 结果为正数
                tempArr[1] = bigIntSub(aDvibPlusy, x);
            }
            else
            { // 结果为负数
                string r = bigIntSub(x, aDvibPlusy);
                tempArr[1] = minuStr(r);
            }
        }
        else if (isMinu(x) && !isMinu(aDvibPlusy)) // 仅x为负数，-((a // b) * y + x)
        {
            x.erase(0, 1);
            tempArr[1] = minuStr(bigIntAdd(aDvibPlusy, x));
        }
        else if (!isMinu(x) && isMinu(aDvibPlusy)) // 仅aDvibPlusy为负数， x + (a // b) * y
        {
            aDvibPlusy.erase(0, 1);
            tempArr[1] = bigIntAdd(aDvibPlusy, x);
        }
        else // 均为正数 x - (a // b) * y
        {
            if (compare(x, aDvibPlusy) >= 0)
            { // 结果为正数
                tempArr[1] = bigIntSub(x, aDvibPlusy);
            }
            else
            { // 结果为负数
                string r = bigIntSub(aDvibPlusy, x);
                tempArr[1] = minuStr(r);
            }
        }
        arr[0] = tempArr[0];
        arr[1] = tempArr[1];
        arr[2] = tempArr[2];
        return arr;
    }
}
string neg2pos_testExt_gcd(string a, string b, string arr[3])
{
    string m = a;
    string *r = testExt_gcd(a, b, arr);
    if (isMinu(r[1]))
    {
        r[1].erase(0, 1);
        r[1] = bigIntSub(m, r[1]);
    }
    // cout << r[0] << " " << r[1] << " " << r[2] << " " << endl;
    return r[1];
}

// 对字符串进行拆分
void convertToASCII(string str, int letter[])
{
    for (int i = 0; i < str.length(); i++)
    {
        char x = str.at(i);
        letter[i] = (x);
    }
}
// 加密
void encrypt(string plaintext, string e, string n, string ciphertext[]) // ciphertext长度为plaintext.length()
{
    int length = plaintext.length();
    int *letter = new int[length];
    convertToASCII(plaintext, letter);
    // string *ciphertext = new string[length];
    for (int i = 0; i < length; i++)
    {
        ciphertext[i] = quickpower(to_string(letter[i]), e, n);
        // cout << letter[i] << " " << ciphertext[i] << endl;
        // cout << letter[i] << endl;
    }
    return;
}
// 解密
string decrypt(string ciphertext[], string d, string n, int len)
{
    string pliantext;
    for (int i = 0; i < len; i++)
    {
        // ciphertext[i] = quickpower(ciphertext[i], d, n);
        string t = quickpower(ciphertext[i], d, n);
        // cout << ciphertext[i] << " " << t << endl;
        pliantext = pliantext + char(atoi(t.c_str()));

        // cout << letter[i] << endl;
    }
    return pliantext;
}
string fileOperate(string filePath)
{
    ifstream file(filePath); // 文件路径

    if (file.is_open())
    {
        stringstream buffer;
        buffer << file.rdbuf(); // 将文件内容读入缓冲区
        return buffer.str();
        file.close();
    }
    else
    {
        cout << "无法打开文件" << endl;
    }
    return "";
}

int main()
{
    // 生成p、q
    LL p = generateLargePrime(PrimeMIN, PrimeMAX);
    LL q = generateLargePrime(PrimeMIN, PrimeMAX, p);
    // p = 71;
    // q = 41;
    cout << "p: " << p << endl;
    cout << "q: " << q << endl;

    // 计算n=p*q
    string n = bigIntPlus(to_string(p), to_string(q));
    string m = bigIntPlus(bigIntSub(to_string(p), "1"), bigIntSub(to_string(q), "1"));
    cout << "n: " << n << endl;
    cout << "m: " << m << endl;
    // 选取e
    int e = generatePublicE(PublicEMIN, PublicEMAX);
    // e = 683;
    cout << "e: " << e << endl;
    // 计算 e 在模 m 域上的逆元 d
    string arr[3] = {"0"};
    string d = neg2pos_testExt_gcd(m, to_string(e), arr);
    cout << "d: " << d << endl;

    cout << "私钥：(" << n << "," << e << ")" << endl;
    cout << "公钥：(" << n << "," << d << ")" << endl;
    // 明文
    // string plaintext ="4444";
    string plaintext = fileOperate("plaintext.txt");
    cout << "明文：" << plaintext << endl;
    cout << "plaintext length: " << plaintext.length() << endl;
    // 加密
    string *ciphertext = new string[plaintext.length()];
    encrypt(plaintext, to_string(e), n, ciphertext);
    cout << "密文：";
    for (int i = 0; i < plaintext.length(); i++)
    {
        cout << ciphertext[i];
    }
    cout << endl;
    cout << endl;
    cout << "解密：" << decrypt(ciphertext, d, n, plaintext.length());
    return 0;
}
