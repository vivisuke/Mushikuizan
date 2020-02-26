#pragma once

#include <vector>
#include <string>

struct Pos {
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

bool solveAdd(std::vector<std::string>&);
bool solveMul(std::vector<std::string>&);
bool solveDiv(std::vector<std::string>&);
void genAdd(std::vector<std::string>&, std::vector<std::string>&, int, int, int);
void genMul(std::vector<std::string>&, std::vector<std::string>&, int, int, double=0.0);
void genDiv(std::vector<std::string>&, std::vector<std::string>&, int, int, int=0, double=0.0);
