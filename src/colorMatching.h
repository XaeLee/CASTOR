#pragma once

#include "palette.h"
#include <QtGui/qimage.h>
using namespace mypalette;

QRgb matchBasic(QRgb base, palette p);

int color_distance(QRgb p1, QRgb p2);

QRgb RGB_color_distance(QRgb p1, QRgb p2);

vector<float> error_rgb(QRgb p1, QRgb p2);
