#include "SyntacticAnalysis.h"
#include<iostream>
#include <string>
#include <fstream>
#include<set>
using namespace std;
// 使用字符分割
void Stringsplit(const string& str, const char split, vector<string>& res)
{
	if (str == "")		return;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + split;
	size_t pos = strs.find(split);

	// 若找不到内容则字符串搜索函数返回 npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(split);
	}
}
// 使用字符串分割
void Stringsplit(const string& str, const string& splits, vector<string>& res)
{
	if (str == "")		return;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + splits;
	size_t pos = strs.find(splits);
	int step = splits.size();

	// 若找不到内容则字符串搜索函数返回 npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + step, strs.size());
		pos = strs.find(splits);
	}
}
SyntacticAnalysis::SyntacticAnalysis(string S)//构造函数，初始化S
{
	//ctor
	this->S = S;

}

void SyntacticAnalysis::getG(string filePath)
{
	string line;
	ifstream in(filePath);
	while (getline(in, line))//读取产生式P
	{
		if (line == "")
			continue;
		if (line[0] == '#')
			continue;
		int index = line.find("->");
		string left = line.substr(0, index - 1);
		string right = line.substr(index + 3, line.length() - 3 - index);
		vector<string> rightList;
		Stringsplit(right, " ", rightList);
		P.push_back(Product(left, rightList));//保存产生式P
		V.insert(left);//保存变量V


	}


	
	for (int i = 0; i<P.size(); i++) {//保存非变量（终结符）T
		for (auto r : P[i].right) {
			if (!(V.count(r)) && (r != "ε")) {
				T.insert(r);
			}

		}
	}



	//打印内容

	cout << "P:" << endl;
	for (auto p : P) {
		cout << p.left << "——>";
		for (auto r : p.right) {
			cout << " " << r;
		}
		cout << endl;
	}
	cout << endl;
	cout << "V:" << endl;
	for (auto v : V)
		cout << v << " ";
	cout << endl;
	cout << endl;
	cout << "T:" << endl;
	for (auto t : T)
		cout << t << " ";
	cout << endl;
	cout << endl;
}
//合并first或follow集
//except_epsilon代表是否去除ε
bool union_FIRST_or_FOLLOW(unordered_map<string, unordered_set<string>> &F, string X, string Y, bool except_epsilon)
{
	unordered_set<string> setX = F[X];
	unordered_set<string> setY;
	if (Y == "ε") {
		setY.insert("ε");
	}
	else
		setY = F[Y];
	if (except_epsilon) {
		auto iter = setY.find("ε");
		if (iter != end(setY))
			setY.erase(iter);
	}
	int before = F[X].size();
	//合并集合
	for (auto y : setY)
	{
		F[X].insert(y);
	}
	int after = F[X].size();
	if (before<after)
		return true;
	else
		return false;
}


//求G的所有FIRST(X)，X∈(V U T)
void SyntacticAnalysis::getFirst()
{
	//初始化First集,终结符为其自身
	for (auto t : T) {
		First[t].insert(t);
	}

	//第一阶段
	for (Product product : P) {
		string left = product.left;
		if (product.right[0] == "ε"&&product.right.size() == 1)
			First[left].insert("ε");
		if (T.count(product.right[0]))
			First[left].insert(product.right[0]);
	}
	//第二阶段
	bool increase_flag = true;
	while (increase_flag)
	{
		increase_flag = false;
		for (Product product : P)
		{
			string X = product.left;
			vector<string> Y = product.right;
			//if (X→Y…∈P and Y∈V) then FIRST(X):= FIRST(X)∪(FIRST(Y)-{ε})
			if (V.count(product.right[0]))
			{
				if (union_FIRST_or_FOLLOW(First, X, Y[0], true))
					increase_flag = true;
			}

			//if (X→Y1…Yn∈P and Y1...Yi-1→ε) then for k=2 to i do FIRST(X):= FIRST(X)∪(FIRST(Yk)-{ε})
			if (Y.size() == 1)
				continue;
			else
			{
				bool epsilon_flag = false;
				for (string y : Y)
				{
					epsilon_flag = false;
					if (First[y].count("ε"))
					{
						epsilon_flag = true;
						if (union_FIRST_or_FOLLOW(First, X, y, true))
							increase_flag = true;
					}
					if (!epsilon_flag)
					{
						if (union_FIRST_or_FOLLOW(First, X, y, true))
							increase_flag = true;
						break;
					}
				}
				//如果右部每一个非终结符都能推出ε，就给FIRST(X)加上ε
				if (epsilon_flag)
					if (union_FIRST_or_FOLLOW(First, X, "ε", false))
						increase_flag = true;
			}
		}

	}

}
//计算Follow集时的B后的First[beta]
unordered_set<string> SyntacticAnalysis::get_FIRST_alpha(vector<string> a)
{

	if (a.size() == 1)
	{
		if (a[0] == "ε")
			First["ε"].insert("ε");
		return First[a[0]];
	}
	string string_a = "";
	for (int i = 0; i<a.size(); i++)
	{
		if (i<a.size() - 1)
			string_a = string_a + a[i] + " ";
		else
			string_a = string_a + a[i];
	}
	bool flag_epsilon = false;//是否存在ε
	for (auto X : a)
	{
		flag_epsilon = false;
		if (First[X].count("ε"))
		{
			flag_epsilon = true;
			union_FIRST_or_FOLLOW(First, string_a, X, true);
		}
		if (!flag_epsilon)
		{
			union_FIRST_or_FOLLOW(First, string_a, X, true);
			break;
		}

	}
	if (flag_epsilon)
		union_FIRST_or_FOLLOW(First, string_a, "ε", false);
	return First[string_a];

}

