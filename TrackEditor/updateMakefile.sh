#!/bin/sh

PRO=TrackEditor.pro
BUILDCONFIG=debug
BUILDARCH=x64

for var in "$@"
do
  if [ "$var" = "release" ]
  then
   BUILDCONFIG=release
  fi
done

QTPATH=../external/Qt5/g++13.3.0-x64-a
TARGET=../bin64/TrackEditor/TrackEditor
BUILDSPEC=linux-g++-64

QMAKE=${QTPATH}/bin/qmake

echo "qmake version:"
${QMAKE} -v

echo "Executing: ${QMAKE} -project -norecursive -o ${PRO}"
${QMAKE} -project -norecursive -o ${PRO}

echo QT += xml >> ${PRO}

if [ "$BUILDCONFIG" == "release" ]
then
  echo CONFIG -= debug >> ${PRO}
  echo DEFINES += NDEBUG >> ${PRO}
else
  echo CONFIG += debug >> ${PRO}
  echo DEFINES -= NDEBUG >> ${PRO}
  echo DEFINES += _DEBUG __DEBUG >> ${PRO}
fi

echo TEMPLATE = app >> ${PRO}
echo CONFIG += console precompile_header >> ${PRO}
echo INCLUDEPATH += ../WhipLib \
                    ${QTPATH}/include \
                    ${QTPATH}/include/QtCore \
                    ${QTPATH}/include/QtWidgets \
                    ${QTPATH}/include/QtOpenGL \
                    ${QTPATH}/include/QtGui \
                    ../external/glew/include/GL \
                    ../external/glm >> ${PRO}

echo INCLUDEPATH -= ui >> ${PRO}
echo MOC_DIR = lmoc >> ${PRO}
echo UI_DIR = lui >> ${PRO}
echo TARGET = $TARGET >> ${PRO}
echo OBJECTS_DIR = obj/$BUILDCONFIG.$BUILDARCH >> ${PRO}

echo SOURCES -= qrc_resource.cpp >> ${PRO}

echo LIBS += ${QTPATH}/lib/libQt5Core.a \
             ${QTPATH}/lib/libQt5Widgets.a \
             ${QTPATH}/lib/libQt5Gui.a \
             ${QTPATH}/lib/libQt5OpenGL.a \
             ../lib/WhipLib.a \
             ../external/glew/lib/libGLEW.a \
             ../external/FBX/lib/release/libalembic.a \
             ../external/FBX/lib/release/libfbxsdk.a \
             -lGL \
             -lxml2 \
             -lz >> ${PRO}

if [ "$BUILDCONFIG" == "release" ]
then
  echo QMAKE_CXXFLAGS += -g >> ${PRO}
  echo QMAKE_POST_LINK += cp $TARGET $TARGET.archive \&\& >> ${PRO}
  echo QMAKE_POST_LINK += strip $TARGET \&\& >> ${PRO}
fi

echo QT += widgets >> ${PRO}
echo QMAKE_CXXFLAGS += -D GLM_ENABLE_EXPERIMENTAL >> ${PRO}
echo QMAKE_LFLAGS += -static-libgcc -static-libstdc++ >> ${PRO}

echo "Executing: ${QMAKE} ${PRO} -spec $BUILDSPEC"
${QMAKE} ${PRO} -spec $BUILDSPEC
