#include "palette.h"

#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

palette palette::OpenPalette(string filename){
    fstream file(filename, ios::in);
    string line, word;
    vector<QRgb> pal;
    int count = 0;
    if (file.is_open()){
        while( getline(file, line)){

            stringstream str(line);
            //read R value in line
            getline(str, word, ',');
            int r = atoi(word.c_str());
            //read G value in line
            getline(str, word, ',');
            int g = atoi(word.c_str());
            //read B value in line
            getline(str, word, ',');
            int b = atoi(word.c_str());
            QRgb col = QColor(r, g, b).rgb();
            pal.push_back(col);
            count++;
        }
    } else {
        cout << "Could not open file " << filename << endl;
    }
    this->nbColors = count;
    this->colors = pal;
    return *this;
}

void palette::display(){
    for (int i = 0; i < nbColors; i++){
        QRgb curr = this->colors[i];
        cout << qRed(curr) << " " << qGreen(curr) << " " << qBlue(curr) << endl;
    }
}

void palette::addColor(QRgb c){
    this->colors.push_back(c);
    this->nbColors++;
}

vector<QRgb> palette::getColors(){
    return this->colors;
}

int palette::getColorCount(){
    return this->colors.size();
}
palette::palette(){
    this->nbColors = 0;
}

palette::~palette(){
    
}