void SyntacticAnalysis::getFollow()
{
	Follow[S].insert("#");
	bool change_flag = true;

	while (change_flag)
	{
		change_flag = false;
		for (Product product : P)
		{
			string A = product.left;
			vector<string> Y = product.right;
			int index = 0;
			for (string B : Y)
			{

				index++;
				if (!V.count(B))
					continue;
				if (index == Y.size())//最后一个非终结符
					change_flag = union_FIRST_or_FOLLOW(Follow, B, A, false);
				else
				{
					auto b = Y.begin() + index;
					auto e = Y.end();
					vector<string> beta = vector<string>(b, e);
					unordered_set<string> first_beta = get_FIRST_alpha(beta);

					if (first_beta.count("ε"))
						change_flag = union_FIRST_or_FOLLOW(Follow, B, A, false);
					int before = Follow[B].size();
					for (auto ele : first_beta)
					{
						if (ele != "ε")
							Follow[B].insert(ele);
					}
					int after = Follow[B].size();
					if (before<after)
						change_flag = true;
				}
			}
		}
	}
}
//求I的闭包
vector<Item> SyntacticAnalysis::cal_Closure(vector<Item> I)
{
	vector<Item> res = I;
	if (I.size() == 0)
		return res;
	int index = 0;
	while (index <= res.size() - 1)
	{
		Item item = res[index];
		index++;
		if (item.index >= item.right.size())
			continue;
		string B = item.right[item.index];
		if (V.count(B))//B是V
		{
			for (Product product : P)//对所有产生式
			{
				if (product.left == B)//产生式左端是B
				{
					Item temp = Item(product.left, product.right, 0);
					bool flag = true;
					for (Item t : res) {//是否已包含在结果中
						if (t == temp)
							flag = false;
					}
					if (flag)
						res.push_back(temp);
				}
			}
		}
	}
	
	return res;
}
//I关于X的后继项目集
vector<Item> SyntacticAnalysis::GO(vector<Item> I, string X)
{
	vector<Item> J ;
	for (Item item : I)
	{
		if (item.index >= item.right.size())
			continue;
		string B = item.right[item.index];
		if (B == X)
			if (V.count(B) || T.count(B))
				J.push_back(Item(item.left, item.right, item.index + 1));

	}
	return cal_Closure(J);
}

vector<vector<Item>> SyntacticAnalysis::getSLR1Collection()//计算项目集规范族
{
	vector<vector<Item>> res;
	vector<Item> I;
	I.push_back(Item(P[0].left, P[0].right, 0));
	res.push_back(cal_Closure(I));
	int beforesize = res.size();
	int aftersize = res.size();
	do {
		beforesize = aftersize;
		int index = 0;
		while (index <= res.size() - 1)
		{
			vector<Item> I = res[index];
			index++;
			for (string X : V)
			{
				vector<Item> J = GO(I, X);
				if (J.size() > 0)
				{
					bool E_flag = true;
					for (vector<Item> K : res)
						if (K == J)
							E_flag = false;
					if (E_flag)
						res.push_back(J);
				}
			}
			for (string X : T)
			{
				vector<Item> J = GO(I, X);
				if (J.size() > 0)
				{
					bool E_flag = true;
					for (vector<Item> K : res)
						if (K == J)
							E_flag = false;
					if (E_flag)
						res.push_back(J);
				}
			}
		}
		aftersize = res.size();
	} while (beforesize<aftersize);//项目集规范族不再增大
	return res;
}

