#include "colorMatching.h"

#include <cmath>

QRgb matchBasic(QRgb base, palette p)
{
    int index = 0;
    int current_distance = INT_MAX;
    vector<QRgb> cols = p.getColors();
    for (int i = 0; i < cols.size(); i++)
    {
        int dist = color_distance(base, cols.at(i));
        if (dist < current_distance)
        {
            current_distance = dist;
            index = i;
        }
    }
    return cols[index];
}

int color_distance(QRgb p1, QRgb p2)
{
    int r1 = qRed(p1);
    int g1 = qGreen(p1);
    int b1 = qBlue(p1);

    int r2 = qRed(p2);
    int g2 = qGreen(p2);
    int b2 = qBlue(p2);

    return abs(r1 - r2) + abs(g1 - g2) + abs(b1 - b2);
}

QRgb RGB_color_distance(QRgb p1, QRgb p2)
{
    int r1 = qRed(p1);
    int g1 = qGreen(p1);
    int b1 = qBlue(p1);

    int r2 = qRed(p2);
    int g2 = qGreen(p2);
    int b2 = qBlue(p2);

    return QColor(abs(r1 - r2), abs(g1 - g2), abs(b1 - b2)).rgb();
}

vector<float> error_rgb(QRgb p1, QRgb p2)
{
    float r1 = qRed(p1);
    float g1 = qGreen(p1);
    float b1 = qBlue(p1);

    float r2 = qRed(p2);
    float g2 = qGreen(p2);
    float b2 = qBlue(p2);

    vector<float> res;
    res.push_back(r1 - r2);
    res.push_back(g1 - g2);
    res.push_back(b1 - b2);

    return res;
}