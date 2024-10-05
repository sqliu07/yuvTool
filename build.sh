#!/bin/bash
CWD=$(pwd)
OS=$(uname -s)
PRODUCT=yuvTool
MKDIR=$CWD/mk
SRCDIR=$CWD/src
BINDIR=$CWD/bin
NDK_PATH=~/workspace/devtools/android-ndk-r26c


function build_android
{
#prepare
  OUTDIR=$BINDIR/$PRODUCT/android
  mkdir -p $OUTDIR
  if [ ! -d "$NDKDIR" ]; then
    ANDROID_NDK=~/workspace/devtools/android-ndk-r26c # change this on different platform
  else
    ANDROID_NDK=$NDKDIR
  fi
  # Check if the NDK is installed
  if [ ! -d "$NDK_PATH" ]; then
      echo "Please set the correct path to the Android NDK."
      exit 1
  fi

  # Run ndk-build
  export PATH=$ANDROID_NDK:$PATH
  echo "compiling $PRODUCT for Android ..."
  proc=$(nproc)
  procs=$((2 * proc))
  ndk-build -j"$procs" NDK_DEBUG=0 NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=$MKDIR/android.mk NDK_APPLICATION_MK=$MKDIR/application.mk || exit 1
  cp -rf libs/* $OUTDIR
  #clean
  rm -rf obj libs
  #done
  echo "Done"
}

function build_pc {
  #prepare
  if [ "$OS" = "Linux" ]; then
    longbit=`getconf LONG_BIT`
    case $longbit in
      32)
      export target=x86
      ;;
      64)
      export target=x64
      ;;
    esac
    OUTDIR=$BINDIR/$PRODUCT/linux/$target
    OUTFILE=$PRODUCT
  fi
  mkdir -p $OUTDIR
  
  cd $SRCDIR
  echo "compiling $OUTFILE for $OS..."
  proc=$(nproc)
  procs=$((2 * proc))
  make -j"$procs" -f $MKDIR/yuv.mk || exit 1
  mv -f $SRCDIR/$OUTFILE $OUTDIR
  cd -
  #done
  echo "Build $OUTFILE Done"
}

case "$1" in
  android)
  build_android || exit 1
  ;;
  pc)
  build_pc || exit 1
  ;;
  *)
  echo "Invalid Input! Only android/pc are supported!"
  ;;
esac