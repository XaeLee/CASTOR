#include "interface.h"

#include <iostream>

int boxToAlgoType(int i){
    switch (i)
    {
    case 0 :
        return MEDIAN_CUT;
    case 1:
        return OCTREE;
    default:
        break;
        return -1;
    } 
}

int boxToMatchType(int i){
    switch (i)
    {
    case 0 :
        return BASIC;
    case 1 :
        return DI_FLOYD_STEINBERG;
    case 2 :
        return DI_NOISE;
    case 3 :
        return DI_JARVISJN;
    case 4 :
        return DI_ATKINSON;
    case 5 :
        return DI_SIERRA;
    case 6 :
        return DI_SIERRA_TWO_ROW;
    case 7 :
        return DI_SIERRA_LITE;
    case 8 :
        return DI_BAYER_4X4;
    case 9 :
        return DI_BAYER_8x8;
    default:
        break;
        return -1;
    } 
}

interface::interface(QWidget *parent) : QMainWindow(parent)
{
    scrollAreaEdited = new QScrollArea();
    editedLabel = new QLabel();
    
    reduceColors = new QPushButton("&Reduce Colors", editedLabel);
    reduceColors->move(15, 600);
    reduceColors->show();
    QObject::connect(reduceColors, &QPushButton::clicked, [this](){
        int at = boxToAlgoType(algoType->currentIndex());
        int mt = boxToMatchType(matchType->currentIndex());
        bool ok;
        int nbColors = QInputDialog::getInt(this, tr("Please provide information"), tr("Number of Colors Wanted:"), 6, 2, INT_MAX, 1, &ok);
        eng.ReduceColors(nbColors, at, mt);
        this->editedLabel->setPixmap(QPixmap::fromImage(eng.edited));
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
    matchType->addItem("Bayer 4x4 Ordered Dithering");
    matchType->addItem("Bayer 8x8 Ordered Dithering");
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