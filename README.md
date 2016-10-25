# dip_unpacker
An unpacking tool for DiP Interactive games (*Nu, pogodi! Dogonyalki* etc.) **(WORK IN PROGRESS)**

How to use: compile, put *base.res* file into the folder with executable, run and get several thousands of unpacked files. :)

The tool is still incomplete and unpacks only about 80% of game resource file. Also, it has only been tested with *Nu, pogodi! Dogonyalki* game.

*base.res* file format (byte order: Little Endian):

`uint32 - signature (6RaP, 0x36526150)`

`uint32 - files count [-1]`

`// for each file`

` char - type? (1 - some text info, 8 - PNG, 9 or 0xC - OGG, 0xB - ogv)`

` uint32 - size?`

` uint32 - offset?`

`// game data`

Have fun!

greencis, 2016
