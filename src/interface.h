#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtWidgets/QFileDialog>
#include <QtGui/QImageReader>
#include <QtGui/QImageWriter>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenuBar>
#include <QtGui/QPixmap>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QDockWidget>

#include "engine.h"

class interface : public QMainWindow
{
    Q_OBJECT

private slots:
    //open image - select a file, build engine around it. disaply it in main viewer.
    void open();
    //saves edit under a given filename
    void saveAs();
    //saves the modified or extracted palette as a new .csv file
    void savePalette();
    //fit the image viewer to the window
    void fitToWindow();
    void normalSize();
    void addColor();
    void reloadPalette();
    void resetImage();
    void generatePaletteFromImageMedianCut();
    void generatePaletteFromImageOctree();
    void buildWidgetViewer();

private :
    void createMenus();
    void createActions();
    void updateActions();
    
    bool saveFile(const QString &filename);

    engine eng;
    mypalette::palette p;
    QImage img;

    //edited/preview image
    QLabel *previewImageLabel;

    // main viewing area, main widget
    QScrollArea *scrollAreaEdited;
    //dockable window for palette viewing
    QDockWidget *paletteDock;
    //layout for building & re-building palette display
    QGridLayout *dockLayout;
    //container palette widget : is the main widget of the dock, we create a new dockLayout to assign
    // the container to refresh the palette display
    QScrollArea *container;

    /**
     * Opens a dialog for the user to choose a source palette if none is loaded
     * then launches adaptation using $matchType algorithm
    */
    QPushButton *adaptToPalette;
    QComboBox *matchType;

    QAction *saveAsAct;
    QAction *fitToWindowAct;
    QAction *savePaletteAct;
    QAction *addColorAct;
    QAction *loadReloadPalAct;
    QAction *resetImageAct;
    
    // Miscellaneous menu actions
    QAction *reduceColorsMedianCutAct;
    QAction *reduceColorsOctreeAct;
    double scaleFactor = 1;
public:
    interface(QWidget *parent = nullptr);
    bool loadFile(const QString &filename);
    bool loadPalette(const QString &filename);
};

