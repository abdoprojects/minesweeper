// rec03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
// ConsoleApplication2.cpp : Defines the entry point for the console application.
//


#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <ctime>
using namespace std;

struct Cell {
	bool bomb = 0;
	bool visible = 0;
	int badNeighbors = 0;
};

class Minesweeper {
private:
	vector<vector<Cell>> mineField;
public:
	Minesweeper() {
		vector<Cell> buffer;
		for (int i = 0; i < 12; ++i) {
			Cell bufferCell;
			bufferCell.bomb = 0;
			bufferCell.visible = 1;
			bufferCell.badNeighbors = 9;
			buffer.push_back(bufferCell);
		}
		Cell bufferCell;
		bufferCell.bomb = 0;
		bufferCell.visible = 1;
		bufferCell.badNeighbors = 9;

		mineField.push_back(buffer);
		srand(time(NULL));
		for (int row = 0; row < 10; ++row) {
			vector<Cell> cellRow;
			cellRow.push_back(bufferCell);
			for (int ind = 0; ind < 10; ++ind) {
				if (rand() % 100 < 10) {
					Cell cell;
					cell.bomb = 1;
					cell.visible = 0;
					cell.badNeighbors = 0;
					cellRow.push_back(cell);
				}
				else {
					Cell cell;
					cell.bomb = 0;
					cell.visible = 0;
					cell.badNeighbors = 0;
					cellRow.push_back(cell);
				}
			}
			cellRow.push_back(bufferCell);
			mineField.push_back(cellRow);
		}
		mineField.push_back(buffer);

		for (size_t row = 0; row < mineField.size(); ++row) { //iterates over each cell and counts neighbors
			for (size_t ind = 0; ind < mineField[row].size(); ++ind) {
				//cout << "here" << row << endl;
				if (mineField[row][ind].bomb) {
					continue;
				}
				if (mineField[row][ind].badNeighbors == 9) {
					continue;
				}
				if (mineField[row][ind + 1].bomb) {
					mineField[row][ind].badNeighbors += 1;
				}
				if (mineField[row + 1][ind + 1].bomb) {
					mineField[row][ind].badNeighbors += 1;
				}
				if (mineField[row - 1][ind + 1].bomb) {
					mineField[row][ind].badNeighbors += 1;
				}
				if (mineField[row + 1][ind].bomb) {
					mineField[row][ind].badNeighbors += 1;
				}
				if (mineField[row - 1][ind].bomb) {
					mineField[row][ind].badNeighbors += 1;
				}
				if (mineField[row + 1][ind - 1].bomb) {
					mineField[row][ind].badNeighbors += 1;
				}
				if (mineField[row][ind - 1].bomb) {
					mineField[row][ind].badNeighbors += 1;
				}
				if (mineField[row - 1][ind - 1].bomb) {
					mineField[row][ind].badNeighbors += 1;
				}
			}
		}
	}

	void display(bool transparent) const {
		for (vector<Cell> row : mineField) {
			for (Cell cell : row) {
				if (transparent) { //game is over
					if (cell.badNeighbors == 9) {
						cout << ' ';
					}
					else if (cell.bomb) {
						cout << '*';
					}
					else { //a transparent square
						cout << cell.badNeighbors;
					}
				}
				else { //if the game is not over
					if (cell.badNeighbors == 9) {
						cout << ' ';
					}
					else if (!cell.visible) {
						cout << '-';
					}
					else if (cell.visible && cell.badNeighbors == 0) {
						cout << '0';
					}
					else if (cell.visible && cell.badNeighbors > 0) {
						cout << cell.badNeighbors;
					}
				}
			}
			cout << endl;
		}
	}

	bool done() {
		for (vector<Cell> row : mineField) {
			for (Cell cell : row) {
				if (!cell.visible && !cell.bomb) {
					return 0;
				}
			}
		}
		return 1;
	}
	bool validRow(int rownum) {
		if (rownum > 10) {
			return 0;
		}
		else {
			return 1;
		}
	}
	bool validCol(int colnum) {
		if (colnum > 10) {
			return 0;
		}
		else {
			return 1;
		}
	}
	bool isVisible(int rownum, int colnum) {
		return (mineField[rownum][colnum].visible && validCol(colnum) && validRow(rownum));
	}
	bool play(int rownum, int colnum) {
		if (mineField[rownum][colnum].bomb) {
			return 0;
		}
		if (mineField[rownum][colnum].visible) {
			return 1;
		}
		if (mineField[rownum][colnum].badNeighbors != 0) {
			mineField[rownum][colnum].visible = 1;
			return 1;
		}
		else if (mineField[rownum][colnum].badNeighbors == 0 && mineField[rownum][colnum].badNeighbors != 9) {
			mineField[rownum][colnum].visible = 1;
			play(rownum, colnum + 1);
			play(rownum + 1, colnum + 1);
			play(rownum - 1, colnum + 1);
			play(rownum + 1, colnum);
			play(rownum - 1, colnum);
			play(rownum + 1, colnum - 1);
			play(rownum, colnum - 1);
			play(rownum - 1, colnum - 1);
		}

	}
};

/*
int main() {
Minesweeper sweeper;
sweeper.play(2, 3);
sweeper.display(0);
sweeper.isVisible(1, 1);

}
*/

int main() {
	Minesweeper sweeper;
	// Continue until the only invisible cells are bombs
	while (!sweeper.done()) {
		sweeper.display(false); // display the board without bombs

		int row_sel = -1, col_sel = -1;
		while (row_sel == -1) {
			// Get a valid move
			int r, c;
			cout << "row? ";
			cin >> r;
			if (!sweeper.validRow(r)) {
				cout << "Row out of bounds\n";
				continue;
			}
			cout << "col? ";
			cin >> c;
			if (!sweeper.validCol(c)) {
				cout << "Column out of bounds\n";
				continue;
			}
			if (sweeper.isVisible(r, c)) {
				cout << "Square already visible\n";
				continue;
			}
			row_sel = r;
			col_sel = c;
		}
		// Set selected square to be visible. May effect other cells.
		if (!sweeper.play(row_sel, col_sel)) {
			cout << "BOOM!!!\n";
			sweeper.display(true);
			// We're done! Should consider ending more "cleanly",
			// eg. Ask user to play another game.
			exit(0);
		}
	}
	// Ah! All invisible cells are bombs, so you won!
	cout << "You won!!!!\n";
	sweeper.display(true); // Final board with bombs shown
}
