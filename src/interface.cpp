#include "interface.h"

#include <iostream>
#include <QtWidgets/QColorDialog>

/**
 * Int to color reduction algorithm type enumeration matching
 * @param i An int
 * @return The corresponding enumeration identifier
 */
int boxToAlgoType(int i)
{
    switch (i)
    {
    case 0:
        return MEDIAN_CUT;
    case 1:
        return OCTREE;
    default:
        break;
        return -1;
    }
}

/**
 * Int to matching algorithm type enumeration matching
 * @param i An int
 * @return The corresponding enumeration identifier
 */
int boxToMatchType(int i)
{
    switch (i)
    {
    case 0:
        return BASIC;
    case 1:
        return DI_FLOYD_STEINBERG;
    case 2:
        return DI_NOISE;
    case 3:
        return DI_JARVISJN;
    case 4:
        return DI_ATKINSON;
    case 5:
        return DI_SIERRA;
    case 6:
        return DI_SIERRA_TWO_ROW;
    case 7:
        return DI_SIERRA_LITE;
    case 8:
        return DI_BAYER_4X4;
    case 9:
        return DI_BAYER_8x8;
    default:
        break;
        return -1;
    }
}

/**
 * Interaction initializer - sets accepted file types for palettes as csv.
 */
static void initializePaletteFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    dialog.setDirectory(QDir::currentPath());

    dialog.setAcceptMode(acceptMode);
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("csv");
}

/**
 * Builds or rebuilds the dockable palette viewer.
 * Create a new layout, and add it a colored button for each color in the palette.
 * Create a new container widget, assign it the layout.
 * Set the container as widget of the dockable palette viewer.
 */
void interface::buildWidgetViewer(){
    dockLayout = new QGridLayout();
    dockLayout->setSpacing(2);
    for (QRgb col : p.getColors()){
        QPushButton *colorButton = new QPushButton();
        char cb[100];
        sprintf(cb, "background-color: rgb(%u, %u, %u)", qRed(col), qGreen(col), qBlue(col));
        std::string str(cb);
        QString color_string = QString::fromStdString(str); 
        colorButton->setStyleSheet(color_string);
        dockLayout->addWidget(colorButton);
    }
    container = new QScrollArea();
    container->setLayout(dockLayout);
    paletteDock->setWidget(container);
    
}

/**
 * Main window definition
 */
interface::interface(QWidget *parent) : QMainWindow(parent)
{
    scrollAreaEdited = new QScrollArea();
    previewImageLabel = new QLabel();

    // PALETTE VIEWER
    paletteDock = new QDockWidget(tr("Opened Palette"), parent);
    buildWidgetViewer();

    paletteDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, paletteDock);

    

    // ADAPT BUTTON
    adaptToPalette = new QPushButton("&Adapt To Palette", previewImageLabel);
    adaptToPalette->move(250, 15);
    adaptToPalette->show();
    QObject::connect(adaptToPalette, &QPushButton::clicked, [this]()
                     {
                         int mt = boxToMatchType(matchType->currentIndex());
                         if (p.getColorCount() == 0)
                         {
                             QFileDialog dialog(this, tr("Choose CSV Palette"));
                             initializePaletteFileDialog(dialog, QFileDialog::AcceptOpen);
                             while (dialog.exec() == QDialog::Accepted && !loadPalette(dialog.selectedFiles().constFirst()))
                             {

                             }
                            this->buildWidgetViewer();
                         }
                         eng.AdaptToPaletteClosest(p, mt);
                         this->previewImageLabel->setPixmap(QPixmap::fromImage(eng.edited)); });

    // MATCHING ALGORITHM SELECTOR
    matchType = new QComboBox(previewImageLabel);
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
    matchType->move(15, 15);

    // ADJUSTMENTS
    previewImageLabel->setBackgroundRole(QPalette::Base);
    previewImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    previewImageLabel->setScaledContents(true);

    scrollAreaEdited->setBackgroundRole(QPalette::Dark);
    scrollAreaEdited->setWidget(previewImageLabel);
    setCentralWidget(scrollAreaEdited);

    createActions();
    resize(QGuiApplication::primaryScreen()->availableSize());
}

/**
 * Interaction initializer for file dialog/selection - sets the supported image types to jpeg and jpg
 */
