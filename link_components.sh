#!/bin/bash
set -x
pushd components/nanopb || exit 1
NANOPB_FILES="pb_decode.c pb_decode.h pb_encode.c pb_encode.h pb_common.c pb_common.h pb.h"
for FILE in $NANOPB_FILES; do
    ln -s ../../3rdparty/nanopb/${FILE} $FILE
done
unset FILE
popd

pushd components/ArduinoThread || exit 1
ARDUINOTHREAD_FILES="StaticThreadController.h Thread.cpp Thread.h ThreadController.cpp ThreadController.h"
for FILE in $ARDUINOTHREAD_FILES; do
    ln -f -s ../../3rdparty/ArduinoThread/${FILE} $FILE
done
unset FILE
popd

pushd components/SdFat || exit 1
SDFAT_FILES="src/FreeStack.cpp src/FreeStack.h src/MinimumSerial.cpp src/MinimumSerial.h src/RingBuf.h src/SdFat.h src/SdFatConfig.h src/SdFatDebugConfig.h src/sdios.h src/BufferedPrint.h "
SDFAT_FILES+="src/common src/DigitalIO src/ExFatLib src/FatLib src/FsLib src/iostream src/SdCard src/SpiDriver"
for FILE in $SDFAT_FILES; do
    ln -f -s ../../3rdparty/SdFat/${FILE} .
done
unset FILE
popd

pushd components/DeviceUI || exit 1
DEVICEUI_FILES="3rdparty/meshtastic-device-ui/drivers 3rdparty/meshtastic-device-ui/generated 3rdparty/meshtastic-device-ui/include 3rdparty/meshtastic-device-ui/locale 3rdparty/meshtastic-device-ui/maps 3rdparty/meshtastic-device-ui/portduino 3rdparty/meshtastic-device-ui/resources 3rdparty/meshtastic-device-ui/source 3rdparty/meshtastic-device-ui/src 3rdparty/meshtastic-device-ui/studio"
for FILE in $DEVICEUI_FILES; do
    ln -f -s ../../$FILE .
done
unset FILE
popd

pushd components/MeshtasticProtobuf || exit 1
MESHTASTIC_PROTOBUF_FILES="3rdparty/protobufs"
for FILE in $MESHTASTIC_PROTOBUF_FILES; do
    ln -f -s ../../$FILE .
done
unset FILE
popd

pushd components/MeshtasticStandalone || exit 1
STANDALONE_UI_LOG_LIB_FILES="3rdparty/meshtastic-standalone-ui/lib/log"
rm -f lib/log
mkdir -p lib/
ln -f -s ../../../$STANDALONE_UI_LOG_LIB_FILES lib/log
popd


