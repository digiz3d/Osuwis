GCC=C:\mingw64\bin\g++.exe
WINDRES=C:\mingw64\bin\windres.exe
IMAGEMAGICK="C:\Program Files\ImageMagick-7.0.5-Q16\magick.exe"

CFLAGS=-W -Wall -Wextra -std=c++17
LDFLAGS=-static -s -mwindows
EXEC=beclean Osuwis clean

all: $(EXEC)

Osuwis: main.o
	$(IMAGEMAGICK) icons/icon16.ico icons/icon32.ico icons/icon48.ico icons/icon128.ico icons/icon256.ico icon.ico
	$(WINDRES) resources.rc resources.o
	$(GCC) -o $@ $< resources.o $(LDFLAGS)

main.o: main.cpp
	$(GCC) -o $@ -c $< $(CFLAGS)

beclean:
	del *.o
	
clean:
	del *.o
	del *.ico
