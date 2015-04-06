#!/bin/sh

TMP=/tmp/android-project
CWD=$(pwd)
JNI=$CWD/android-project/jni

rm -rf $TMP
mkdir $TMP && cd $_

wget "https://www.libsdl.org/release/SDL2-2.0.3.tar.gz"
wget "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.0.tar.gz"
wget "http://www.lua.org/ftp/lua-5.2.3.tar.gz"
wget "http://downloads.sourceforge.net/project/ogl-math/glm-0.9.6.3/glm-0.9.6.3.zip"

# debug
# cp ../SDL2-2.0.3.tar.gz .
# cp ../SDL2_image-2.0.0.tar.gz .
# cp ../lua-5.2.3.tar.gz .
# cp ../glm-0.9.6.3.zip .

tar xvf SDL2-2.0.3.tar.gz
tar xvf SDL2_image-2.0.0.tar.gz
tar xvf lua-5.2.3.tar.gz
unzip glm-0.9.6.3.zip

cd $CWD
cp -an $TMP/SDL2-2.0.3/android-project .
cp -an $TMP/SDL2-2.0.3/. $JNI/SDL2
cp -an $TMP/SDL2_image-2.0.0/. $JNI/SDL2_image
cp -an $TMP/lua-5.2.3/. $JNI/lua

mkdir $JNI/include
cp -an $TMP/glm/glm $JNI/include
ln -sf ../SDL2/include $JNI/include/SDL2
cp -an $JNI/SDL2_image/SDL_image.h $JNI/include/SDL2
ln -sf ../lua/src $JNI/include/lua

ASSETS=android-project/assets
mkdir $ASSETS
ln -sf ../../textures/ $ASSETS/textures
ln -sf ../../scripts/ $ASSETS/scripts

ln -sf ../../../src $JNI/src/src

android update project --path android-project/ --target 1
