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

//
void engine::AdaptToPaletteClosestFloyd_Steinberg(palette p){
    int nbCols = this->original.width();
    int nbRows = this->original.height();

    this->cpy = this->original;
    
    float one = 1.0/16;
    float three = 3.0/16;
    float five = 5.0/16;
    float seven = 7.0/16;

    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbRows; y++){
            QRgb oldpix = cpy.pixel(x,y);
            QRgb newpix = matchBasic(cpy.pixel(x, y), p);
            this->edited.setPixel(x, y, newpix);
            QRgb error = RGB_color_distance(oldpix, newpix);
            if (x + 1 < nbCols){
                if (y + 1 < nbRows)
                    cpy.setPixel(x + 1, y + 1, add_rgb(cpy.pixel(x + 1,y + 1), error, one));
                cpy.setPixel(x + 1, y, add_rgb(cpy.pixel(x + 1,y), error, seven));
            }
            if (y + 1 < nbRows) {
                if (x > 0)
                    cpy.setPixel(x - 1, y + 1, add_rgb(cpy.pixel(x - 1,y + 1), error, three));
                cpy.setPixel(x, y + 1, add_rgb(cpy.pixel(x,y + 1), error, five));
            }
        }
    }
    
}

void engine::AdaptToPaletteClosestJarvis(palette p){
    int nbCols = this->original.width();
    int nbRows = this->original.height();

    this->cpy = this->original;
    
    float one = 1.0/48;
    float three = 3.0/48;
    float five = 5.0/48;
    float seven = 7.0/48;
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbRows; y++){
            QRgb oldpix = cpy.pixel(x,y);
            QRgb newpix = matchBasic(cpy.pixel(x, y), p);
            this->edited.setPixel(x, y, newpix);
            QRgb error = RGB_color_distance(oldpix, newpix);
            if (y  + 2 < nbRows){
                if (x - 2 > 0)
                    cpy.setPixel(x - 2, y + 2, add_rgb(cpy.pixel(x - 2,y + 2), error, one));
                if (x - 1 > 0)
                    cpy.setPixel(x - 1, y + 2, add_rgb(cpy.pixel(x - 1,y + 2), error, three));
                cpy.setPixel(x, y + 2, add_rgb(cpy.pixel(x,y + 2), error, five));
                if (x + 1 < nbCols)
                    cpy.setPixel(x + 1, y + 2, add_rgb(cpy.pixel(x + 1,y + 2), error, three));
                if (x + 2 < nbCols)
                    cpy.setPixel(x + 2, y + 2, add_rgb(cpy.pixel(x + 2,y + 2), error, one));
            }
            if (y  + 1 < nbRows){
                if (x - 2 > 0)
                    cpy.setPixel(x - 2, y + 1, add_rgb(cpy.pixel(x - 2,y + 1), error, three));
                if (x - 1 > 0)
                    cpy.setPixel(x - 1, y + 1, add_rgb(cpy.pixel(x - 1,y + 1), error, five));
                cpy.setPixel(x, y + 1, add_rgb(cpy.pixel(x,y + 1), error, seven));
                if (x + 1 < nbCols)
                    cpy.setPixel(x + 1, y + 1, add_rgb(cpy.pixel(x + 1,y + 1), error, five));
                if (x + 2 < nbCols)
                    cpy.setPixel(x + 2, y + 1, add_rgb(cpy.pixel(x + 2,y + 1), error, three));
            }
            if (x + 1 < nbCols)
                cpy.setPixel(x + 1, y, add_rgb(cpy.pixel(x + 1,y), error, seven));
            if (x + 2 < nbCols)
                cpy.setPixel(x + 2, y, add_rgb(cpy.pixel(x + 2,y), error, five));
        }
    }
    
}


void engine::AdaptToPaletteClosestAtkinson(palette p){
    int nbCols = this->original.width();
    int nbRows = this->original.height();

    this->cpy = this->original;
    
    float one = 1.0/8;
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbRows; y++){
            QRgb oldpix = cpy.pixel(x,y);
            QRgb newpix = matchBasic(cpy.pixel(x, y), p);
            this->edited.setPixel(x, y, newpix);
            QRgb error = RGB_color_distance(oldpix, newpix);
            if (y  + 2 < nbRows){
                cpy.setPixel(x, y + 2, add_rgb(cpy.pixel(x,y + 2), error, one));
            }
            if (y  + 1 < nbRows){
                if (x - 1 > 0)
                    cpy.setPixel(x - 1, y + 1, add_rgb(cpy.pixel(x - 1,y + 1), error, one));
                cpy.setPixel(x, y + 1, add_rgb(cpy.pixel(x,y + 1), error, one));
                if (x + 1 < nbCols)
                    cpy.setPixel(x + 1, y + 1, add_rgb(cpy.pixel(x + 1,y + 1), error, one));
            }
            if (x + 1 < nbCols)
                cpy.setPixel(x + 1, y, add_rgb(cpy.pixel(x + 1,y), error, one));
            if (x + 2 < nbCols)
                cpy.setPixel(x + 2, y, add_rgb(cpy.pixel(x + 2,y), error, one));
        }
    }
    
}

