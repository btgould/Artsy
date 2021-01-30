# Artsy
Lightweight art / animation tool.
# Building
The main build system is premake, however, GLEW already supplies its own Makefiles which I do not wish to overwrite. Therefore, it must be build seperately, and first. 

To build GLEW, `cd` into "Dependencies/GLEW/auto" and run `make`. When that is finished, `cd` one level up, and run `make` again. 

Everything else should be handeled by premake. Run `premake5 gmake` in the main directory, and then run `make`. The final executable should be placed in a subdirectory of "bin" which depends on the build platform and configuration. 
