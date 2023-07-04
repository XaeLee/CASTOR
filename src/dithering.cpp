#include "engine.h"
#include "colorMatching.h"

#include <algorithm>
#include <iostream>

QRgb add_rgb(QRgb pix, QRgb error, float fact){
    int red = qRed(pix) + qRed(error) * fact;
    red = red > 255 ? 255 : red;
    int green = qGreen(pix) + qGreen(error) * fact;
    green = green > 255 ? 255 : green;
    int blue = qBlue(pix) + +qBlue(error) * fact;
    blue = blue > 255 ? 255 : blue;
    return QColor(red, green, blue).rgb();
}

QRgb add_to_rgb(QRgb pix, float f){
    int red = qRed(pix) + f;
    red = red > 255 ? 255 : red;
    int green = qGreen(pix) + f;
    green = green > 255 ? 255 : green;
    int blue = qBlue(pix) + f;
    blue = blue > 255 ? 255 : blue;
    return QColor(red, green, blue).rgb();

}

void engine::AdaptToPaletteClosestNeutral(palette p){
    int nbCols = this->original.width();
    int nbRows = this->original.height();
    
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbRows; y++){
            QRgb m = matchBasic(original.pixel(x, y), p);
            this->edited.setPixel(x, y, m);
        }
    }
}

void engine::AdaptToPaletteClosestNoise(palette p){
    int nbCols = this->original.width();
    int nbRows = this->original.height();
    
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbRows; y++){
            float random = rand() % 200 / 10.0 - 10;
            QRgb newpix = matchBasic(original.pixel(x, y), p);
            this->edited.setPixel(x, y, newpix);
        }
    }
}

void engine::AdaptToPaletteClosestFloyd_Steinberg(palette p){
    int nbCols = this->original.width();
    int nbRows = this->original.height();

    this->cpy = this->original;
    
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbRows; y++){
            QRgb oldpix = cpy.pixel(x,y);
            QRgb newpix = matchBasic(cpy.pixel(x, y), p);
            this->edited.setPixel(x, y, newpix);
            QRgb error = RGB_color_distance(oldpix, newpix);
            if (x + 1 < nbCols){
                if (y + 1 < nbRows)
                    cpy.setPixel(x + 1, y + 1, add_rgb(cpy.pixel(x + 1,y + 1), error, 1.0 / 16));
                cpy.setPixel(x + 1, y, add_rgb(cpy.pixel(x + 1,y), error, 7.0 / 16));
            }
            if (y + 1 < nbRows) {
                if (x > 0)
                    cpy.setPixel(x - 1, y + 1, add_rgb(cpy.pixel(x - 1,y + 1), error, 3.0 / 16));
                cpy.setPixel(x, y + 1, add_rgb(cpy.pixel(x,y + 1), error, 5.0 / 16));
            }
        }
    }
    
}