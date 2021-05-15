# TinyTerm 
## A Terminal Game Engine

### Use on Linux:
To use on Linux, do the following:
1. In the root directory, create a "build" directory and `cd` into it.
2. Execute this command: `ccmake ..`.
3. A dialog should appear with various options, type `c`; this should configure our files for us.
4. Once returned to the main dialog, type `c` once more.
5. Type `g` to generate the makefiles.
6. Once returned to the main terminal interface, type `make`.
7. A message should appear reading: `Built target TinyTerm`.
8. `cd` into "src", and enter: `./TinyTerm`.

### Use on Windows and Visual Studio 2019:

---

#### Select the image below to view the video tutorial:
[![Windows CMake Visual Studio Tutorial Video](https://img.youtube.com/vi/oCmclfF_WKU/maxresdefault.jpg)](https://www.youtube.com/watch?v=oCmclfF_WKU)

---

To compile and use this project on Windows, and Visual Studio, you will need CMake installed. Visit cmake.org and download the appropriate version. 

To build using CMake:
1. Once Cmake is install and added to the path, open your environment variables. 
2. Under "System Variables" add a new variable titled "CMAKE_GENERATOR". 
3. The value of this variable, for me, is: `C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.28.29333\bin\Hostx64\x64\bscmake`. This may differ depending on your version. Before, when attempting to run CMake with an inappropriate generator, it simply used its default configurations to build the project. I believe the important thing here is to define the variable and set it to something that reasonably makes sense.
4. Clone the repository, then open it in your favorite editor.
5. Note the `CMakeLists.txt` files in the `root` and `src` directories, these contain the specific instructions for CMake the properly build our project.
6. Open a terminal (command prompt), and enter the following: `mkdir build && cd build`.
7. Once the `/build` directory has been created and the terminal directory has been changed, run this command: `cmake .. -G "Visual Studio 16 2019" -A x64`
8. CMake should successfully build the project; open a file explorer and navigate to the build directory.
9. A `sln` solution file should be displayed in the directory; this file should be named "TinyTerm". Open this file using Visual Studio.
10. Visual Studio should successfully launch the project, displaying the header and source files within the respective directories.

#### Notes:
- If you run the Debugger in Visual Studio and get an "Access Denied" error, it is most likely due to Visual Studio attempting to run the `ALL_BUILD` solution instead of `TinyTerm`. To fix this, do the following:
1. Right click on the "Solution 'Tiny Term'" at the top of the Solution Explorer.
2. Select "Set Startup Projects..."
3. A dialog window should appear; make sue "Startup Project" is selected under "Common Properties" on the left hand selection menu.
4. There are 3 radio buttons that appear, select "Single startup project" if not selected.
5. From the dropdown menu, select "TinyTerm" instead of "ALL_BUILD".
6. Apply changes and close the window; select the "Local Windows Debugger" once again to run the program.
7. A terminal interface should appear displaying to program.

### Curses Integration on Windows Visual Studio:
Curses is a Unix-specific library. To get around this, we can use Microsoft's `vcpkg` utility, found here: [vcpkg](https://github.com/Microsoft/vcpkg).

To use this, do the following:
1. Using the link above, clone the git repository to anywhere you'd like.
2. Navigate to the `vcpkg` directory and type: `./bootstrap-vcpkg.bat`.
3. Add this directory to your Windows system/user path.
4. To integrate with Visual Studio, type the following: `vcpkg integrate install`.
5. Open the project in Visual Studio, and navigate to `engine.hpp`. 
6. Most likely, the include statement for: `curses.h`. 
7. Open the terminal and type the following: `vcpkg install pdcurses:x64-windows`.
8. After that is installed, type `vcpkg integrate project`.
9. Navigate back to Visual studio, then go to the top bar and do the following: _Tools->NuGet Package Manager->Package Manager Console_.
10. This will open a console at the bottom of the screen; within it, enter: `Install-Package vcpkg.C.vclib.vcpkg -Source "C:\vclib\vcpkg\scripts\buildsystems"` where `C:\vclib\vcpkg` is the installation directory for `vcpkg`, i.e. the place where you cloned it from Github.
11. Return to `engine.hpp`; the include statement for Curses should no longer be red.
12. Attempt to build and run; if successful, the program should display!

#### Notes:
If the an error occurs when attempting to launch the program indicating that the `pdcurses.dll` could not be found; do the following:
1. Right click on the TinyTerm solution and select _Properties_.
2. In the TinyTerm Property Pages dialog, select _Linker_ from the right hand navigation menu.
3. Under the Linker dropdown, select _Input_.
4. Within the Input menu, add `pdcurses.lib` to the _Additonal Dependencies_ line, and then apply the changes.