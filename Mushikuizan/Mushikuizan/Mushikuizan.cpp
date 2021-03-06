﻿#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <assert.h>

using namespace std;

struct Pos {		//	行・桁 構造体
public:
	Pos(int row = 0, int col = 0)
		: m_row(row)
		, m_col(col)
	{
	}
public:
	int		m_row;
	int		m_col;
};

random_device g_rd;
#if	0
mt19937 g_mt(g_rd());
#else
mt19937 g_mt(4);
#endif

typedef const char cchar;

int g_cnt = 0;		//	解の数
char g_alphabet[26];		//	'A'-'Z' の値（'0'～'9'）、-1 for 未使用
bool g_used[10];			//	’A'-'Z' に 0-9 を使用済みかのフラグ

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
		//if( t[i] != '*' && t[i] != vs[i] )		//	'*' はすべての文字とマッチ
		//	return false;
		if( t[i] == '*' ) {
		} else if( isupper(t[i]) ) {
			if( g_alphabet[t[i] - 'A'] >= 0 ) {
				if( g_alphabet[t[i] - 'A'] != vs[i] )
					return false;
			} else {
				//	undone:	要検索
			}
		} else {
			if( t[i] != vs[i] )
				return false;
		}
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
//		vs[0] + vs[1] + ...vs[vs.size() - 2] が vs[vs.size() - 1] と等しいかチェック
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
//	
void printQuest(const string& txt, int len, bool nl = true)
{
	static cchar *digTxt[] = {"０", "１", "２", "３", "４", "５", "６", "７", "８", "９", };
	static cchar *alphabetTxt[] = {"Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "Ｇ", "Ｈ", "Ｉ", "Ｊ",
												"Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ", "Ｐ", "Ｑ", "Ｒ", "Ｓ", "Ｔ",
												"Ｕ", "Ｖ", "Ｗ", "Ｘ", "Ｙ", "Ｚ", };
	if( len > txt.size() )
		cout << string((len-txt.size())*2, ' ');
	for(auto ch: txt) {
		if( isdigit(ch) ) cout << digTxt[ch - '0'];
		else if( isupper(ch) ) cout << alphabetTxt[ch - 'A'];
		else cout << "□";
	}
	if( nl )
		cout << "\n";
}
//	加算問題ソルバー
//		row行、col文字目の次から順に数字を決めていく
//		すべての空欄を埋めたら、式が成立しているかチェック
//		解はひとつだけという前提なので、解をひとつ発見したら探索終了
bool solveAdd(vector<string> &vs, int row, int col)
{
	for (;;) {
		auto ch = vs[row][++col];		//	次の桁に移動し、その文字を ch に
		if( ch == '*' ) {	//	'*'（空欄）を見つけたら 0～9 を入れて探索、ただし最上位桁に 0 は入れない
			for (ch = !col ? '1' : '0'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				if( solveAdd(vs, row, col) )	//	自分自身を再帰コール
					return true;		//	解をひとつ発見したら終了
			}
			vs[row][col] = '*';		//	元に戻す
			return false;		//	解無し
		}
		if( ch >= '0' && ch <= '9' ) continue;	//	数字が入っている部分はスキップ
		if( ch == '\0' ) {		//	行末に達した場合
			if( ++row != vs.size() ) {		//	まだ最後に達していない場合
				col = -1;
				continue;
			}
			//	全てが確定した場合
			return checkAdd(vs /*, true*/);
		}
		//	上記以外の文字はスキップ
	}
}
//	加算問題ソルバー
bool solveAdd(std::vector<std::string>& vs)
{
	return solveAdd(vs, 0, -1);		//	最初の行・桁から探索開始
}
bool solveAddAlphabet(vector<string> &vs, int row, int col)
{
	for (;;) {
		auto ch = vs[row][++col];		//	次の桁に移動し、その文字を ch に
		if( ch == '*' ) {	//	'*'（空欄）を見つけたら 0～9 を入れて探索、ただし最上位桁に 0 は入れない
			for (char ch = !col ? '1' : '0'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				if( solveAddAlphabet(vs, row, col) )	//	自分自身を再帰コール
					return true;		//	解をひとつ発見したら終了
			}
			vs[row][col] = '*';		//	元に戻す
			return false;		//	解無し
		}
		if( ch >= 'A' && ch <= 'Z' ) {
			if( g_alphabet[ch - 'A'] >= 0 ) {
				vs[row][col] = g_alphabet[ch - 'A'];
				if( solveAddAlphabet(vs, row, col) )	//	自分自身を再帰コール
					return true;		//	解をひとつ発見したら終了
				vs[row][col] = ch;		//	元に戻す
				return false;		//	解無し
			}
			for (char c = !col ? '1' : '0'; c <= '9'; ++c) {
				if( !g_used[c - '0'] ) {
					g_alphabet[ch - 'A'] = c;
					g_used[c - '0'] = true;
					vs[row][col] = c;
					if( solveAddAlphabet(vs, row, col) )	//	自分自身を再帰コール
						return true;		//	解をひとつ発見したら終了
					g_used[c - '0'] = false;
				}
			}
			g_alphabet[ch - 'A'] = -1;
			vs[row][col] =ch;		//	元に戻す
			return false;		//	解無し
		}
		if( ch >= '0' && ch <= '9' ) continue;	//	数字が入っている部分はスキップ
		if( ch == '\0' ) {		//	行末に達した場合
			if( ++row != vs.size() ) {		//	まだ最後に達していない場合
				col = -1;
				continue;
			}
			//	全てが確定した場合
			return checkAdd(vs /*, true*/);
		}
		//	上記以外の文字はスキップ
	}
}
//	アルファベット対応加算問題ソルバー
bool solveAddAlphabet(std::vector<std::string>& vs)
{
	for(auto& x: g_alphabet) x = -1;
	for(auto& x: g_used) x = false;
	return solveAddAlphabet(vs, 0, -1);		//	最初の行・桁から探索開始
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
bool checkMul(vector<string> &vs , bool fill = false)	//	fill: 答えを埋めるかどうか
{
	int A = atoi(vs[0].c_str());		//	Aの値
	const auto &bstr = vs[1];		//	B文字列
	int ln = 2;
	for (int i = bstr.size(); --i >= 0; ++ln) {	//	Bの下の桁から順に掛け算を行う
		if( bstr[i] != '*' ) {
			int d = bstr[i] - '0';
			//auto s = to_string(d*A);
			if( !isMatchEx(vs[ln], d*A) )		//	vs[ln] は '*' を含んでいてもよい
				return false;
		}
	}
	int B = atoi(bstr.c_str());			//	Bの値
	if( !isMatchEx(vs[ln], B * A) )
		return false;
	if( fill ) {
		ln = 2;
		for (int i = bstr.size(); --i >= 0; ++ln) {
			int d = bstr[i] - '0';
			vs[ln] = to_string(d*A);
		}
		vs[ln] = to_string(B*A);
	}
	return true;
}
//		A*B
bool checkMulAlphabet(vector<string> &vs , bool fill = false)	//	fill: 答えを埋めるかどうか
{
	int A = atoi(vs[0].c_str());		//	Aの値
	const auto &bstr = vs[1];		//	B文字列
	int ln = 2;
	for (int i = bstr.size(); --i >= 0; ++ln) {	//	Bの下の桁から順に掛け算を行う
		if( bstr[i] != '*' ) {
			int d = bstr[i] - '0';
			//auto s = to_string(d*A);
			if( !d*A || !isMatchEx(vs[ln], d*A) )		//	先頭の数字は０以外、vs[ln] は '*', 'A'-'Z' を含んでいてもよい
				return false;
		}
	}
	int B = atoi(bstr.c_str());			//	Bの値
	if( !isMatchEx(vs[ln], B * A) )
		return false;
	if( fill ) {
		ln = 2;
		for (int i = bstr.size(); --i >= 0; ++ln) {
			int d = bstr[i] - '0';
			vs[ln] = to_string(d*A);
		}
		vs[ln] = to_string(B*A);
	}
	return true;
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
void test_checkMul()
{
	vector<string>vs1 = {"11", "11", "11", "11", "121"}; assert( checkMul(vs1) );
	vector<string>vs2 = {"55", "55", "275", "275", "3025"}; assert( checkMul(vs1) );
	vector<string>vs3 = {"120", "74", "*8*", "8**", "8*8*"};
	assert( checkMul(vs3) );
	vector<string>vs4 = {"140", "62", "*8*", "8**", "8*8*"};
	assert( checkMul(vs4) );
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
bool solveMulAlphabet(vector<string> &vs, int row, int col)		//	row行、col文字目の次から決めていく
{
	for (;;) {
		auto ch = vs[row][++col];
		if( ch == '*' ) {
			for (char ch = !col ? '1' : '0'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				if( solveMulAlphabet(vs, row, col) )
					return true;
			}
			vs[row][col] = '*';
			return false;		//	解無し
		}
		if( ch >= 'A' && ch <= 'Z' ) {
			if( g_alphabet[ch - 'A'] >= 0 ) {
				vs[row][col] = g_alphabet[ch - 'A'];
				if( solveMulAlphabet(vs, row, col) )	//	自分自身を再帰コール
					return true;		//	解をひとつ発見したら終了
				vs[row][col] = ch;		//	元に戻す
				return false;		//	解無し
			}
			for (char c = !col ? '1' : '0'; c <= '9'; ++c) {
				if( !g_used[c - '0'] ) {
					g_alphabet[ch - 'A'] = c;
					g_used[c - '0'] = true;
					vs[row][col] = c;
					if( solveMulAlphabet(vs, row, col) )	//	自分自身を再帰コール
						return true;		//	解をひとつ発見したら終了
					g_used[c - '0'] = false;
				}
			}
			g_alphabet[ch - 'A'] = -1;
			vs[row][col] =ch;		//	元に戻す
			return false;		//	解無し
		}
		if( ch >= '0' && ch <= '9' ) continue;
		if( ch == '\0' ) {
			if( ++row < 2 ) {
				col = -1;
				continue;
			}
			//	A * B が確定した場合
			char svAlphabet[26];
			bool svUsed[10];
			for(int i = 0; i != 26; ++i) svAlphabet[i] = g_alphabet[i];
			for(int i = 0; i != 10; ++i) svUsed[i] = g_used[i];
			auto rc = checkMulAlphabet(vs, true);
			for(int i = 0; i != 26; ++i) g_alphabet[i] = svAlphabet[i];
			for(int i = 0; i != 10; ++i) g_used[i] = svUsed[i];
			return rc;
		}
		//	上記以外の文字はスキップ
	}
}
bool solveMulAlphabet(std::vector<std::string>& vs)
{
	for(auto& x: g_alphabet) x = -1;
	for(auto& x: g_used) x = false;
	return solveMulAlphabet(vs, 0, -1);
}
/*
	　　A
	　－－－－－
	Ｂ）C
		.....
	　－－－－－
			R

	C/B = A…R		or		A*B + R = C

*/
void printDivQuest(const vector<string>& vs)
{
	int mxlen = vs[1].size() + 1 + vs[2].size();		//	1 for '）'
	printQuest(vs[0], mxlen);
	cout << string(vs[1].size()*2, ' ') << string(vs[2].size()*2 + 2, '-')<< "\n";
	printQuest(vs[1], 0, false);
	cout << "）";
	printQuest(vs[2], 0);
	for (int i = 3; i != vs.size() - 1; ++i) {
		printQuest(vs[i], mxlen - (vs.size()-i)/2 + 1);
		if( (i%2) == 1 )
			cout << string(vs[1].size()*2, ' ') << string(vs[2].size()*2 + 2, '-')<< "\n";
	}
	printQuest(vs.back(), mxlen);
	cout << "\n";
}
//	割り算として成立しているかどうかをチェック
//	A, B, R（商, 除数, 余り）は確定してるものとする
bool checkDiv(vector<string> &vs , bool fill = false)	//	fill: 答えを埋める
{
	//	C / B = A … R（被乗数 / 除数 = 商 … 余り）
	const auto &astr = vs[0];	//	商文字列
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
			row = vs.size() - 1;
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
bool isUniqAdd(vector<string> &vs, int row, int col)		//	row行、col文字目の次から決めていく
{
	for (;;) {
		auto ch = vs[row][++col];
		if( ch == '*' ) {
			for (ch = !col ? '1' : '0'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				isUniqAdd(vs, row, col);
				if( g_cnt > 1 )		//	ユニークで無い場合
					return false;
			}
			vs[row][col] = '*';
			return false;		//	解無し
		}
		if( ch >= '0' && ch <= '9' ) continue;
		if( ch == '\0' ) {
			if( ++row < vs.size() ) {
				col = -1;
				continue;
			}
			//	A + B + C が確定した場合
			if( !checkAdd(vs) )
				return false;
			if( ++g_cnt > 1 )	//	ユニークでない場合
				return false;
			else
				return true;
		}
		//	上記以外の文字はスキップ
	}
}
bool isUniqAdd(const vector<string> &vs0)
{
	g_cnt = 0;
	vector<string> vs(vs0);
	isUniqAdd(vs, 0, -1);
	return g_cnt == 1;
}
void removeAdd(vector<string> &vs)
{
	int mxlen = 0;	//	最大桁数
	for(auto& txt: vs)
		mxlen = max(mxlen, (int)txt.size());
	int r;
	for (int i = 0; i < mxlen; ++i) {	//	下から何桁目の数字を空欄にするか
		do {
			r = g_mt() % vs.size();		//	何番目の数字を空欄にするか
		} while ( i >= vs[r].size() );		//	i番目の桁が無い場合は、やり直し
		int ix = vs[r].size() - i - 1;		//	先頭から ix 番目を空欄にする
		vs[r][ix] = '*';
	}
#if	0
	for (int i = 0; i != vs[0].size(); ++i) {
		int r = g_mt() % vs.size();
		int ix = vs[r].size() - 1 - i;
		vs[r][ix] = '*';
	}
	if( vs.back().size() > vs[1].size() )
		vs.back()[0] = '*';
#endif
#if	0
	//	最初に * に出来る位置の一覧を取得し、そこから * にしていく版
	vector<string> vs2;
	if( isUniqAdd(vs) )
		vs2 = vs;
	for (;;) {
		vector<Pos>	vpos;	//	虫食いに出来る位置リスト
		int row = 0;
		int col = 0;
		for (;;) {
			if( vs[row][col] == '\0' ) {
				if( ++row == vs.size() ) break;
				col = 0;
				continue;
			}
			auto ch = vs[row][col];
			if( ch >= '0' && ch <= '9' ) {
				vs[row][col] = '*';
				if( isUniqAdd(vs) )
					vpos.emplace_back(row, col);
				vs[row][col] = ch;
			}
			++col;
		}
		if( vpos.empty() )
			break;
		int r = g_mt() % vpos.size();
		row = vpos[r].m_row;
		col = vpos[r].m_col;
		vs[row][col] = '*';
		vs2 = vs;
	}
#endif
}
void genAdd(std::vector<std::string>& vs0, std::vector<std::string>& vs, int A, int B, int C)
{
	vs.clear();
	vs.push_back(to_string(A));
	vs.push_back(to_string(B));
	vs.push_back(to_string(C));
	vs.push_back(to_string(A+B+C));
	vs0 = vs;
	removeAdd(vs);
}
//	row行、col文字目の次から決めていく
//	'*' を見つけた場合は '0' ～ '9' を順に入れて、再帰コール。ただし先頭桁に '0' は入らない
//	'*' 以外はスキップ、A*B が確定した時点で解チェック
void isUniqMul(vector<string> &vs, int row, int col)
{
	for (;;) {
		auto ch = vs[row][++col];
		if( ch == '*' ) {
			for (ch = !col ? '1' : '0'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				isUniqMul(vs, row, col);
				if( g_cnt > 1 )		//	ユニークで無い場合
					return;
			}
			vs[row][col] = '*';
			return;		//	解無し
		}
		if( ch >= '0' && ch <= '9' ) continue;
		if( ch == '\0' ) {
			if( ++row < 2 ) {
				col = -1;
				continue;
			}
			//	A * B が確定した場合
			//if( vs[0] == "120" && vs[1] == "74" || vs[0] == "140" && vs[1] == "62" )
			//	cout << vs[0] << " * " << vs[1] << "\n";
			if( checkMul(vs /*, false*/) ) {		//	解として成立している場合
				++g_cnt;
			}
			return;
		}
	}
}
bool isUniqMul(const vector<string> &vs0)
{
	g_cnt = 0;
	vector<string> vs(vs0);
	isUniqMul(vs, 0, -1);
	return g_cnt == 1;
}
//	貪欲法により可能な数字をできるだけ消す
void removeGreedyMul(vector<string> &vs,
									double p = 0.0)		//	残す数字割合、p が大きいほど数字が残る
{
	//	最初に * に出来る位置の一覧を取得し、そこから * にしていく版
	//vector<string> vs2;
	//if( isUniqMul(vs) )
	//	vs2 = vs;
	int cnt = 0;
	for(const auto &s : vs)
		cnt += s.size();	//	全文字数計算
	int limit = round(cnt * p);
	cnt -= limit;		//	cnt: 消すべき文字数
	for (;;) {
		vector<Pos>	vpos;	//	虫食いに出来る位置リスト
		int row = 0;
		int col = 0;
		for (;;) {
			if( vs[row][col] == '\0' ) {
				if( ++row == vs.size() )	//	スキャン終わり
					break;	
				col = 0;
				continue;
			}
			auto ch = vs[row][col];
			if( ch >= '0' && ch <= '9' ) {
				vs[row][col] = '*';		//	row, col の文字を空欄にし、
				if( isUniqMul(vs) )		//	それがユニーク解を持つかチェック
					vpos.emplace_back(row, col);
				vs[row][col] = ch;
			}
			++col;
		}
		//vpos.pop_back();		//	最後の余りは消さない
		if( vpos.empty() )		//	消せる数字が無くなった場合
			break;
		int r = g_mt() % vpos.size();		//	消す位置をランダムに決める
		//row = vpos[r].m_row;
		//col = vpos[r].m_col;
		vs[vpos[r].m_row][vpos[r].m_col] = '*';
		//vs2 = vs;
		if( --cnt <= 0 ) break;
	}
}
//	A*B を与えられ、貪欲法により問題を生成
void genMul(std::vector<std::string>& vs0, std::vector<std::string>& vs, int A, int B,
			double p = 0)		//	空欄割合
{
	for (;;) {
		vs.clear();
		vs.push_back(to_string(A));		//	被乗数
		vs.push_back(to_string(B));		//	乗数
		int b2 = B;
		while( b2 ) {
			int t = b2 % 10;
			vs.push_back(to_string(A*t));		//	計算途中結果
			b2 /= 10;
		}
		vs.push_back(to_string(A*B));			//	乗算結果
		vs0 = vs;
		removeGreedyMul(vs, p);					//	貪欲法により可能な数字をできるだけ消す
		int cnt = count(vs[0].begin(), vs[0].end(), '*') +
					count(vs[1].begin(), vs[1].end(), '*');
		if( cnt >= 2 ) return;						//	空欄がひとつだけの場合はやり直し
	}
}
bool genMulOnly1(	std::vector<std::string>& vs0,		//	解答
							std::vector<std::string>& vs,		//	問題
							int A, int B)
{
	vs.clear();
	vs.push_back(to_string(A));
	vs.push_back(to_string(B));
	int b2 = B;
	while( b2 ) {
		int t = b2 % 10;
		vs.push_back(to_string(A*t));
		b2 /= 10;
	}
	vs.push_back(to_string(A*B));
	vs0 = vs;
	for (char d = '1'; d <= '9'; ++d) {
		vs = vs0;
		for(auto& str: vs) {
			for(auto& ch: str) {
				if( ch != d ) ch = '*';
			}
		}
		if( isUniqMul(vs) ) {
			removeGreedyMul(vs);
			int cnt = count(vs[0].begin(), vs[0].end(), '*') +
						count(vs[1].begin(), vs[1].end(), '*');
			if( cnt != 0 )		//	A*B に '*' が含まれる場合
				return true;
		}
	}
	return false;
}
void isUniqDiv(vector<string> &vs, int row, int col)		//	row行、col文字目の次から決めていく
{
	for (;;) {
		auto ch = vs[row][++col];
		if( ch == '*' ) {
			for (ch = !col ? '1' : '0'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				isUniqDiv(vs, row, col);
				if( g_cnt > 1 )		//	ユニークで無い場合
					return;
			}
			vs[row][col] = '*';
			return;		//	解無し
		}
		if( ch >= '0' && ch <= '9' ) continue;
		if( ch == '\0' ) {
			if( ++row < 2 ) {
				col = -1;
				continue;
			}
			//	A * B が確定した場合
			if( vs.back() != "*" ) {
				if( checkDiv(vs /*, false*/) )
					++g_cnt;
			} else {
				for(char d = '0'; d <= '9'; ++d) {
					vs.back() = d;
					if( checkDiv(vs) ) {
						if (++g_cnt > 1) break;
					}
				}
				vs.back() = '*';
			}
			return;
		}
		//	上記以外の文字はスキップ
	}
}
bool isUniqDiv(const vector<string> &vs0)
{
	g_cnt = 0;
	vector<string> vs(vs0);
	isUniqDiv(vs, 0, -1);
	return g_cnt == 1;
}
void removeGreedyDiv(vector<string> &vs, double p = 0)
{
	//	最初に * に出来る位置の一覧を取得し、そこから * にしていく版
	int cnt = 0;	//	全文字数
	for(const auto &s : vs)
		cnt += s.size();
	int limit = (int)(cnt * p + 0.5);
	cnt -= limit;
	vector<string> vs2;
	if( isUniqDiv(vs) )
		vs2 = vs;
	for (;;) {
		vector<Pos>	vpos;	//	虫食いに出来る位置リスト
		int row = 0;
		int col = 0;
		for (;;) {
			if( vs[row][col] == '\0' ) {
				if( ++row == vs.size() - 1 ) break;
				col = 0;
				continue;
			}
			auto ch = vs[row][col];
			if( ch >= '0' && ch <= '9' ) {
				vs[row][col] = '*';
				if( isUniqDiv(vs) )
					vpos.emplace_back(row, col);
				vs[row][col] = ch;
			}
			++col;
		}
		if( vpos.empty() )
			break;
		int r = g_mt() % vpos.size();
		row = vpos[r].m_row;
		col = vpos[r].m_col;
		vs[row][col] = '*';
		vs2 = vs;
		if( --cnt <= 0 ) break;
	}
}
void genDiv(std::vector<std::string>& vs0, std::vector<std::string>& vs, int A, int B, int R, double p)
{
	if( !B ) return;
	R %= B;	//	R >= B の場合対応
	const auto astr = to_string(A);
	for(;;) {
		vs.clear();
		vs.push_back(astr);
		vs.push_back(to_string(B));
		int C = A * B + R;
		vs.push_back(to_string(C));
		int sc = 1;
		for (int i = 1; i != astr.size(); ++i) {
			sc *= 10;
		}
		for (int i = 0; i != astr.size(); ++i) {
			int t = astr[i] - '0';
			vs.push_back(to_string(B*t));
			C -= B*t*sc;
			if( (sc /= 10) == 0 ) sc = 1;
			vs.push_back(to_string(C/sc));
		}
		vs0 = vs;
		removeGreedyDiv(vs, p);
		int cnt = count(vs[0].begin(), vs[0].end(), '*') +
					count(vs[1].begin(), vs[1].end(), '*');
		if( cnt >= 2 ) return;
	}
}
bool genDivOnly1(std::vector<std::string>& vs0, std::vector<std::string>& vs, int A, int B, int R = 0)
{
	if (!B) return false;
	R %= B;	//	R >= B の場合対応
	const auto astr = to_string(A);
	vs.clear();
	vs.push_back(astr);
	vs.push_back(to_string(B));
	int C = A * B + R;
	vs.push_back(to_string(C));
	int sc = 1;
	for (int i = 1; i != astr.size(); ++i) {
		sc *= 10;
	}
	for (int i = 0; i != astr.size(); ++i) {
		int t = astr[i] - '0';
		vs.push_back(to_string(B*t));
		C -= B*t*sc;
		if( (sc /= 10) == 0 ) sc = 1;
		vs.push_back(to_string(C/sc));
	}
	vs0 = vs;
	for (char d = '1'; d <= '9'; ++d) {
		vs = vs0;
		for(auto& str: vs) {
			for(auto& ch: str) {
				if( ch != d ) ch = '*';
			}
		}
		if( isUniqDiv(vs) ) {
			removeGreedyDiv(vs);
			int cnt = count(vs[0].begin(), vs[0].end(), '*') +
						count(vs[1].begin(), vs[1].end(), '*');
			if( cnt != 0 )		//	A*B に '*' が含まれる場合
				return true;
		}
	}
	return false;
}
void test_uniqMul()
{
	vector<string>vs2 = {"*4", "30", "0", "282", "2820"};
	assert( isUniqMul(vs2) );
	vector<string>vs1 = {"***", "**", "*8*", "8**", "8*8*"};
	assert( !isUniqMul(vs1) );
}
void test_uniqDiv()
{
	vector<string>vs1 = {"*", "**", "7*7", "***", "*"};
	assert( !isUniqDiv(vs1) );
	vector<string>vs2 = {"*", "5", "*", "*", "*"};
	assert( !isUniqDiv(vs2) );
}
int main()
{
	test_isMatch();
	test_checkAdd();
	test_checkMul();
	test_uniqMul();
	test_uniqDiv();
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
#if	0
	vector<string> qm3 = {"**", "**", "**5", "25*", "2**5"};
	printMulQuest(qm3);
	if( solveMul(qm3) ) printMulQuest(qm3);
	else cout << "can't solve\n\n";
	//
	vector<string> qm4 = {"***", "***", "11**", "****", "*11", "**1**"};
	printMulQuest(qm4);
	if( solveMul(qm4) ) printMulQuest(qm4);
	else cout << "can't solve\n\n";
	//
	vector<string> qm5 = {"***", "****", "***", "****", "****", "2012", "**24***"};
	printMulQuest(qm5);
	if( solveMul(qm5) ) printMulQuest(qm5);
	else cout << "can't solve\n\n";
#endif
	//
#if	0
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
	if( true ) {
		std::vector<std::string> va0, va;
		genAdd(va0, va, 123, 456, 789);
		printAddQuest(va0);
		printAddQuest(va);
	}
	//
	if( true ) {
		std::vector<std::string> va0, va;
		genMul(va0, va, 123, 456, 0.3);
		printMulQuest(va0);
		printMulQuest(va);
	}
	//
	if( true ) {
		std::vector<std::string> va0, va;
		genDiv(va0, va, 123, 45, 9, 0.3);
		printDivQuest(va0);
		printDivQuest(va);
	}
#endif
#if	0
	if( true ) {
		std::vector<std::string> va0, va;
		genAdd(va0, va, 123, 6, 9);
		//genAdd(va0, va, g_mt() % 1000, g_mt() % 1000, g_mt() % 1000);
		printAddQuest(va0);
		printAddQuest(va);
	}
#endif
	//
#if	1
	if( true ) {
		std::vector<std::string> va0, va;
		genMul(va0, va, g_mt() % 100, g_mt() % 100, 0.3);
		printMulQuest(va0);
		printMulQuest(va);
	}
#endif
	//
#if	0
	if( true ) {
		std::vector<std::string> va0, va;
		genDiv(va0, va, g_mt() % 100, g_mt() % 100, g_mt() % 10, 0.3);
		printDivQuest(va0);
		printDivQuest(va);
	}
#endif
#if	0
	if( true ) {
		std::vector<std::string> va0, va;
		if( genMulOnly1(va0, va, 98, 89) ) {
			printMulQuest(va0);
			printMulQuest(va);
		} else {
			printMulQuest(va0);
			cout << "failed to gen quest.\n";
		}
	}
	if( true ) {
		std::vector<std::string> va0, va;
		if( genMulOnly1(va0, va, 898, 989) ) {
			printMulQuest(va0);
			printMulQuest(va);
		} else {
			printMulQuest(va0);
			cout << "failed to gen quest.\n";
		}
	}
#endif
#if	0
	if( true ) {
		std::vector<std::string> va0, va;
		for(;;) {
			if( genMulOnly1(va0, va, g_mt() % 1000, g_mt() % 1000) )
				break;
		}
		printMulQuest(va0);
		printMulQuest(va);
	}
#endif
#if	0
	if( true ) {
		std::vector<std::string> va0, va;
		genMulOnly1(va0, va, 11, 11);
		printMulQuest(va0);
		printMulQuest(va);
	}
	if( true ) {
		std::vector<std::string> va0, va;
		genMulOnly1(va0, va, 123, 456);
		printMulQuest(va0);
		printMulQuest(va);
	}
#endif
#if	0
	if( true ) {
		std::vector<std::string> va0, va;
		if( genDivOnly1(va0, va, 121, 212, 2) ) {
			printDivQuest(va0);
			printDivQuest(va);
		} else {
			printDivQuest(va0);
			cout << "failed to gen quest.\n";
		}
	}
#endif
#if	0
	if( true ) {
		std::vector<std::string> va0, va;
		for(;;) {
			if( genDivOnly1(va0, va, g_mt() % 1000, g_mt() % 100, g_mt() % 10) )
				break;
		}
		printDivQuest(va0);
		printDivQuest(va);
	}
#endif
#if	0
	vector<string>vs1 = {"SEND", "MORE", "MONEY"};
	printAddQuest(vs1);
	if( solveAddAlphabet(vs1) )
		printAddQuest(vs1);
	else
		cout << "can't solve\n";
	//
#endif
#if	0
	//vector<string>vs1 = {"ABA", "ABA", "CAC", "ABA", "CAC", "CCDCC"};
	vector<string>vs1 = {"ABCD", "ABCD", "****D", "****C", "****B", "****A", "*******D"};
	printMulQuest(vs1);
	if( solveMulAlphabet(vs1) )
		printMulQuest(vs1);
	else
		cout << "can't solve\n";
#endif
	//
    std::cout << "OK\n";
}
