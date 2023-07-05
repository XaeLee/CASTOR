#include "octree.h"

#include <iostream>

ocnode::ocnode(){
    for (int i = 0; i < 8; i++){
        this->children[i] = NULL;
    }
}

ocnode::~ocnode(){

}

octree::octree(int n){
    root = new ocnode();
    wantedColors = n;
}

octree::~octree(){

}

/**
 * Get node that is passed by the least, meaning the color the least used in the image
*/
void octree::getMinRefCount(ocnode *n){
    if (n->passes < MinReferenceCount)
        MinReferenceCount = n->passes;
    if (n->childrenCount == 0)
        return;
    for (int i = 0; i < 8; i++){
        if (n->children[i])
            getMinRefCount(n->children[i]);
    }
}

/**
 * Merge colors from children into the parent (thats the part where we actually reduce the colors)
 * Merging goes on the least used node
 * We are then looking for the node with mincount passes
*/
void octree::mergeLeast(ocnode *n, int mincount){
    if (n->passes > mincount || currcolorsCount == wantedColors)
        return;
    
    for (int i = 0; i < 8; i++){
        // we go over the 9 children
        if(n->children[i] && (n->children[i]->childrenCount > 0))
            //if the given node has children, its not a leaf, and only leaves represent colors. We go deeper.
            mergeLeast(n->children[i], mincount);
        else {
            //else, we merge - or stop if we reached the nb of wanted colors.
            if (currcolorsCount == wantedColors)
                break;
            if (n->children[i]){
                n->pixel_count += n->children[i]->pixel_count;
                n->r_sum += n->children[i]->r_sum;
                n->g_sum += n->children[i]->g_sum;
                n->b_sum += n->children[i]->b_sum;

                delete n->children[i];

                n->children[i] = NULL;
                n->childrenCount--;
                currcolorsCount--; //we removed a color from the tree


            }
        }
    }
    if (n->childrenCount == 0)
        currcolorsCount++; // we removed all 8 children color, but parent became a leaf and thus a color
}

void octree::writeColor(ocnode *n){
    float r = n->r_sum / n->pixel_count;
    float g = n->g_sum / n->pixel_count;
    float b = n->b_sum / n->pixel_count;
    if (r + g + b > 0)
        p.addColor(QColor(r, g, b).rgb());
}

void octree::FillPalette(ocnode *n){
    if (n->childrenCount == 0){
        writeColor(n);
        return;
    }
    for (int i = 0; i < 8; i++){
        if (n->children[i])
            FillPalette(n->children[i]);
    }
}

void octree::setColor(ocnode *n, QRgb color){
    currcolorsCount += 1&(!n->passes);

    n->passes++;
    n->pixel_count++;

    n->r_sum += qRed(color);
    n->g_sum += qGreen(color); 
    n->b_sum += qBlue(color);
}

/**
 * Where the magic happens! We generate an index from a RGB triplet, using bitshifting
 * The form is 0000RGB (r <<2, g << 1, b << 0).
 * |= ça s'appelle un après où binaire, c'est un +=, ex : a = 5 (b101), c = 3 (b010), a|= c (b111).
 * ouais je commente en français psk j'ai un peu la flemme de cherche comment on dit après où binaire en anglais
*/
int getIndex(int r, int g, int b, int level){
    int index=0;
    int bit = 1<<level;
    if((r&bit) == bit)
        index |= (1<<2);
    if((g&bit) == bit)
        index |= (1<<1);
    if((b&bit) == bit)
        index |= (1<<0);
    return index;
}

void octree::addColor(ocnode *n, QRgb color, int level){
    if (level < maxDepth){
        //if we're on a parent, build index from color, and create node if needed.
        // then, addColor deeper.
        int index = getIndex(qRed(color), qGreen(color), qBlue(color), level);
        //std::cout << "index is :" << to_string(index) << std::endl;
        n->passes++;
        if (!n->children[index]){
            n->children[index] = new ocnode();
            n->childrenCount++;
        }
        addColor(n->children[index], color, level + 1);
    } else {
        // if we're on a leaf, we set its color to color.
        setColor(n, color);
    }
}

palette octree::reduceColors(QImage img){
    int nbCols = img.width();
    int nbRows = img.height();
    
    // we build the tree
    for (int x = 0; x < nbCols; x++){
        for (int y = 0; y < nbRows; y++){
            // we add every color met to the tree
            QRgb color = img.pixel(x, y);
            addColor(root, color, 0);
        }
    }
    cout << "done building the tree" << endl;
    // we reduce the colors, starting with the least used ones
    this->MinReferenceCount = INT_MAX;
    getMinRefCount(root);
    int leastUsed = MinReferenceCount;
    while((currcolorsCount > wantedColors)){
        mergeLeast(root, leastUsed);
        leastUsed += MinReferenceCount;
    }

    cout << "done reducing to "<< to_string(wantedColors) << " colors" << endl;
    // we build the palette from the reduced tree
    FillPalette(root);

    cout << "filled palette" << endl;
    
    return p;
}