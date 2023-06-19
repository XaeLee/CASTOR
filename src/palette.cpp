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
    this->nbColors = count + 1;
    this->colors = pal;
    return *this;
}

void palette::display(){
    for (int i = 0; i < nbColors; i++){
        cout << this->colors[i] << endl;
    }
}