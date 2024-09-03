#include "palette.h"

#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
using namespace mypalette;

/**
 * Opens and loads a palette from a csv path/filename.
 * @param filename The path/filename of the csv file
 * @return The opened palette as a mypalette::palette object
 */
palette palette::OpenPalette(string filename)
{
    fstream file(filename, ios::in);
    string line, word;
    vector<QRgb> pal;
    int count = 0;
    if (file.is_open())
    {
        while (getline(file, line))
        {

            stringstream str(line);
            // read R value in line
            getline(str, word, ',');
            int r = atoi(word.c_str());
            // read G value in line
            getline(str, word, ',');
            int g = atoi(word.c_str());
            // read B value in line
            getline(str, word, ',');
            int b = atoi(word.c_str());
            QRgb col = QColor(r, g, b).rgb();
            pal.push_back(col);
            count++;
        }
    }
    else
    {
        cout << "Could not open file " << filename << endl;
    }
    this->nbColors = count;
    this->colors = pal;
    return *this;
}

/**
 * Simple print function to display and check values from command line/debug
 */
void palette::display()
{
    for (int i = 0; i < nbColors; i++)
    {
        QRgb curr = this->colors[i];
        cout << qRed(curr) << " " << qGreen(curr) << " " << qBlue(curr) << endl;
    }
}

/**
 * Adds the given color to the palette, increases its color count.
 * @param c The color to add to the palette as QRgb
 */
void palette::addColor(QRgb c)
{
    this->colors.push_back(c);
    this->nbColors++;
}

vector<QRgb> palette::getColors()
{
    return this->colors;
}

int palette::getColorCount()
{
    return this->colors.size();
}

/**
 * Saves the palette under filename, with correct csv formatting.
 * @param filename The path/name under which the file must be saved
 */
void palette::savePalette(string filename)
{
    std::ofstream file;
    file.open(filename);
    if (file.is_open()){
    for (QRgb c : colors)
    {
        file << to_string(qRed(c)) + ", " + to_string(qGreen(c)) + ", " + to_string(qBlue(c)) + "\n";
    }
    file.close();
    } else {
        std::cout << "could not open file"<<endl;
    }
}

palette::palette()
{
    this->nbColors = 0;
}

palette::~palette()
{
}