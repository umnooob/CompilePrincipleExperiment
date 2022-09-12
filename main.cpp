// 编译原理.cpp : 定义控制台应用程序的入口点。
//2191110306
#include<vector>
#include<map>
#include<iostream>
#include"LexicalAnalysis.h"
#include<string>
#include"SyntacticAnalysis.h"
#include <fstream>
using namespace std;
int main()
{
	
	LexicalAnalysis a;
	SyntacticAnalysis b("程序");
	a.LexicalAnalysisFromFile("testprogram.txt");
	//读取文法P、S、V、T
	cout << "读取文法P、S、V、T" << endl;
	b.getG("grammar.txt");
	system("pause");
	//输出First
	cout << "First" << endl;
	b.getFirst();
	for (auto iter = b.First.begin(); iter != b.First.end(); iter++) {
		cout << iter->first << " : ";
		for (auto value : iter->second)
			cout << value << " ";
		cout << endl;
	}
	//输出Follow集
	cout << "Follow" << endl;
	b.getFollow();
	for (auto iter = b.Follow.begin(); iter != b.Follow.end(); iter++) {
		cout << iter->first << " : ";
		for (auto value : iter->second)
			cout << value << " ";
		cout << endl;
	}


	//构建GOTO表及Action表
	b.get_SLR1_table();
	cout << "ACTION:" << endl;
	for (int i = 0; i<b.Action.size(); i++)
	{
		cout << i << ":\t";

		for (auto iter = b.Action[i].begin(); iter != b.Action[i].end(); iter++) {
			cout << iter->first << " : " << iter->second << "\t";
		}
		cout << endl;
	}

	cout << "GOTO:" << endl;
	for (int i = 0; i<b.Goto.size(); i++)
	{
		cout << i << ":\t";
		for (auto iter = b.Goto[i].begin(); iter != b.Goto[i].end(); iter++) {
			cout << iter->first << " : " << iter->second << "\t";
		}
		cout << endl;
	}

	//词法分析,获取输入程序
	cout << "词法分析过程" << endl;
	a.LexicalAnalysisFromFile("testprogram.txt");

	vector<pair<string, string>> tokens = a.getTokens("testprogram.txt");
	cout << "Tokens：" << endl;
	for (auto token : tokens) {
		cout << token.first << " " << token.second << endl;
	}
	//LR分析
	cout << "LR分析" << endl;
	b.SLR1_analysis(tokens);
	system("pause");
	return 0;
}

