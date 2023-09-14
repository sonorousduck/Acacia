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