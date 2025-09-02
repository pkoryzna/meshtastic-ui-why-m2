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
    ln -f -s ../../3rdparty/ArduinoThread/${FILE} $FILE
done
unset FILE
popd

pushd components/SdFat
SDFAT_FILES="src/FreeStack.cpp src/FreeStack.h src/MinimumSerial.cpp src/MinimumSerial.h src/RingBuf.h src/SdFat.h src/SdFatConfig.h src/SdFatDebugConfig.h src/sdios.h src/BufferedPrint.h "
SDFAT_FILES+="src/common src/DigitalIO src/ExFatLib src/FatLib src/FsLib src/iostream src/SdCard src/SpiDriver"
for FILE in $SDFAT_FILES; do
    ln -f -s ../../3rdparty/SdFat/${FILE} .
done
unset FILE
popd

pushd components/DeviceUI
DEVICEUI_FILES="3rdparty/meshtastic-device-ui/drivers 3rdparty/meshtastic-device-ui/generated 3rdparty/meshtastic-device-ui/include 3rdparty/meshtastic-device-ui/locale 3rdparty/meshtastic-device-ui/maps 3rdparty/meshtastic-device-ui/portduino 3rdparty/meshtastic-device-ui/resources 3rdparty/meshtastic-device-ui/source 3rdparty/meshtastic-device-ui/src 3rdparty/meshtastic-device-ui/studio"
for FILE in $DEVICEUI_FILES; do
    ln -f -s ../../$FILE .
done
unset FILE
popd

pushd components/MeshtasticProtobuf
MESHTASTIC_PROTOBUF_FILES="3rdparty/meshtastic-standalone-ui/lib"
for FILE in $MESHTASTIC_PROTOBUF_FILES; do
    ln -f -s ../../$FILE .
done
unset FILE
popd

pushd main
STANDALONE_UI_FILES="3rdparty/meshtastic-standalone-ui/3rdparty 3rdparty/meshtastic-standalone-ui/src"
for FILE in $STANDALONE_UI_FILES; do
    ln -f -s ../$FILE .
done
unset FILE
popd


