#pragma once

#include <string>
#include <vector>
#include <QtGui/qcolor.h>

using namespace std;
namespace mypalette
{
    

class palette
{
private:
    int nbColors = 0;
    vector<QRgb> colors;
public:
    void addColor(QRgb c);
    vector<QRgb> getColors();
    void display();
    palette OpenPalette(string filename);
    int getColorCount();
    palette();
    ~palette();
};
} // namespace mypalette