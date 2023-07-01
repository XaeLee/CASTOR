#pragma once

#include <string>
#include <vector>
#include <qt5/QtGui/qcolor.h>

using namespace std;
class palette
{
private:
    int nbColors;
    vector<QRgb> colors;
public:
    void addColor(QRgb c);
    vector<QRgb> getColors();
    void display();
    palette OpenPalette(string filename);
    int getColorCount();
    ~palette();
};