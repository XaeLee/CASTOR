#include <string>

using namespace std;
class image
{
private:
    void toPalette();
public:
    image(string filename);
    ~image();
};