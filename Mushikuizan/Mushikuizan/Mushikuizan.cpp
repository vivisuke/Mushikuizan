#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <assert.h>

using namespace std;

bool isMatch(const string &t, int v)		//	計算結果(t) と v がマッチしているか？
{
	//return t == to_string(v);
	return atoi(t.c_str()) == v;
}
void test_isMatch()
{
	assert( isMatch("314", 314) );
	assert( !isMatch("314", 315) );
	assert( !isMatch("3.14", 314) );
	assert( isMatch("1192", 1192) );
	assert( isMatch("123", 123) );
	assert( isMatch("0123", 123) );
}
//		vs[0] + vs[1] + ...vs[vs.size() - 2] が vs.vs[vs.size() - 1] と等しいかチェック
bool checkAdd(vector<string> &vs)
{
	assert( vs.size() >= 3 );
	int sum = 0;
	for (int i = 0; i != vs.size() - 1; ++i) {		//	vs[0] + vs[1] + ...vs[vs.size() - 2] を計算
		sum += atoi(vs[i].c_str());
	}
	return isMatch(vs.back(), sum);	//	加算結果が会っているか？
}
void test_checkAdd()
{
	vector<string>vs1 = {"12", "34", "46"}; assert( checkAdd(vs1) );
	vector<string>vs2 = {"12", "34", "56"}; assert( !checkAdd(vs2) );
	vector<string>vs3 = {"99", "34", "133"}; assert( checkAdd(vs3) );
	vector<string>vs4 = {"99", "34", "33"}; assert( !checkAdd(vs4) );
	vector<string>vs5 = {"12", "34", "56", "102"}; assert( checkAdd(vs5) );
}
void printQuest(const string& txt)
{
	static cchar *digTxt[] = {"０", "１", "２", "３", "４", "５", "６", "７", "８", "９", };
	for(auto ch: txt) {
		if( isdigit(ch) ) cout << digTxt[ch - '0'];
		else cout << "□";
	}
	cout << "\n";
}
void printAddQuest(const vector<string>& vs)
{
	for (int i = 0; i != vs.size() - 1; ++i) {
		printQuest(vs[i]);
	}
	cout << "----------";
		printQuest(vs.back());
}
int main()
{
	test_isMatch();
	test_checkAdd();
	//
	vector<string>vs1 = {"45", "**", "48", "*05"};
	printAddQuest(vs1);
	//
    std::cout << "OK\n";
}
