## Running instructions

There are a few different Third Party libraries you have to build

Make sure that you compile and build 

```
openal-soft
freetype
assimp
box2d
```

For the assimp build, you will then need to grab the config.h that is generated in ThirdParty/assimp/build/include/assimp and bring it up to the ThirdParty/assimp/include/assimp folder. Just copy and paste it in

For box2d, make sure you disable BOX2D_BUILD_TESTBED, as this links to libraries already included, causing all sorts of problems


You will also need to install SDL2. If you are on windows, that is the most complicated:

```
Download SDL2 from here https://www.libsdl.org/
Extract the package into a directory
In the CMake GUI, you will need to set the variable SDL2_DIR to the cmake of that extracted folder, wherever that may be
It should generate and build correctly after that point! :)
If you are on windows, you will also need to grab the dll from the lib/x64 folder (SDL2.dll) and copy it into the build folder as well (Until I write something to automatically do this)
```