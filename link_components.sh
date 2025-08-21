#!/bin/bash
set -x
pushd components/nanopb

NANOPB_FILES="pb_decode.c pb_decode.h pb_encode.c pb_encode.h pb_common.c pb_common.h pb.h"
for FILE in $NANOPB_FILES; do
    ln -s ../../3rdparty/nanopb/${FILE} $FILE
done
unset FILE
popd

pushd components/ArduinoThread
ARDUINOTHREAD_FILES="StaticThreadController.h Thread.cpp Thread.h ThreadController.cpp ThreadController.h"
for FILE in $ARDUINOTHREAD_FILES; do
    ln -s ../../3rdparty/ArduinoThread/${FILE} $FILE
done
unset FILE
popd

pushd components/SdFat
SDFAT_FILES="src/FreeStack.cpp src/FreeStack.h src/MinimumSerial.cpp src/MinimumSerial.h src/RingBuf.h src/SdFat.h src/SdFatConfig.h src/SdFatDebugConfig.h src/sdios.h src/BufferedPrint.h"
SDFAT_FILES+="src/common src/DigitalIO src/ExFatLib src/FatLib src/FsLib src/iostream src/SdCard src/SpiDriver"
for FILE in $SDFAT_FILES; do
    ln -s ../../3rdparty/SdFat/${FILE} .
done
unset FILE
popd