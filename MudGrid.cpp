
#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <set> 
#include<istream>
#include <limits.h> 

class Matrix
{
public:

	int ROW, COL;
	int size;
	int c_width;
	int* grid;

	Matrix(int r, int c) :
		ROW{ r }, COL{ c }, size{ r * c }, c_width{ c }, grid{ new int[size] }  {}
	//~Matrix() { delete[] grid; }
	//get value at index of matrix
	int getIndex(int x, int y) const { return grid[index(x, y)]; }
	//replace the value at index of matrix
	int replaceIndexValue(int x, int y, unsigned int Val) { return grid[index(x, y)] = Val; }

	// structure for information of each cell 
	struct cell
	{
		int cellRow, cellCol;
		unsigned int MudDepth;
		cell(unsigned int MudDepth, int cellRow, int cellCol) :
			cellRow(cellRow), cellCol(cellCol), MudDepth(MudDepth) {}
	};

	// Utility method for comparing two cells
	friend bool operator<(const cell& a, const cell& b)
	{
		if (a.MudDepth == b.MudDepth)
		{
			if (a.cellRow != b.cellRow)
				return (a.cellRow < b.cellRow);
			else
				return (a.cellCol < b.cellCol);
		}
		return (a.MudDepth < b.MudDepth);
	}

	bool isInsideGrid(int i, int j)
	{
		return (i >= 0 && i < ROW && j >= 0 && j < COL);
	}

protected:
	//indexing function to handle array like a 2d grid
	int index(int rows, int columns) const { return rows * c_width + columns; }
};

// deceleration of function to traverse through the grid finding paths
int minimum(Matrix gridToTraverse, int row, int col);

int main()
{
	int r, c;
	std::cin >> r;
	std::cin >> c;

	//creating the class class that holds all the information about the grid
	Matrix mudGrid(r, c);

	int Row{ 0 };
	int Column{ 0 };
	unsigned int MudDepth{ 0 };

	// inputing the grid to traverse
	for (Row = 0; Row < r; Row++)
	{
		for (Column = 0; Column < c; Column++)
		{
			std::cin >> MudDepth;
			mudGrid.replaceIndexValue(Row, Column, MudDepth);
		}

	}

	minimum(mudGrid, r, c);

	return 0;
}



// Method returns minimum deapth from left to right
int minimum(Matrix gridToTraverse, int row, int col)
{
	std::set< unsigned int > bestExitCell;
	//loop for different starting positions
	for (int startRow = 0; startRow < row; startRow++)
	{
		//grid to keep track of visited nodes by filling it with infinity
		Matrix searchGrid(row, col);
		// initializing distance array by INT_MAX 
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				searchGrid.replaceIndexValue(i, j, INT_MAX);

		// direction arrays for simplification of getting neighbour 
		int dRow[] = { 0, 1, 0, -1 };
		int dCol[] = { -1, 0, 1, 0 };

		// set filled with cell structs to describe vertices
		std::set<Matrix::cell> setBestVertices;

		//input first grid in set, the current cell
		setBestVertices.insert(Matrix::cell(gridToTraverse.getIndex(startRow, 0), startRow, 0));

		// initialize depthgrid(startrow, 0) with its grid value 
		searchGrid.replaceIndexValue(startRow, 0, gridToTraverse.getIndex(startRow, 0));
		
		//as long as the set of interesting vertices is not empty, check neighbours
		while (!setBestVertices.empty())
		{
			// get the cell with minimum muddepth and delete it from the set 
			Matrix::cell currentCell = *setBestVertices.begin();
			setBestVertices.erase(setBestVertices.begin());
		
			// looping through all neighbours 
			for (int i = 0; i < 4; i++)
			{
				int  row_neighbour = currentCell.cellRow + dRow[i];
				int col_neighbour = currentCell.cellCol + dCol[i];

				// if vertex is not inside grid, ignore that vertex
				if (!(searchGrid.isInsideGrid(row_neighbour, col_neighbour)))
					continue;

				if (searchGrid.getIndex(row_neighbour, col_neighbour) == INT_MAX)
				{
					//om vi inte besökt noden tidigare. Vill vi fylla noden med det högsta värdet vi måste stöta på för att ta sig dit
					int deepestMud = std::max( searchGrid.getIndex(currentCell.cellRow, currentCell.cellCol), 
						gridToTraverse.getIndex(row_neighbour, col_neighbour) );

					if (deepestMud < searchGrid.getIndex(row_neighbour, col_neighbour))
					{
						searchGrid.replaceIndexValue(row_neighbour, col_neighbour, deepestMud);
						setBestVertices.insert(Matrix::cell(deepestMud, row_neighbour, col_neighbour));
					}
				}
			}
		}

		for (int i = 0; i < row; i++)
		{
			bestExitCell.insert(searchGrid.getIndex(i, col-1));
		}

	}

	std::cout << *bestExitCell.begin();

	return 0;
		
}
	
