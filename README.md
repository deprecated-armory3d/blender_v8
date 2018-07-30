# blender_v8

## Intro guide
https://wiki.blender.org/wiki/Building_Blender

## Windows
- mkdir blender-git
- cd blender-git
- svn checkout https://svn.blender.org/svnroot/bf-blender/trunk/lib/win64_vc14  lib/win64_vc14
- git clone https://github.com/armory3d/blender_v8 blender
- cd blender
- .\make full nobuild x64 2015
- Build `INSTALL` project
- Copy V8 deployment libs from `/blender/source/blender/draw/engines/armory/Deployment/release/win32` to Blender.exe location

## Linux
- sudo apt-get update; sudo apt-get install git build-essential
- mkdir blender-git
- cd blender-git
- git clone https://github.com/armory3d/blender_v8 blender
- ./blender/build_files/build_environment/install_deps.sh
- mkdir build
- cd build
- cmake ../blender
- make
- make install
- patchelf --set-rpath '$ORIGIN' blender
- patchelf --set-rpath '$ORIGIN' .so libs

## MacOS
- mkdir blender-git
- cd blender-git
- svn checkout https://svn.blender.org/svnroot/bf-blender/trunk/lib/darwin lib/darwin
- git clone https://github.com/armory3d/krom_blender blender
- mkdir build
- cd build
- cmake ../blender -G Xcode
- Open xcodeproj, set install scheme, set Blender.app executable
- Blender project - Build Settings - add rpath: @loader_path/../Resources/macos
- Copy v8 deployment libs to Blender.app/Contents/Resources/macos

---
