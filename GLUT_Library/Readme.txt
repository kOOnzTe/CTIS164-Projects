INSTALLING FREEGLUT LIBRARY :

If you have Visual Studio 2022, its default folder: C:\Program Files\Microsoft Visual Studio\2022\Community\VC

STEPS:
------------------------------------
1. Copy all header files:
   a) Create a "GL" folder under
 	"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.30.30705\include"
   b) Copy all files in "include" folder of this zip file into
 	"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.30.30705\include\GL"
2. Copy freeglut.lib in "lib" folder into
   "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.30.30705\lib\x86" folder.
3. Copy freeglut.dll in "dll" folder into
   "C:\Windows\" and "C:\Windows\System32\" and "C:\Windows\System\" folders.

You can perform similar steps for the other versions of Visual Studio. Only the folder locations will change.
