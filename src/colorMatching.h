#pragma once

#include "palette.h"
#include <qt5/QtGui/qimage.h>

QRgb matchBasic(QRgb base, palette p);
int color_distance(QRgb p1, QRgb p2);
QRgb RGB_color_distance(QRgb p1, QRgb p2);