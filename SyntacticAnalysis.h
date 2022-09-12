#pragma once
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include <string>
#include<unordered_set>
#include<unordered_map>
#ifndef SYNTACTICANALYSIS_H
#define SYNTACTICANALYSIS_H
using namespace std;
//产生式
class Product {
public:
	string left;
	vector<string> right;
	Product(string left, vector<string> right)
	{
		this->left = left;
		this->right = right;
	}

};
// LR(0)项目，即带圆点的产生式
// 三个变量分别表示产生式左部、右部和圆点坐标（在对应坐标字符的左侧）

class Item
{
public:
	string left;
	vector<string> right;
	int index;
	Item(string left, vector<string> right, int index)
	{
		this->left = left;
		this->right = right;
		this->index = index;
	}
	bool operator==(const Item &i) const {
		return (left == i.left) && (right == i.right) && (index == i.index);
	}
	void print()
	{
		cout << this->left << "——>";
		for (int i = 0; i < right.size(); i++)
		{
			if (i == index)
				cout << ".";
			cout << right[i]<<" ";
		}
		if(index==right.size())
			cout << ".";
		cout << endl;
	}
};

class SyntacticAnalysis
{
public:
	SyntacticAnalysis(string S);
	void getG(string filePath);//获取文法，得到P、T、V、S
	void getFollow();//构建Follow集
	void getFirst();//构建First集
	unordered_set<string> get_FIRST_alpha(vector<string> a);//first[beta]
	void get_SLR1_table();//构建Action表与Goto表
	vector<Item> cal_Closure(vector<Item> I);//求项目集I的闭包
	vector<Item> GO(vector<Item> I, string X);//Go转移函数，I关于X的后继项目集
	vector<vector<Item>> getSLR1Collection();//计算项目集规范族
	bool SLR1_analysis(vector<pair<string, string>>tokens);//SLR1分析过程，可视化输入区与栈的变化


	unordered_map<string, unordered_set<string>> First;
	unordered_map<string, unordered_set<string>> Follow;
	vector<unordered_map<string, string>> Goto;
	vector<unordered_map<string, string>> Action;

	vector<Product> P;
	unordered_set<string> T;
	unordered_set<string> V;
	string S;

};

#endif // SYNTACTICANALYSIS_H
