/*--------------- P r o g 7. c p p ---------------

by:   George Cheney
16.322 Data Structures
ECE Dept.
UMASS Lowell

PURPOSE
Find a path through a maze.

DEMOSTRATES
Stack as Linked List
Backtracking

CHANGES
2-19-2018 gpc - Create for EECE.3220 class.
3-23-2018 ers - wrote PathFound() function
*/

#include <stdlib.h>
#include <iostream>

using namespace std;

#include "CursorCntl.h"
#include "Maze.h"
#include "Stack.h"

/*----- P a t h F o u n d (  ) -----

PURPOSE
Determine whether there is a path from the current position to the goal,
and mark the path with the "path cell" indicator

RETURN VALUE
true  -- a path was found and marked
false -- no path exists
*/

bool PathFound(Maze &maze, Position &curPos)
{
	if (!maze.IsOpen(curPos))
		return false;

	maze.Visit(curPos);

	//check if current position is goal
	if (curPos == maze.Goal()) 
	{
		cin.get(); //wait for ENTER

		maze.MarkPathCell(curPos);

		return true;
	}
	//check if a path found from neighbor of current position
	else if (PathFound(maze, curPos + StepEast))
	{
		maze.MarkPathCell(curPos);
		return true;
	}
	else if (PathFound(maze, curPos + StepSouth))
	{
		maze.MarkPathCell(curPos);
		return true;
	}
	else if (PathFound(maze, curPos + StepWest))
	{
		maze.MarkPathCell(curPos);
		return true;
	}
	else if (PathFound(maze, curPos + StepNorth))
	{
		maze.MarkPathCell(curPos);
		return true;
	}

	//no open neighbors
	maze.Reject(curPos);
	return false;
}


/*--------------- m a i n ( ) ---------------*/

int main(void)
{
	// Screen positions
	const unsigned XResult = 15;
	const unsigned YResult = 5;
	const unsigned XFinish = 0;
	const unsigned YFinish = 20;

	// Construct a maze from a maze definition file.
	Maze maze;

	// Position stack remembers visited positions.
	Position curPos = maze.Start(); /////////

	// Traverse the maze.
	bool success = PathFound(maze, curPos);
	
	// Indicate success or failure.
	gotoxy(XResult, YResult);

	// Done
	gotoxy(XFinish, YFinish);

	return 0;
}
