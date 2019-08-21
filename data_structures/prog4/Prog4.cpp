/*--------------- P r o g 4 . c p p ---------------

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
2-26-2018 ers - corrected solveMaze() function, created linked-list Stack class
*/

#include <stdlib.h>
#include <iostream>

using namespace std;

#include "CursorCntl.h"
#include "Maze.h"
#include "Stack.h"

/*----- S o l ve M a z e (  ) -----

PURPOSE
Naive maze traversal algorithm. Try all possible next
positions, but give up at a dead end..

RETURN VALUE
true  -- a solution was found.
false -- failed to find a solution.
*/

bool SolveMaze(Maze &maze, Stack &stack)
{
	// Make the start cell the current position..
	stack.Push(maze.Start());
	// Make the start cell the current position.
	maze.Visit(stack.Top());
	// Repeatedly try to find a next move until the goal is reached.
	do
	{
			Position neighbor;     // The position of an open neighbor

			if (maze.IsOpen(stack.Top() + StepEast)) {
				neighbor = stack.Top() + StepEast;
				// Make the open neighbor the current position.
				stack.Push(neighbor);
				// Mark the current position visited.
				maze.Visit(stack.Top());
			}
			else if (maze.IsOpen(stack.Top() + StepSouth)) {
				neighbor = stack.Top() + StepSouth;
				stack.Push(neighbor);
				maze.Visit(stack.Top());
			}
			else if (maze.IsOpen(stack.Top() + StepWest)) {
				neighbor = stack.Top() + StepWest;
				stack.Push(neighbor);
				maze.Visit(stack.Top());
			}
			else if (maze.IsOpen(stack.Top() + StepNorth)) {
				neighbor = stack.Top() + StepNorth;
				stack.Push(neighbor);
				maze.Visit(stack.Top());
			}
			else {
				maze.Reject(stack.Top());
				stack.Pop();
				if (stack.Empty())
					return false;
			}
	} while (stack.Top() != maze.Goal());
	// Found a path.
	return true;
}


/*--------------- m a i n ( ) ---------------*/

int main(void)
{
	// Screen positions
	const unsigned XResult = 15;
	const unsigned YResult = 5;
	const unsigned XFinish = 0;
	const unsigned YFinish = 20;

	// Position stack remembers visited positions.
	Stack posStack;

	// Construct a maze from a maze definition file.
	Maze maze;

	// Traverse the maze.
	bool success = SolveMaze(maze, posStack);
	// Indicate success or failure.
	gotoxy(XResult, YResult);
	if (!success)
		cout << "Failed: No path from start to goal exists." << endl;
	else
	{
		cout << "Success: Found a path. Press ENTER to retrace..." << endl;

		cin.get();  // Wait for ENTER.

					// Retrace the path back to the goal.
		while (!posStack.Empty())
			maze.MarkPathCell(posStack.Pop());
	}

	// Done
	gotoxy(XFinish, YFinish);

	return 0;
}
