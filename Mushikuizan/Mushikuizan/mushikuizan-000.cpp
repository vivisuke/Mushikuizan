#include <random>
#include <algorithm>    // std::count
#include <sstream>      // std::ostringstream
#include "mushikuizan.h"

using namespace std;

random_device g_rd2;
mt19937 g_mt2(g_rd2());

template <typename T> std::string to_string(const T& t) { std::ostringstream os; os<<t; return os.str(); }

bool isMatch(const string &t, int v)		//	計算結果(t) と v がマッチしているか？
{
	for (int i = t.size(); --i >= 0; ) {
		int d = v % 10;
		if( t[i] >= '0' && t[i] <= '9' && t[i] != d + '0' )
			return false;
		v /= 10;
		if( v == 0 )
			return i == 0;
	}
	return v == 0;
}
bool checkMul(vector<string> &vs , bool fill = false)	//	fill: 答えを埋める
{
	int A = atoi(vs[0].c_str());
	const auto &bstr = vs[1];
	int ln = 2;
	for (int i = bstr.size(); --i >= 0; ++ln) {
		int d = bstr[i] - '0';
		//auto s = to_string(d*A);
		if( !d*A || !isMatch(vs[ln], d*A) )		//	先頭の数字は０以外
			return false;
	}
	int B = atoi(vs[1].c_str());
	if( !isMatch(vs[ln], B * A) )
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
bool checkAdd(vector<string> &vs /*, bool fill*/)	//	fill: 答えを埋める → 常に埋まっている？
{
	int sum = 0;
	for (int i = 0; i != vs.size() - 1; ++i) {
		sum += atoi(vs[i].c_str());
	}
	return isMatch(vs.back(), sum);
	//return atoi(vs.back().c_str()) == sum;
}
int g_cnt = 0;
bool isUniqMul(vector<string> &vs, int row, int col)		//	row行、col文字目の次から決めていく
{
	for (;;) {
		auto ch = vs[row][++col];
		if( ch == '*' ) {
			for (ch = '1'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				isUniqMul(vs, row, col);
				if( g_cnt > 1 )		//	ユニークで無い場合
					return false;
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
			if( !checkMul(vs /*, false*/) )
				return false;
			if( ++g_cnt > 1 )	//	ユニークでない場合
				return false;
			else
				return true;
		}
		//	上記以外の文字はスキップ
	}
}
bool isUniqMul(const vector<string> &vs0)
{
	g_cnt = 0;
	vector<string> vs(vs0);
	isUniqMul(vs, 0, -1);
	return g_cnt == 1;
}
void removeGreedyMul(vector<string> &vs, double p = 0.0)
{
	//	最初に * に出来る位置の一覧を取得し、そこから * にしていく版
	vector<string> vs2;
	if( isUniqMul(vs) )
		vs2 = vs;
	int cnt = 0;	//	全文字数
	for(const auto &s : vs)
		cnt += s.size();
	int limit = (int)(cnt * p + 0.5);
	cnt -= limit;
	for (;;) {
		vector<Pos>	vpos;	//	虫食いに出来る位置リスト
		int row = 0;
		int col = 0;
		for (;;) {
			if( vs[row][col] == '\0' ) {
				if( ++row == vs.size() )		//	-1 for 最後の余りは消さない
					break;	
				col = 0;
				continue;
			}
			auto ch = vs[row][col];
			if( ch >= '0' && ch <= '9' ) {
				vs[row][col] = '*';
				if( isUniqMul(vs) )
					vpos.emplace_back(row, col);
				vs[row][col] = ch;
			}
			++col;
		}
		//vpos.pop_back();		//	最後の余りは消さない
		if( vpos.empty() )
			break;
		int r = g_mt2() % vpos.size();
		row = vpos[r].m_row;
		col = vpos[r].m_col;
		vs[row][col] = '*';
		//printMul(vs);
		vs2 = vs;
		if( --cnt <= 0 ) break;
	}
	//if( !vs2.empty() )
	//	printMul(vs2);
}
bool checkDiv(vector<string> &vs , bool fill = false)	//	fill: 答えを埋める
{
	const auto &astr = vs[0];
	int A = atoi(astr.c_str());		//	商
	int B = atoi(vs[1].c_str());	//	除数
	int R = atoi(vs.back().c_str());		//	余り
	int C = A * B + R;					//	被除数
	if( !isMatch(vs[2], C) )		//	答えが合っているか？
		return false;
	int sc = 1;
	for (int i = 1; i != astr.size(); ++i) {
		sc *= 10;
	}
	int sc0 = sc;
	int ln = 3;
	for (int i = 0; i != astr.size(); ++i, ln+=2) {
		int d = astr[i] - '0';
		if( !isMatch(vs[ln], d*B) )
			return false;
		C -= d*B*sc;
		if( (sc /= 10) == 0 ) sc = 1;
		if( !isMatch(vs[ln+1], C/sc) )
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
bool isUniqDiv(vector<string> &vs, int row, int col)		//	row行、col文字目の次から決めていく
{
	for (;;) {
		auto ch = vs[row][++col];
		if( ch == '*' ) {
			for (ch = !col ? '1' : '0'; ch <= '9'; ++ch) {
				vs[row][col] = ch;
				isUniqDiv(vs, row, col);
				if( g_cnt > 1 )		//	ユニークで無い場合
					return false;
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
			if( !checkDiv(vs /*, false*/) )
				return false;
			if( ++g_cnt > 1 )	//	ユニークでない場合
				return false;
			else
				return true;
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
void removeGreedyDiv(vector<string> &vs, double p)
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
		int r = g_mt2() % vpos.size();
		row = vpos[r].m_row;
		col = vpos[r].m_col;
		vs[row][col] = '*';
		vs2 = vs;
		if( --cnt <= 0 ) break;
	}
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
void removeGreedyAdd(vector<string> &vs)
{
	for (int i = 0; i != vs[0].size(); ++i) {
		int r = g_mt2() % vs.size();
		int ix = vs[r].size() - 1 - i;
		vs[r][ix] = '*';
	}
	if( vs.back().size() > vs[1].size() )
		vs.back()[0] = '*';
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
		int r = g_mt2() % vpos.size();
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
	removeGreedyAdd(vs);
}
void genMul(std::vector<std::string>& vs0, std::vector<std::string>& vs, int A, int B,
			double p)		//	空欄割合
{
	for (;;) {
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
		removeGreedyMul(vs, p);
		int cnt = count(vs[0].begin(), vs[0].end(), '*') +
					count(vs[1].begin(), vs[1].end(), '*');
		if( cnt >= 2 ) return;
	}
}
void genDiv(std::vector<std::string>& vs0, std::vector<std::string>& vs, int A, int B, int R, double p)
{
	R %= B;
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
bool solveDiv(vector<string> &vs, int row, int col, bool fill = true)		//	row行、col文字目の次から決めていく
{
	for (;;) {
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
		if( ch >= '0' && ch <= '9' ) continue;
		if( ch == '\0' ) {
			if( ++row < 2 ) {
				col = -1;
				continue;
			}
			//	A * B が確定した場合
			return checkDiv(vs, fill);
		}
		//	上記以外の文字はスキップ
	}
}
bool solveDiv(std::vector<std::string>& vs)
{
	return solveDiv(vs, 0, -1);
}