static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{

    dialog.setDirectory(QDir::currentPath());

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
                                                  ? QImageReader::supportedMimeTypes()
                                                  : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    dialog.setAcceptMode(acceptMode);
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

/**
 * An action-linked function that wraps the engine's reduce color functions
 */
void interface::generatePaletteFromImageMedianCut(){
    int at = MEDIAN_CUT;
    bool ok;
    int nbColors = QInputDialog::getInt(this, tr("Please provide information"), tr("Number of Colors Wanted:"), 6, 2, INT_MAX, 1, &ok);
    p = eng.ExtractPalette(nbColors, at);
    buildWidgetViewer();
}

/**
 * An action-linked function that wraps the engine's reduce color functions
 */
void interface::generatePaletteFromImageOctree(){
    int at = OCTREE;
    bool ok;
    int nbColors = QInputDialog::getInt(this, tr("Please provide information"), tr("Number of Colors Wanted:"), 6, 2, INT_MAX, 1, &ok);
    p = eng.ExtractPalette(nbColors, at);
    buildWidgetViewer();
}

/**
 * An action-linked function that loads a palette, and updates the palette viewer.
 */
bool interface::loadPalette(const QString &filename)
{
    p.OpenPalette(filename.toStdString());
    buildWidgetViewer();
    return true;
}

/**
 * An action-linked function that loads and displays a new image.
 */
bool interface::loadFile(const QString &filename)
{
    QImageReader reader(filename);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull())
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                     .arg(QDir::toNativeSeparators(filename), "Could not open this file."));
        return false;
    }

    scaleFactor = 1;

    img = newImage;
    eng.openImage(img);
    fitToWindowAct->setEnabled(true);
    previewImageLabel->setPixmap(QPixmap::fromImage(newImage));
    updateActions();
    if (!fitToWindowAct->isChecked())
        previewImageLabel->adjustSize();
    return true;
}

/**
 * An action-linked function to open a file
 */
void interface::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst()))
    {
    }
}

/**
 * An action-linked function that launched a file dialog to save the edited image.
 */
void interface::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save edited image", ""));
    if (fileName.isEmpty())
        return;
    else
    {
        eng.saveEdit(fileName.toStdString());
    }
}

/**
 * An action-linked function that launched a file dialog to save the opened,
 * generated or modified palette.
 */
void interface::savePalette()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Palette As...", ""));
    if (fileName.isEmpty())
        return;
    else
    {
        p = eng.getPalette();
        p.savePalette(fileName.toStdString());
    }
}

/**
 * An action-linked function that opens a file dialog to pick a new palette to load.
 */
void interface::reloadPalette()
{
    int mt = boxToMatchType(matchType->currentIndex());
    QFileDialog dialog(this, tr("Choose CSV Palette"));
    initializePaletteFileDialog(dialog, QFileDialog::AcceptOpen);
    while (dialog.exec() == QDialog::Accepted && !loadPalette(dialog.selectedFiles().constFirst()))
    {
    }
    buildWidgetViewer();
    eng.AdaptToPaletteClosest(p, mt);
    this->previewImageLabel->setPixmap(QPixmap::fromImage(eng.edited));
}

/**
 * Action and menu managing function
 */
void interface::createActions()
{
    // File Menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open Image..."), this, &interface::open);
    openAct->setShortcut(QKeySequence::Open);

    saveAsAct = fileMenu->addAction(tr("Save Image As..."), this, &interface::saveAs);
    savePaletteAct = fileMenu->addAction(tr("Save Palette As..."), this, &interface::savePalette);

    QAction *exitAct = fileMenu->addAction(tr("&Exit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));


    // Color Menu
    QMenu *PaletteMenu = menuBar()->addMenu(tr("&Palette"));

    loadReloadPalAct = PaletteMenu->addAction(tr("Load New Palette..."), this, &interface::reloadPalette);
    addColorAct = PaletteMenu->addAction(tr("Add Color to palette..."), this, &interface::addColor);

    // View Menu
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &interface::fitToWindow);
    fitToWindowAct->setEnabled(true);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    
    resetImageAct = viewMenu->addAction(tr("Reset image preview"), this, &interface::resetImage);
    resetImageAct->setShortcut(tr("Ctrl+R"));

    QMenu *miscMenu = menuBar()->addMenu(tr("&Misc."));

    reduceColorsMedianCutAct = miscMenu->addAction(tr("Generate Palette - Median Cut"), this, &interface::generatePaletteFromImageMedianCut);
    reduceColorsOctreeAct = miscMenu->addAction(tr("Generate Palette - Octree"), this, &interface::generatePaletteFromImageOctree);
}

void interface::updateActions()
{
}

/**
 * An action-linked function to reset the preview of the edited image to the original image.
 */
void interface::resetImage()
{
    previewImageLabel->setPixmap(QPixmap::fromImage(img));
}

void interface::normalSize()
{
    previewImageLabel->adjustSize();
    scaleFactor = 10.0;
}

void interface::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollAreaEdited->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}

/**
 * An action-linked function that opens a color dialog, allowing the user to add a color to the palette.
 */
void interface::addColor()
{
    // OPEN COLOR PICKER
    QColor color = QColorDialog::getColor();
    // BUILD QRGB FROM ITS DATA
    p.addColor(color.rgb());
    // ADD COLOR TO P
    buildWidgetViewer();
}