# Developer Log

## Log #2: Sunday, January 12th, 2025

I finally figured out how to use SDL through Lua (I forgot to mention that the game is written in C)! I have not implemented my solution yet, however. I found it by reading through [Programming in Lua](https://www.lua.org/pil/) in section 27.3.1! I will use the registry that it describes to have a way to access the SDL renderer to render things!  

I am finishing this part of the log after my coding session for the day (the first part was before) and I found a much easier way to render textures that I choose through Lua (store their filepaths in a table, loop through all of the filepaths and load each one or something like that). I will implement that later!  

Thank you once again for reading my log, and have a great day!~

Danielle ♡

## Log #1: Friday, January 10th, 2025

I have decided to create a developer log to write down my thoughts as ManySnakes grows in size!  

I am a little overwhelmed with the project. So far, the game can open to a main menu screen (which is currently blank as I make proper textures and code infrastructure for buttons and other UI) and once you press the enter key, you can play the classic Snake game (with border looping)! You can pause with the escape key and it dims the screen until you press the escape key again. The graphics are minimal, with a poorly created (by me, of course) apple texture and everything else being drawn with solid colors. There is no UI yet while playing the game or in the pause screen. When you lose, you return to the main menu. You can exit the game at any time by exiting the window.  

What I'm currently working on:
- Embedding Lua into the game to provide scripting and modding capabilities for my convenience while developing the game and for the enjoyment of whoever wants to play my game!
- Creating an resource manager to more simply create, use, reuse, and destroy assets!

Once those two are largely done, I want to be able to create and edit UI elements on the fly through Lua scripting!  

Thank you for reading this log, and have a nice day!~  

Danielle ♡