void engine::AdaptToPaletteClosestSierra(palette p){
    int nbCols = this->original.width();
    int nbRows = this->original.height();

    this->cpy = this->original;
    
    float two = 2.0/32;
    float three = 3.0/32;
    float five = 5.0/32;
    float four = 4.0/32;
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbRows; y++){
            QRgb oldpix = cpy.pixel(x,y);
            QRgb newpix = matchBasic(cpy.pixel(x, y), p);
            this->edited.setPixel(x, y, newpix);
            QRgb error = RGB_color_distance(oldpix, newpix);
            if (y  + 2 < nbRows){
                if (x - 1 > 0)
                    cpy.setPixel(x - 1, y + 2, add_rgb(cpy.pixel(x - 1,y + 2), error, two));
                cpy.setPixel(x, y + 2, add_rgb(cpy.pixel(x,y + 2), error, three));
                if (x + 1 < nbCols)
                    cpy.setPixel(x + 1, y + 2, add_rgb(cpy.pixel(x + 1,y + 2), error, two));
            }
            if (y  + 1 < nbRows){
                if (x - 2 > 0)
                    cpy.setPixel(x - 2, y + 1, add_rgb(cpy.pixel(x - 2,y + 1), error, two));
                if (x - 1 > 0)
                    cpy.setPixel(x - 1, y + 1, add_rgb(cpy.pixel(x - 1,y + 1), error, four));
                cpy.setPixel(x, y + 1, add_rgb(cpy.pixel(x,y + 1), error, five));
                if (x + 1 < nbCols)
                    cpy.setPixel(x + 1, y + 1, add_rgb(cpy.pixel(x + 1,y + 1), error, four));
                if (x + 2 < nbCols)
                    cpy.setPixel(x + 2, y + 1, add_rgb(cpy.pixel(x + 2,y + 1), error, two));
            }
            if (x + 1 < nbCols)
                cpy.setPixel(x + 1, y, add_rgb(cpy.pixel(x + 1,y), error, five));
            if (x + 2 < nbCols)
                cpy.setPixel(x + 2, y, add_rgb(cpy.pixel(x + 2,y), error, three));
        }
    }
}

void engine::AdaptToPaletteClosestSierraTwoRows(palette p){
    int nbCols = this->original.width();
    int nbRows = this->original.height();

    this->cpy = this->original;
    
    float two = 2.0/16;
    float three = 3.0/16;
    float one = 1.0/16;
    float four = 4.0/16;
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbRows; y++){
            QRgb oldpix = cpy.pixel(x,y);
            QRgb newpix = matchBasic(cpy.pixel(x, y), p);
            this->edited.setPixel(x, y, newpix);
            QRgb error = RGB_color_distance(oldpix, newpix);
            if (y  + 1 < nbRows){
                if (x - 2 > 0)
                    cpy.setPixel(x - 2, y + 1, add_rgb(cpy.pixel(x - 2,y + 1), error, one));
                if (x - 1 > 0)
                    cpy.setPixel(x - 1, y + 1, add_rgb(cpy.pixel(x - 1,y + 1), error, two));
                cpy.setPixel(x, y + 1, add_rgb(cpy.pixel(x,y + 1), error, three));
                if (x + 1 < nbCols)
                    cpy.setPixel(x + 1, y + 1, add_rgb(cpy.pixel(x + 1,y + 1), error, two));
                if (x + 2 < nbCols)
                    cpy.setPixel(x + 2, y + 1, add_rgb(cpy.pixel(x + 2,y + 1), error, one));
            }
            if (x + 1 < nbCols)
                cpy.setPixel(x + 1, y, add_rgb(cpy.pixel(x + 1,y), error, four));
            if (x + 2 < nbCols)
                cpy.setPixel(x + 2, y, add_rgb(cpy.pixel(x + 2,y), error, three));
        }
    }

}

void engine::AdaptToPaletteClosestSierraLite(palette p){
    int nbCols = this->original.width();
    int nbRows = this->original.height();

    this->cpy = this->original;
    
    float two = 2.0/4;
    float one = 1.0/4;
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbRows; y++){
            QRgb oldpix = cpy.pixel(x,y);
            QRgb newpix = matchBasic(cpy.pixel(x, y), p);
            this->edited.setPixel(x, y, newpix);
            QRgb error = RGB_color_distance(oldpix, newpix);
            if (y  + 1 < nbRows){
                if (x - 1 > 0)
                    cpy.setPixel(x - 1, y + 1, add_rgb(cpy.pixel(x - 1,y + 1), error, one));
                cpy.setPixel(x, y + 1, add_rgb(cpy.pixel(x,y + 1), error, one));
            }
            if (x + 1 < nbCols)
                cpy.setPixel(x + 1, y, add_rgb(cpy.pixel(x + 1,y), error, two));
        }
    }
}