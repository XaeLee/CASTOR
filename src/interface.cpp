#include "interface.h"

#include <iostream>

int boxToAlgoType(int i){
    switch (i)
    {
    case 0 :
        std::cout << "median cut" << endl;
        return MEDIAN_CUT;
    default:
        break;
        return -1;
    } 
}

int boxToMatchType(int i){
    switch (i)
    {
    case 0 :
        std::cout << "basic" << endl;
        return BASIC;
    case 1 :
        std::cout << "floyd" << endl;
        return DI_FLOYD_STEINBERG;
    case 2 :
        std::cout << "noise" << endl;
        return DI_NOISE;
    default:
        break;
        return -1;
    } 
}

interface::interface(QWidget *parent) : QMainWindow(parent)
{
    scrollAreaEdited = new QScrollArea();
    editedLabel = new QLabel();
    
    apply = new QPushButton("&Apply", editedLabel);
    apply->move(15, 600);
    apply->show();
    QObject::connect(apply, &QPushButton::clicked, [this](){
        int at = boxToAlgoType(algoType->currentIndex());
        int mt = boxToMatchType(matchType->currentIndex());
    });

    algoType = new QComboBox(editedLabel);
    algoType->addItem("Median cut");
    algoType->addItem("Octree");
    algoType->move(15, 500);
    
    matchType = new QComboBox(editedLabel);
    matchType->addItem("Closest - RGB");
    matchType->addItem("Floyd Steinberg Dithering");
    matchType->addItem("Noise Dithering");
    matchType->addItem("Jarvis J.N. Dithering");
    matchType->addItem("Atkinson Dithering");
    matchType->addItem("Sierra Dithering");
    matchType->addItem("Sierra Two Rows Dithering");
    matchType->addItem("Sierra Lite Dithering");
    matchType->addItem("Bayer Ordered Dithering");
    matchType->move(15, 550);

    editedLabel->setBackgroundRole(QPalette::Base);
    editedLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    editedLabel->setScaledContents(true);

    scrollAreaEdited->setBackgroundRole(QPalette::Dark);
    scrollAreaEdited->setWidget(editedLabel);
    scrollAreaEdited->setVisible(false);
    setCentralWidget(scrollAreaEdited);


    createActions();
    resize(QGuiApplication::primaryScreen()->availableSize());
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    dialog.setAcceptMode(acceptMode);
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

bool interface::loadFile(const QString &filename){
    QImageReader reader(filename);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(filename), "Could not open this file."));
        return false;
    }

    scaleFactor = 1;

    img = newImage;
    eng.openImage(img);
    scrollAreaEdited->setVisible(true);
    fitToWindowAct->setEnabled(true);
    editedLabel->setPixmap(QPixmap::fromImage(newImage));
    updateActions();
    if (!fitToWindowAct->isChecked())
        editedLabel->adjustSize();
    return true;
}

void interface::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}
}

void interface::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save edited image", ""));
    if (fileName.isEmpty())
        return;
    else {
        eng.saveEdit(fileName.toStdString());
    }
}

void interface::createActions(){
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &interface::open);
    openAct->setShortcut(QKeySequence::Open);

    saveAsAct = fileMenu->addAction(tr("Save As..."), this, &interface::saveAs);
    
    QAction *exitAct = fileMenu->addAction(tr("&Exit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &interface::fitToWindow);
    fitToWindowAct->setEnabled(true);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));

}

void interface::updateActions(){
}

void interface::normalSize(){
    editedLabel->adjustSize();
    scaleFactor = 10.0;
}

void interface::fitToWindow(){
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollAreaEdited->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}