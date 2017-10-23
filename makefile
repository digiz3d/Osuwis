GCC=C:\mingw64\bin\g++.exe
WINDRES=C:\mingw64\bin\windres.exe
IMAGEMAGICK="C:\Program Files\ImageMagick-7.0.5-Q16\magick.exe"

CFLAGS=-W -Wall -Wextra -std=c++17
LDFLAGS=-static -s -mwindows

SRCDIR=src
OBJDIR=obj
BINDIR=bin
BINARY=Osuwis

all: clean directories $(BINDIR)/$(BINARY)

$(BINDIR)/$(BINARY): $(OBJDIR)/main.o
	$(IMAGEMAGICK) icons/icon16.ico icons/icon32.ico icons/icon48.ico icons/icon128.ico icons/icon256.ico $(OBJDIR)/icon.ico
	$(WINDRES) $(SRCDIR)/resources.rc $(OBJDIR)/resources.o
	$(GCC) -o $@ $< $(OBJDIR)/resources.o $(LDFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	$(GCC) -o $@ -c $< $(CFLAGS)


run: $(BINDIR)/$(BINARY)
	$(BINDIR)/$(BINARY)

.PHONY: directories clean

directories: $(OBJDIR) $(BINDIR)
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	@if not exist $(OBJDIR) mkdir $(BINDIR)

clean:
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	@if not exist $(OBJDIR) mkdir $(BINDIR)
	@del /s /q $(OBJDIR)\*
	@del /s /q $(BINDIR)\*