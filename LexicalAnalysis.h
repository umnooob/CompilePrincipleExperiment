#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<iostream>
#include<map>
#include<vector>
//词法分析程序
//首先定义种别码
/*
第一类：标识符   letter(letter | digit)*  无穷集
第二类：常数    (digit)+  无穷集
第三类：保留字(32)
auto       break    case     char        const      continue
default    do       double   else        enum       extern
float      for      goto     if          int        long
register   return   short    signed      sizeof     static
struct     switch   typedef  union       unsigned   void
volatile    while

第四类：界符  ‘/*’、‘//’、 () { } [ ] " "  '
第五类：运算符 <、<=、>、>=、=、+、-、*、/、^、

对所有可数符号进行编码：一符一类
<$,0>
<auto,1>
...
<while,32>
<+，33>
<-,34>
<*,35>
</,36>
<<,37>
<<=,38>
<>,39>
<>=,40>
<=,41>
<==,42>
<!=,43>
<;,44>
<(,45>
<),46>
<^,47>
<,,48>
<",49>
<',50>
<#,51>
<&,52>
<&&,53>
<|,54>
<||,55>
<%,56>
<~,57>
<<<,58>左移
<>>,59>右移
<[,60>
<],61>
<{,62>
<},63>
<\,64>
<.,65>
<?,66>
<:,67>
<!,68>
"[","]","{","}"
<常数99  ,数值>
<标识符100 ，标识符指针>


*/

/****************************************************************************************/
using namespace std;
class LexicalAnalysis
{
public:
	LexicalAnalysis();
	int searchReserve(char s[]);
	void filterResource(char r[], int pProject);//过滤源程序 去除注释及空格
	void Scanner(int &syn, char resourceProject[], char token[], int &pProject);//扫描过滤后程序
	void LexicalAnalysisFromFile(string filePath, string outputPath = "");//打印分析结果
	vector<pair<string, string>> getTokens(string filePath);//获取词法分析结果 First：词汇  Second：类型
private:
	//全局变量，保留字表
	static  char reserveWord[32][20];
	//界符运算符表,根据需要可以自行增加
	static  char operatorOrDelimiter[36][10];

	static char IDentifierTbl[1000][50];//标识符表
};

