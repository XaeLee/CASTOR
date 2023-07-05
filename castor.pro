HEADERS += src/engine.h
HEADERS += src/palette.h
HEADERS += src/colorMatching.h
HEADERS += src/octree.h

SOURCES += src/engine.cpp
SOURCES += src/mediancut.cpp
SOURCES += src/palette.cpp
SOURCES += src/colorMatching.cpp
SOURCES += src/main.cpp
SOURCES += src/dithering.cpp
SOURCES += src/octree.cpp

TARGET = castor

CONFIG += debug