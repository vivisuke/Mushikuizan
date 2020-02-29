﻿#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <assert.h>

using namespace std;

typedef const char cchar;

bool isMatch(const string &t, int v)		//	計算結果(t) と v がマッチしているか？
{
	//return t == to_string(v);
	return atoi(t.c_str()) == v;
}
bool isMatchEx(const string &t, int v)		//	計算結果(t) と v がマッチしているか？ t は'*' を含んでいてもよい
{
	const auto vs = to_string(v);
	if( vs.size() != t.size() ) return false;
	for (int i = 0; i != t.size(); ++i) {
		if( t[i] != '*' && t[i] != vs[i] )
			return false;
	}
	return true;
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
void printQuest(const string& txt, int len, bool nl = true)
{
	static cchar *digTxt[] = {"０", "１", "２", "３", "４", "５", "６", "７", "８", "９", };
	if( len > txt.size() )
		cout << string((len-txt.size())*2, ' ');
	for(auto ch: txt) {
		if( isdigit(ch) ) cout << digTxt[ch - '0'];
		else cout << "□";
	}
	if( nl )
		cout << "\n";
}
bool solveAdd(vector<string> &vs, int row, int col)		//	row行、col文字目の次から決めていく
{
	for (;;) {
		auto ch = vs[row][++col];
		if( ch == '*' ) {
			for (ch = !col ? '1' : '0'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				if( solveAdd(vs, row, col) )
					return true;
			}
			vs[row][col] = '*';
			return false;		//	解無し
		}
		if( ch >= '0' && ch <= '9' ) continue;
		if( ch == '\0' ) {
			if( ++row != vs.size() ) {
				col = -1;
				continue;
			}
			//	全てが確定した場合
			return checkAdd(vs /*, true*/);
		}
		//	上記以外の文字はスキップ
	}
}
bool solveAdd(std::vector<std::string>& vs)
{
	return solveAdd(vs, 0, -1);
}
void printAddQuest(const vector<string>& vs)
{
	int mxlen = 0;
	for(auto& txt: vs)
		mxlen = max(mxlen, (int)txt.size());
	for (int i = 0; i < (int)vs.size() - 2; ++i) {
		printQuest(vs[i], mxlen);
	}
	cout << "＋";
	printQuest(vs[vs.size()-2], mxlen-1);
	cout << string(mxlen*2, '-') << "\n";
	printQuest(vs.back(), mxlen);
	cout << "\n";
}
//		A*B
bool checkMul(vector<string> &vs , bool fill = false)	//	fill: 答えを埋める
{
	int A = atoi(vs[0].c_str());		//	Aの値
	const auto &bstr = vs[1];		//	B文字列
	int ln = 2;
	for (int i = bstr.size(); --i >= 0; ++ln) {	//	Bの下の桁から順に掛け算を行う
		if( bstr[i] != '*' ) {
			int d = bstr[i] - '0';
			//auto s = to_string(d*A);
			if( !d*A || !isMatchEx(vs[ln], d*A) )		//	先頭の数字は０以外、vs[ln] は '*' を含んでいてもよい
				return false;
		}
	}
	int B = atoi(bstr.c_str());			//	Bの値
	if( !isMatchEx(vs[ln], B * A) )
		return false;
#if	1
	if( fill ) {
		ln = 2;
		for (int i = bstr.size(); --i >= 0; ++ln) {
			int d = bstr[i] - '0';
			vs[ln] = to_string(d*A);
		}
		vs[ln] = to_string(B*A);
	}
#endif
	return true;
}
void test_checkMul()
{
	vector<string>vs1 = {"11", "11", "11", "11", "121"}; assert( checkMul(vs1) );
	vector<string>vs2 = {"55", "55", "275", "275", "3025"}; assert( checkMul(vs1) );
}
void printMulQuest(const vector<string>& vs)
{
	int mxlen = 0;
	for(auto& txt: vs)
		mxlen = max(mxlen, (int)txt.size());
	printQuest(vs[0], mxlen);
	cout << "＊";
	printQuest(vs[1], mxlen -1);
	cout << string(mxlen*2, '-') << "\n";
	for (int i = 2; i < vs.size() - 1; ++i) {
		printQuest(vs[i], mxlen -(i - 2));
	}
	cout << string(mxlen*2, '-') << "\n";
	printQuest(vs.back(), mxlen);
	cout << "\n";
}
bool solveMul(vector<string> &vs, int row, int col)		//	row行、col文字目の次から決めていく
{
	for (;;) {
		auto ch = vs[row][++col];
		if( ch == '*' ) {
			for (ch = !col ? '1' : '0'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				if( solveMul(vs, row, col) )
					return true;
			}
			vs[row][col] = '*';
			return false;		//	解無し
		}
		if( ch >= '0' && ch <= '9' ) continue;
		if( ch == '\0' ) {
			if( ++row < 2 ) {
				col = -1;
				continue;
			}
			//	A * B が確定した場合
			return checkMul(vs, true);
		}
		//	上記以外の文字はスキップ
	}
}
bool solveMul(std::vector<std::string>& vs)
{
	return solveMul(vs, 0, -1);
}
/*
	　　A
	　－－－－－
	Ｂ）C
		.....
	　－－－－－
			R

	C/B = A・・・R		or A*B + R = C

*/
void printDivQuest(const vector<string>& vs)
{
	int maxlen = vs[1].size() + 1 + vs[2].size();		//	1 for '）'
	printQuest(vs[0], maxlen);
	cout << string(vs[1].size()*2, ' ') << string(vs[2].size()*2 + 2, '-')<< "\n";
	printQuest(vs[1], 0, false);
	cout << "）";
	printQuest(vs[2], 0);
	for (int i = 3; i != vs.size() - 1; ++i) {
		printQuest(vs[i], maxlen - (vs.size()-i)/2 + 1);
		if( (i%2) == 1 )
			cout << string(vs[1].size()*2, ' ') << string(vs[2].size()*2 + 2, '-')<< "\n";
	}
	printQuest(vs.back(), maxlen);
	cout << "\n";
}
bool checkDiv(vector<string> &vs , bool fill = false)	//	fill: 答えを埋める
{
	const auto &astr = vs[0];
	int A = atoi(astr.c_str());		//	商
	int B = atoi(vs[1].c_str());	//	除数
	int R = atoi(vs.back().c_str());		//	余り
	int C = A * B + R;					//	被除数
	if( !isMatchEx(vs[2], C) )		//	答えが合っているか？
		return false;
	int sc = 1;
	for (int i = 1; i != astr.size(); ++i) {
		sc *= 10;
	}
	int sc0 = sc;
	int ln = 3;
	for (int i = 0; i != astr.size(); ++i, ln+=2) {
		int d = astr[i] - '0';
		if( !isMatchEx(vs[ln], d*B) )
			return false;
		C -= d*B*sc;
		if( (sc /= 10) == 0 ) sc = 1;
		if( !isMatchEx(vs[ln+1], C/sc) )
			return false;
	}
	if( C != R )
		return false;
	if( fill ) {
		C = A * B + R;					//	被除数
		vs[2] = to_string(C);
		int ln = 3;
		sc = sc0;
		for (int i = 0; i != astr.size(); ++i, ln+=2) {
			int d = astr[i] - '0';
			vs[ln] = to_string(d*B);
			C -= d*B*sc;
			if( (sc /= 10) == 0 ) sc = 1;
			vs[ln+1] = to_string(C/sc);
		}
	}
	return true;
}
bool solveDiv(vector<string> &vs, int row, int col, bool fill = true)		//	row行、col文字目の次から決めていく
{
	for (;;) {
		if( row == vs.size() ) //	A, B, R が確定した場合
			return checkDiv(vs, fill);
		if (row >= 2 && row < vs.size() - 1) {
			++row;
			continue;		//	商、計算途中はスキップ
		}
		auto ch = vs[row][++col];
		if( ch == '*' ) {
			for (ch = !col ? '1' : '0'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				if( solveDiv(vs, row, col) )
					return true;
			}
			vs[row][col] = '*';
			return false;		//	解無し
		}
		//if( ch >= '0' && ch <= '9' ) continue;
		if( ch == '\0' ) {
			++row;
			col = -1;
		}
	}
}
bool solveDiv(std::vector<std::string>& vs)
{
	return solveDiv(vs, 0, -1);
}
int main()
{
	test_isMatch();
	test_checkAdd();
	test_checkMul();
	//
#if	0
	vector<string>vs1 = {"45", "**", "48", "*05"};
	printAddQuest(vs1);
	if( solveAdd(vs1) ) printAddQuest(vs1);
	else cout << "can't solve\n";
	//
	vector<string>vs2 = {"471", "*74", "885", "*1**"};
	printAddQuest(vs2);
	if( solveAdd(vs2) ) printAddQuest(vs2);
	else cout << "can't solve\n";
	//
	vector<string> qm0 = {"11", "11", "**", "**", "***"};
	printMulQuest(qm0);
	if( solveMul(qm0) ) printMulQuest(qm0);
	else cout << "can't solve\n\n";
	//
	vector<string> qm1 = {"**", "16", "13*", "*2", "35*"};
	printMulQuest(qm1);
	if( solveMul(qm1) ) printMulQuest(qm1);
	else cout << "can't solve\n\n";
	//
	vector<string> qm2 = {"**", "98", "***", "**2", "4***"};
	printMulQuest(qm2);
	if( solveMul(qm2) ) printMulQuest(qm2);
	else cout << "can't solve\n\n";
#endif
	//
	vector<string> qd0 = {"1*", "49", "6**", "49", "19*", "1*6", "0"};
	printDivQuest(qd0);
	if( solveDiv(qd0) ) printDivQuest(qd0);
	else cout << "can't solve\n\n";
	//
	vector<string> qd1 = {"1*", "4*", "6**", "49", "19*", "1*6", "0"};
	printDivQuest(qd1);
	if( solveDiv(qd1) ) printDivQuest(qd1);
	else cout << "can't solve\n\n";
	//
	vector<string> qd2 = {"***", "***", "*2**7*", "***1", "****", "**8*", "****", "6***", "5"};
	printDivQuest(qd2);
	if( solveDiv(qd2) ) printDivQuest(qd2);
	else cout << "can't solve\n\n";
	//
    std::cout << "OK\n";
}
