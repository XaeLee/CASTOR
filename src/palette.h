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
    vector<QRgb> getColor();
    void display();
    palette OpenPalette(string filename);
    ~palette();
};