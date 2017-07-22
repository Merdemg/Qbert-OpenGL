# Qbert-OpenGL
QBert clone. Used OpenGL, c++

Arrows to navigate menus, W-E-S-D to control QBert. You will need to enter name on console to start the game. Make game the active window to continue.


Milestone 3 Mechanics:

1) Elevator Disc: At the start of each level, two discs will be spawned on random positions on each side. If qbert jumps on one, it carries qbert to top.
2) Coily: Green, red and purple balls spawn at the top of the level. If a purple ball reaches bottom, it turns to Coily (purple hexagon). Coily damages player if it touces. Colliding with the purple before it hatches just gives bonus points.
3) Leaderboard: The game keeps track of Top 3 scores. Player enter names on console, top3 scores are displayed. After game is over and player is returned to main screen, if player score is a high score, table is updated and saved.

Leaderboard ONLY WORKS ON EXE, and not on Visual Studio  debugger.
(Classworkshop.exe in folder EXE)

4) Progressive difficulty: First level requires only one change for each cube. Starting from 2nd level, the player will have to change each cube twice.
5) Game over: If player loses 3 lives, game is over and is returned to main menu. High scores and player scores can be checked on the console.
6) Sound effects: Player lose life, player collects green ball, Level complete and menu enter events call sound effects.

Sound effects only work on WINDOWS and ONLY FROM EXE and not on Visual Studio debugger.
