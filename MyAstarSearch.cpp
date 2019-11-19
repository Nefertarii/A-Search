#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using std::sort;
using std::cout;
using std::vector;
using std::fstream;
using std::string;
using std::istringstream;
const int delta[4][2]{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
static int Car = 0;
enum State {
	kEmpty, kObstacle, kClosed, kPath, kStart, kFinish
};
void Pause() {
	int a;
	std::cin >> a;
}
void PrintBoard(vector<vector<int>> board)
{
	for (int row = 0; row != board.size(); row++)
	{
		for (int col = 0; col != board[row].size(); col++)
		{
			cout << board[row][col] << "";
		}
		cout << "\n";
	}
}
/*string CellString(State cell) {
	switch (cell) {
	case State::kObstacle: return "M    ";
	case State::kPath: return     "C    ";
	case State::kEmpty: return    "E    ";
	case State::kClosed: return   "X    ";
	case State::kStart: return    "B    ";
	case State::kFinish: return   "G    ";
	default: return "?   ";
	}
}*/
string CellString(State cell) {
	switch (cell) {
	case State::kObstacle: return "⛰️   ";
	case State::kPath: return "🚗   ";
	case State::kEmpty: return "E   ";
	case State::kClosed: return "C   ";
	case State::kStart: return "🚦   ";
	case State::kFinish: return "🏁   ";
	default: return "?   ";
	}
}
void PrintBoard(vector<vector<State>> board)
{
	for (int row = 0; row != board.size(); row++)
	{
		for (int col = 0; col != board[row].size(); col++)
		{
			cout << CellString(board[row][col]);
		}
		cout << "\n";
	}
}
State CellString(int i)
{
		switch (i)
		{
		case kEmpty:return kEmpty;
		case kObstacle:return kObstacle;
		case kStart:return kStart;
		case kFinish:return kFinish;
		case kClosed:return kClosed;
		case kPath:return kPath;
		}
}
vector<vector<State>> ReadFile(string filename)
{
	fstream fout;
	fout.open(filename);
	vector<vector<State>> board;
	if (fout)
	{
		string Tmpstr;
		char Tmpc;
		int Tmpi;
		vector<State> ivec = {};
		while (getline(fout, Tmpstr))
		{
			istringstream Strm(Tmpstr);
			ivec.clear();
			while (Strm >> Tmpi >> Tmpc)
			{
				ivec.push_back(CellString(Tmpi));
			}
			board.push_back(ivec);
		}
	}
	else
	{
		cout << "File not open.\n";
	}
	return board;
}
void PrintBoard(vector<int> board)
{
	for (auto i : board)
		cout << i << " ";
	cout << "\n";
}
int Heuristic(int Begin[], int Goal[])
{
	int X1 = Begin[0], X2 = Goal[0];
	int Y1 = Begin[1], Y2 = Goal[1];
	int Manhattan = abs(X2 - X1) + abs(Y2 - Y1);
	return Manhattan;
}
int Heuristic(int X1,int Y1,int X2,int Y2)
{
	int Manhattan = abs(X2 - X1) + abs(Y2 - Y1);
	return Manhattan;
}
void AddtoOpen(int X, int Y, int G, int H, vector<vector<int>> &Openlist, vector<vector<State>> &board)
{
	Openlist.push_back(vector<int>{X, Y, G, H});
	board[X][Y] = kClosed;
}
bool Compare(vector<int> List1, vector<int> List2)
{
	auto list1 = List1[2] + List1[3];
	auto list2 = List2[2] + List2[3];
	return list1 > list2;
}

void CellSort(vector<vector<int>> *v) {
	sort(v->begin(), v->end(), Compare);
}
bool CheckValidCell(int x, int y, vector<vector<State>> &board)
{
	/*if (x >= 0 && x < board.size())
		return false;
	else if (y >= 0 && y < board[0].size())
		return false;
	else if (board[x][y] != kEmpty)
		return false;
	else return true;*/
	bool on_grid_x = (x >= 0 && x < board.size());
	bool on_grid_y = (y >= 0 && y < board[0].size());
	if (on_grid_x && on_grid_y)
		return board[x][y] == State::kEmpty;
	return false;
}
void ExpandNeighbors(vector<int> &node, int *goal, vector<vector<int>> &openlist, vector<vector<State>> &board)
{
	int x = node[0];
	int y = node[1];
	int g = node[2];
	int h = node[3];
	int newg = g + 1;
	for (int i = 0; i < 4; i++)
	{
		int _x = x + delta[i][0];
		int _y = y + delta[i][1];
		if (CheckValidCell(_x, _y, board))
		{
			int newh = Heuristic(_x, _y, goal[0], goal[1]);
			AddtoOpen(_x, _y, newg, newh, openlist, board);
		}
	}

}
vector<vector<State>> Search(vector<vector<State>> &board, int Begin[], int Goal[])
{
	int begin = board[Begin[0]][Begin[1]];
	int goal = board[Goal[0]][Goal[1]];
	int heuristic = Heuristic(Begin, Goal);
	int g = 0;
	vector<vector<int>> Openlist;
	AddtoOpen(Begin[0], Begin[1], g, Heuristic(Begin, Goal), Openlist, board);
	while (Openlist.size() > 0)
	{
		CellSort(&Openlist);
		auto Move = Openlist.back();
		Openlist.pop_back();
		int x = Move[0];
		int y = Move[1];
		board[x][y] = kPath;
		if (x == Goal[0] && y == Goal[1])
		{
			board[Begin[0]][Begin[1]] = State::kStart;
			board[Goal[0]][Goal[1]] = State::kFinish;
			return board;
		}
		ExpandNeighbors(Move, Goal, Openlist, board);
	}
	cout << "No path found!" << "\n";
	return vector<vector<State>>{};
}

int main()
{
	/*
	vector<vector<int>> board = {{0,1,0,0,0,0},
								 {0,1,0,0,0,0},
								 {0,1,0,0,0,0},
								 {0,1,0,0,0,0},
								 {0,0,0,0,0,0}};
								 */
	//PrintBoard(board);
	auto board = ReadFile("board");
	PrintBoard(board);
	int Begin[] = { 0,0 };
	int Goal[] = { 4,7 };
	board = Search(board, Begin, Goal);
	cout << "\n";
	PrintBoard(board);
	Pause();
}