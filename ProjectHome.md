<a href='http://picasaweb.google.com/tiago.dionizio/PuzzlesS60?authkey=Gv1sRgCN-I05ztx5T8zgE&feat=embedwebsite'><img src='http://lh6.ggpht.com/_fJQSWco-qh8/TTSjNluAPwE/AAAAAAAAFs0/jLuaXRAsjmE/s360/PuzzlesS60.jpg' align='right' /></a>
<table cellpadding='0' border='0' cellspacing='0' valign='top'>
<tr>
<td>
This is port of Simon Tatham's Portable Puzzle Collection, a collection of logic games, to the Symbian platform.<br>
</td>

</tr>
</table>

It runs on devices with Symbian 3rd and 5th Edition (also known as Symbian`^`1). It should also work in Symbian`^`3 platform.

## What it does ##

  * Allows you to play more than 30 logic games!
  * Supports 3rd Edition phones - keypad input
  * Supports 5th Edition phones - touch screen input

## News ##

  * 2011-03-26 - version 1.5r9411
    * updated games code to svn [revision 9411](https://code.google.com/p/puzzless60/source/detail?r=9411)
    * new game: pearl
    * loopy: added unsupported code that enables cursor input for 3rd ed phones (code taken from the Android port by Chris Boyle)

  * 2011-04-26 - version 1.5r9163
    * updated games code to svn [revision 9163](https://code.google.com/p/puzzless60/source/detail?r=9163)
    * (loopy: 2 new grid types - penrose)

  * 2011-02-28 - version 1.5r9109
    * added new game range from svn [revision 9109](https://code.google.com/p/puzzless60/source/detail?r=9109)
    * (loopy: 3 new grid types)

  * 2010-10-20 - version 1.5r9014
    * updated games code to svn [revision 9014](https://code.google.com/p/puzzless60/source/detail?r=9014) (small updates)
    * fixed: circles were not being drawn with the correct size, they were one pixel short on the right and bottom sides
    * new touch ui: tap application title to show game selection list
    * new touch ui: tap game preset (bellow title) to show game preset menu
    * new touch ui: long tap 1/4 of a second to switch between stylus selection modes
    * new keypad: for games that don't require numeric input, the number keys will be mapped to the numeric keypad (on PC) input, so games like inertia will allow diagonal moves

  * 2010-09-15 - version 1.4r8997
    * added new game range from svn [revision 8997](https://code.google.com/p/puzzless60/source/detail?r=8997)

  * 2010-09-01 - version 1.4r8965
    * updated games code to svn [revision 8965](https://code.google.com/p/puzzless60/source/detail?r=8965) (small updates)
    * added help files (adapted from original documentation)

  * 2010-05-27 - version 1.3r8951
    * reorganized the application menu items
    * save the screen orientation on exit

  * 2010-05-18 - version 1.2r8951
    * updated games code to svn [revision 8951](https://code.google.com/p/puzzless60/source/detail?r=8951)
    * better support for saving last game
      * now it saves the last game/profile for each type of game
      * saves screen size selection
    * added screen orientation configuration (doesn't save on exit)

  * 2010-02-27 - version 1.1r8887
    * updated games code to svn [revision 8887](https://code.google.com/p/puzzless60/source/detail?r=8887)
    * fixed game settings (Solo game crashed when customizing game parameters)
    * added basic autoload/save at startup/exit - it will only save the current game
    * increased application stack size - Solo game would crash on 4x4 board size

  * 2010-02-18 - version 1.1r8872
    * initial release

## Download ##

Installation files are made available for the Symbian 3rd and 5th edition playform in the download section.

For older 3rd edition models, you need the **PIPS** plugin to be installed, for those cases a separate download is available that includes the plugin installation - not visible in the featured download list.

## Supported Models ##

  * 3rd Edition
    * Visit forum.nokia.com site for a complete list.
  * 5th Edition
    * Nokia 5800 XM, N97, 5530 XM, 5230, X6-00, N97 mini, 5235 ME, 5230 Nuron, C6-00
  * Symbian^3
    * Nokia N8

## Contact information ##

  * author: Tiago Dionizio
  * email: puzzles.s60@gmail.com

## Known problems ##

  * The galaxies game is memory hungry for large board sizes
  * The screen size option is far from optimal, but works.
  * There is no way to enter letters (a-z). Ex: 4x4 Solo games require you to fill cells with 1-9 numbers and a-g letters.
  * If the board size is too big:
    * Text may become unreadable (screen size limitation);
    * Game generation may take a while to complete depending on the board size and difficulty setting;
    * Game may need a reasonable amount of memory (ex: galaxies) to keep intermediate game states.

## Input - keypad ##

  * joystick to move across the board
  * middle joystick - selection button
  * C : alternate selection button
  * 0-9 : specify number for certain games
  * `*` : undo move
  * # : redo move

## Input - touch screen ##

  * pen : selection/action
  * to enter numbers, a virtual keyboard is shown at the bottom
  * 0-9 : specify number for certain games
  * U : undo move
  * R : redo move
  * S and / : current selection mode (similar to left and right mouse click)
    * S : normal selection
    * / : alternate selection
    * M : menu

## Screen size options ##

  * Normal
  * Large Top - hide application title
  * Large Bottom - hide softkeys
  * Full

## Wish list (some time in the future) ##

  * Manual save and load support
  * Save custom game settings

## Links of interest ##

  * [Simon Tatham's Portable Puzzle Collection](http://www.chiark.greenend.org.uk/~sgtatham/puzzles/)
  * [Developer Documentation](http://www.chiark.greenend.org.uk/~sgtatham/puzzles/devel/)
  * [Original sources svn repository](http://svn.tartarus.org/sgt/puzzles/)
  * [S60 5th Edition Cpp Developers Library](http://library.forum.nokia.com/index.jsp?topic=/S60_5th_Edition_Cpp_Developers_Library/GUID-07F7CEA3-DC49-40E1-B313-5AAC1A5A3F8D_cover.html)