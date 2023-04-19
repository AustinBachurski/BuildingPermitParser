
# Building Permit Parse Tool

## Udacity C++ Nanodegree Capstone Project


I don't really have an interest in game development and would much rather put effort into building something useful so for the final project in the C++ Nanodegree program I chose to build a project of my choice, a spreadsheet parsing tool to make work easier for a coworker.<br>

This tool parses out building permit information from a tracking spreadsheet to quickly provide totals for year end reports.  The tool automatically loads and parses the sheet for the previous calendar year when ran and displays the information to the user.  A "Copy" button near each displayed value loads the information into the operating system clipboard so it can be easily pasted into a report document with minimal effort required by the user.

<br>

// Image goes here?

<br>

## Dependencies for Running Locally // CHECK THE PROJECT WORKSPACE FOR VERSIONS!

### CMake

*CMake is used to control the software compilation process using simple platform and compiler independent configuration files, and generate native makefiles and workspaces that can be used in the compiler environment of your choice.*
  * Version 3.25 or higher.
  * All OSes - [Installation instructions.](https://cmake.org/install/)

### Make

*Make is a tool which controls the generation of executables and other non-source files of a program from the program's source files.*
* Version 4.1 or higher for Linux or Mac, version 3.81 or higher for Windows.
* Linux - Arch based systems - `sudo pacman -S make` |  Debian based systems - `sudo apt install make`
* Mac - [Install Xcode command line tools.](https://developer.apple.com/xcode/features/)
* Windows - [Installation instructions.](http://gnuwin32.sourceforge.net/packages/make.htm)

### GCC/G++

*The GNU Compiler Collection includes front ends for C, C++, Objective-C, Fortran, Ada, Go, and D, as well as libraries for these languages (libstdc++,...).*
* Version 5.4 or higher.
* Linux - Arch based systems - `sudo pacman -S gcc` | Debian based systems - `sudo apt install build-essential`
* Mac - same deal as make - [Install Xcode command line tools.](https://developer.apple.com/xcode/features/)
* Windows - [Install MinGW](http://www.mingw.org/)

### OpenXLSX >= 0.3.2

*OpenXLSX is a C++ library for reading, writing, creating and modifying Microsoft Excel® files, with the .xlsx format.*
* Version 0.3.2 or higher.
* [GitHub Repository](https://github.com/troldal/OpenXLSX)

### wxWidgets
*wxWidgets is a C++ library that lets developers create applications for Windows, macOS, Linux and other platforms with a single code base.*
  * [Installation instructions.](https://wiki.wxwidgets.org/Install)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Move the spreadsheet from the 'test' directory into the build directory: `mv ../test/"Building Permits.xlsx" .`
5. Run: `./BuildingPermitParser`

## Rubric Points Addressed

1. The project demonstrates an understanding of C++ functions and control structures: `parser.cpp` lines 41 through 278.
2. The project reads data from a file and process the data, or the program writes data to a file: `parser.cpp` lines 119 through 263.
3. The project uses Object Oriented Programming techniques: `parser.hpp`
4. Classes use appropriate access specifiers for class members: `parser.hpp` lines 23 & 48.
5. Class constructors utilize member initialization lists: `parser.cpp` lines 3 through 36.
6. Classes abstract implementation details from their interfaces: `parser.hpp` & `parser.cpp`
7. The project makes use of references in function declarations: `parser.hpp` lines 42 through 46 and lines 60 through 66.

