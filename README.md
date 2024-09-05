![castorBanner](https://github.com/user-attachments/assets/63caaca1-7d21-4315-bdcc-a5cea601e371)
# BRIEF

The goal of this project is to implement a C++ solution allowing anyone to change the color palette of an image easily.

There are many ways and websites allowing anyone to extract colors from a given image, but almost none go further than this and allow users to apply a selected color palette to an image. Our goal is to try and fill that void, to allow any Linux enthusiast to easily match their favorite background illustration to their favorite terminal/OS theme. 


![castorGUIAnimation](https://github.com/user-attachments/assets/a0e53c45-acd0-45fe-ba3b-3bcd4b2a0db0)

# Launching CASTOR

In the basic directory, run :
```qmake -o Makefile castor.pro```
This will generate the up-to-date Makefile for the project.
You can then just proceed to :
```make```
And then launch the GUI with :
```./castor```

# Using CASTOR
You can load your palette from the color menu or by pressing the "adapt to palette button" for the first time - please note that the expected file format is a csv containing one "R, G, B" color per line, where r g and b range from 0 to 255.

You can edit your palette directly from the interface, by adding colors and saving the modified version.

Once you found an interesting result image, you can save it from the File menu.


# NOTES

Originally a school project, CASTOR is now maintened as a personal tool mainly focused on color adjustment features - there are already many color extraction and/or color picker tools available for free online, and the 2 color reduction algorithms still available under the "Miscellaneous" menu were implemented for fun and to complete the school project only. They can be used, but are slow and suboptimized, and will likely not be updated.