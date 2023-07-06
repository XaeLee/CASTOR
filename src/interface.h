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

#include "engine.h"

class interface : public QMainWindow
{
    Q_OBJECT

private slots:
    //open image - select a file, build engine around it. disaply it in main viewer.
    //TODO
    void open();
    //saves edit under a given filename
    //TODO
    void saveAs();
    void fitToWindow();
    void normalSize();
    
private :
    void createMenus();
    void createActions();
    void updateActions();
    
    bool saveFile(const QString &filename);

    engine eng;
    QImage img;

    QLabel *editedLabel;
    QScrollArea *scrollAreaEdited;

    /**
     * Launches reduceColors w params from the ComboBox
    */
    QPushButton *reduceColors;
    /**
     * Opens a dialog for the user to choose a source palette
    */
    QPushButton *adaptToPalette;

    QComboBox *algoType;
    QComboBox *matchType;

    QAction *saveAsAct;
    QAction *fitToWindowAct;

    double scaleFactor = 1;
public:
    interface(QWidget *parent = nullptr);
    bool loadFile(const QString &filename);
};

