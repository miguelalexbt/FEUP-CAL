COMPILE:

-- EASY WAY --

Please follow the instructions for an easy setup:

1) Create a new Visual Studio C++ project.

2) Copy the .rar contents to the folder $(PROJECT_PATH)\(PROJECT_NAME)
	Example with a project named "CAL":
		Copy the contents to $(PATH)\CAL\CAL

3) In Visual Studio, in Solution Explorer, click on "Show all files" option (on the top of the tab), right-click in the folder "src" and select "Include in Project".
   Do the same for the folder "include".

4) Right-click in the project name and select "Properties" (ALT-ENTER).

5) Go to "Configuration Properties" -> "C/C++" -> "General" -> "Additional Include Directories", add the folder "include" to the list.

6) Go to "Configuration Properties" -> "Linker" -> "General" -> "Aditional Library Directories", add the folder "lib" to the list.

7) Go to "Configuration Properties" -> "Linker" -> "Input" -> "Additional Dependencies", add "libcurl.lib".

8) You can now build the project (not ready to run yet!).

9) Finally, copy the ".dll" files from the folder "bin" to the folder where the generated ".exe" is located.

10) You can now run the program!

-- HARD WAY --

In case the easy way didn't work, do the following:

1) Download (or clone the repository) the management tool "vcpkg" (https://github.com/Microsoft/vcpkg).

2) Open a console and change directory to the folder you just downloaded.

3) Run the file "bootstrap-vcpkg.bat".

4) Enable user-wide integration (so that Visual Studio recognizes the dependencies):
	>.\vcpkg integrate install

5) Install required dependencies:
	>.\vpckg install curl cpr nlohmann-json

6) Visual Studio should now recognize the dependencies and will build the project!

7) To uninstall:
	>.\vpckg remove curl cpr nlohmann-json
	>.\vcpkg integrate remove
	Delete the folder of the tool.

Documentation was not generated, because of lack of time.

