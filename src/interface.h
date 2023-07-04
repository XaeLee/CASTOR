#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>

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
private :
    void createMenus();
    void createActions();
    void updateActions();
    
    bool loadFile(const QString filename);
    bool saveFile(const QString &filename);

    engine *eng;

    QWidget *mainWindow;
    QLabel *sourceLabel;
    QLabel *editedLabel;
    QScrollArea *scrollAreaSource;
    QScrollArea *scrollAreaEdited;


    QVBoxLayout *images;

    interface *ui;

    QAction *saveAsAct;

public:
    interface(QWidget *parent = nullptr);
};