void SyntacticAnalysis::get_SLR1_table()
{
	vector<string> temp;
	temp.push_back(S);
	P.insert(P.begin(), Product(S + "'", temp));
	V.insert(V.begin(), S + "'");
	cout << "Collection: " << endl;
	vector<vector<Item>> C = getSLR1Collection();
	int index = 0;
	for (auto v : C)
	{
		cout << "I" << index << endl;
		index++;
		for (auto i : v)
		{
			i.print();
		}
	}
	cout << "getSLR1Collection Finished" << endl;
	cout << "开始计算Action及Goto（1min）" << endl;
	unordered_map<string, string>temp_goto;
	for (string i : V)
	{
		temp_goto[i] = "";
	}
	for (int i = 0; i<C.size(); i++)
		Goto.push_back(temp_goto);
	unordered_map<string, string>temp_action;
	for (string i : T)
	{
		temp_action[i] = "";
	}
	temp_action["#"] = "";
	for (int i = 0; i<C.size(); i++)
		Action.push_back(temp_action);
	for (int k = 0; k<C.size(); k++)
	{
		vector<Item> I = C[k];
		for (Item item : I)
		{
			string character ;

			//圆点不在右部表达式的最右侧，即还不需要归约
			if (item.index<item.right.size())
			{
				character = item.right[item.index];
				for (int j = 0; j<C.size(); j++)
				{
					if (GO(I, character) == C[j])
					{
						if (T.count(character))//如果character是终结符
						{
							Action[k][character] = "S" + to_string(j);
						}
						else//如果是非终结符
						{
							Goto[k][character] = to_string(j);
						}
						break;
					}
				}
			}
			//圆点在右部表达式的最右侧，即需要归约
			else
			{
				//如果该表达式是S'->S，则在action表里添加acc
				if (item.left == S + "'")
				{
					Action[k]["#"] = "acc";
					continue;
				}
				//否则，找到P中对应的产生式序号ri加入Action表
				int m = P.size();
				for (int j = 1; j<m; j++)
				{
					if (P[j].left == item.left&&P[j].right == item.right)
					{
						unordered_set<string> FOLLOW_A = Follow[item.left];
						for (string t : T)
						{
							if (FOLLOW_A.count(t))
								Action[k][t] = "r" + to_string(j);

						}
						if (FOLLOW_A.count("#"))
							Action[k]["#"] = "r" + to_string(j);
						break;
					}
				}
			}
		}
	}
	

}

//SLR1分析过程，可视化输入区与栈的变化
//tokens为词法分析后的结果
bool SyntacticAnalysis::SLR1_analysis(vector<pair<string, string>> tokens)
{
	vector<int> state;//状态栈
	vector<string> character;//符号栈
	vector<string> buffer;//输入缓冲区
	state.push_back(0);
	character.push_back("#");
	for (auto token : tokens)//处理tokens
	{
		if (token.second == "标识符")
			buffer.push_back("id");
		else if (token.second == "常数")
			buffer.push_back("digit");
		else
			buffer.push_back(token.first);
	}
	buffer.push_back("#");
	int ip = 0;
	while (true)
	{
		cout << "符号栈:";
		for (string i : character)
			cout << i << " ";
		cout << endl;
		cout << "状态栈:";
		for (int i : state)
			cout << i << " ";
		cout << endl;
		cout << "输入缓冲区:";
		for (int i = buffer.size() - 1; i >= ip; i--)
			cout << buffer[i] << " ";
		cout << endl;
		if (ip > buffer.size())
			break;



		int S = state[state.size() - 1];
		string a = buffer[ip];
		string act = Action[S][a];
		cout << "动作：" << act << endl;
		if (act == "")
		{
			cout << "ERR分析失败" << endl;
			return false;
		}
		else if (act[0] == 'S')
		{
			//移进a
			cout << "移进" << a << endl;
			character.push_back(a);
			state.push_back(stoi(act.substr(1)));
			ip++;
		}
		else if (act[0] == 'r')
		{
			int k = stoi(act.substr(1));
			//第k个产生式进行归约
			Product product = P[k];
			int size = product.right.size();
			state.erase(state.end() - size, state.end());
			character.erase(character.end() - size, character.end());
			// state._Pop_back_n(size);
			// character._Pop_back_n(size);
			int _S = state[state.size() - 1];
			string A = product.left;
			character.push_back(A);
			state.push_back(stoi(Goto[_S][A]));
			cout << "使用产生式 " << A<<"——>";
			for (string i : product.right)
				cout << i << " ";
			cout << "进行归约" << endl;

		}
		else if (act == "acc")
		{
			cout << "分析成功，语法分析结束" << endl;
			return true;
		}

	}
	return false;
}


