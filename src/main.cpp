#include "palette.h"
#include "engine.h"
#include "interface.h"

#include <QtWidgets/QApplication>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
    QApplication app(argc, argv);
    interface inter;
    inter.show();

    return app.exec();
}