/*--------------- P r o g 5 . c p p ---------------

by:   George Cheney
      16.322 Data Structures
      ECE Dept.
      UMASS Lowell

PURPOSE
Find the shortest path through a maze using a position queue to schedule
future positions to visit.

CHANGES
10-23-2017 gpc - Create for EECE322 class.
03-01-2018 gpc - Update for spring 2018
03-06-2018 ers - Updated solve maze
03-08-2018 ers - Retrace function, linked list queue
*/

#include <stdlib.h>
#include <iostream>

using namespace std;

#include "CursorCntl.h"
#include "Maze.h"

/*----- S o l v e (  ) -----

PURPOSE
Attempt to find the shortest path through the maze.

INPUT PARAMETERS
maze           -- the maze object to be traversed
positionQueue  -- the queue of current and future positions

RETURN VALUE
true  -- a path was found.
false -- failed to find a path.
*/
bool Solve(Maze &maze, Queue &queue)
{
   Position curPos;     // The current position

   // Move to the start cell.
   curPos = maze.Start();
   queue.Enqueue(curPos);
   maze.Mark(queue.Head(), 0);
   
   // Repeatedly find a next move until the goal is reached.
   while ( !queue.Empty()) {

		// Find an open neighbor position and go there.
	   bool found;
	   do {
		   found = false;
		   if (maze.State(queue.Head() + StepEast) == Open) {
			   curPos = queue.Head() + StepEast;
			   found = true;
		   }
		   else if (maze.State(queue.Head() + StepSouth) == Open) {
			   curPos = queue.Head() + StepSouth;
			   found = true;
		   }
		   else if (maze.State(queue.Head() + StepWest) == Open) {
			   curPos = queue.Head() + StepWest;;
			   found = true;
		   }
		   else if (maze.State(queue.Head() + StepNorth) == Open) {
			   curPos = queue.Head() + StepNorth;
			   found = true;
		   }

		   if (found) {
			   maze.Mark(curPos, maze.State(queue.Head()) + 1);
			   if (curPos == maze.Goal())
				   return true;
			   queue.Enqueue(curPos);
		   }
	   } while (found == true);
	   queue.Dequeue();
   }
   return false;
}


/*----- R e t r a c e (  ) -----

PURPOSE
Mark the path from the goal to the start cell.

INPUT PARAMETERS
maze           -- the maze object to be marked
*/
void Retrace(Maze &maze)
{
	Position curPos;     // The current position
	// Move to the start cell.
	curPos = maze.Goal();
	unsigned dist = maze.State(curPos);
	maze.Mark(curPos, PathCell);

	while (curPos != maze.Start()) {
		
		if (maze.State(curPos + StepNorth) == dist - 1) {
			curPos += StepNorth;
		}
		else if (maze.State(curPos + StepWest) == dist - 1) {
			curPos += StepWest;
		}
		else if (maze.State(curPos + StepSouth) == dist - 1) {
			curPos += StepSouth;
		}
		else if (maze.State(curPos + StepEast) == dist - 1) {
			curPos += StepEast;
		}
		dist = maze.State(curPos);
		maze.Mark(curPos, PathCell);
	}

}


/*--------------- m a i n ( ) ---------------*/

int main()
{
	// Screen positions
	const unsigned XResult = 35;
	const unsigned YResult = 5;
	const unsigned XFinish = 0;
	const unsigned YFinish = 23;
       
	Maze  maze;				// Construct a maze from a maze definition file.
	Queue positionQueue;	// Create a queue of future positions to visit

	// Solve the maze.
	bool success = Solve(maze, positionQueue);

	// Indicate success or failure.
	gotoxy(XResult, YResult);
	if (!success)
		cout << "Failed: No path from start to goal exists." << endl;
	else
		{
		cout << "Success: Found the shortest path." << endl;
		gotoxy(XResult, YResult+2);
		cout << "Press ENTER to highlight the shortest path.";
		cin.get();
		// Retrace the path from the goal cell to the start cell.
		Retrace(maze);
		cout << endl;
		}
	// Done
	gotoxy(XFinish, YFinish);
	clreol();
	return 0;
}
