# Scatter-Blast
A 2D shoot 'em up game with color matching mechanics.

This Game was created in C using visual studio. It utalizes the Simple DirectMedia Layer (SDL) libraries found at https://www.libsdl.org
for simple graphics. In order to compile this project yourself, you will have to link it with the SDL libriaries which I found easiest to
do with visual studio. A fairly comprehensive guide on how to do this can be found here (https://www.wikihow.com/Set-Up-SDL-with-Visual-Studio-2017).
I wrote and compiled this project using windows 10, but in theory one should be able to compile and run the game on most operating systems 
as SDL provides fairly universal support. I included a compiled version of the game in the folder named 'scatter blast'. This folder also contains the bitmap images that you will need to run the game and a copy of the 32-bit SDL dll.

The game also utalizes a random number generation library found here http://www.pcg-random.org/. This library is liscensed under apache 2.0 which requires the full text of the liscense to be included in any redistribution, so I will include this in the file "pcg liscense.txt". This liscense only applies to the files pcg_basic.c and pcg_basic.h.

![Screenshot](https://github.com/seth-tinglof/Scatter-Blast/screen_cap.png)
