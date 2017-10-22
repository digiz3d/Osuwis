# Description
Detects the song playing in Osu! and writes it to a .txt file.
Originally made for Ezywis' stream.

# Usage
1. Enable the MSN integration setting in Osu!
2. Start this software.
3. Play any song in Osu and a .txt file will pop next to Osuwis.
4. Use that .txt file to display the song's name on your stream :)

# Compilation
## Requirements
mingw-w64, that supports the C++17 revision. I don't use any particular C++17 feature so you could change that, I guess.  
ImageMagick 7 or later to produce the multi-size icon that will be included in the software.

## Instructions
Edit the makefile to fit your needs and execute something like `C:\mingw64\bin\mingw32-make.exe -f makefile`
