#include "interface.h"

#include <QtWidgets/QFileDialog>
#include <QtGui/QImageReader>
#include <QtGui/QImageWriter>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenuBar>

interface::interface(QWidget *parent) : QMainWindow(parent)
{
    mainWindow = new QWidget();
    scrollAreaSource = new QScrollArea();
    scrollAreaEdited = new QScrollArea();
    sourceLabel = new QLabel();
    editedLabel = new QLabel();
    images = new QVBoxLayout(mainWindow);

    sourceLabel->setBackgroundRole(QPalette::Base);
    sourceLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    sourceLabel->setScaledContents(true);

    editedLabel->setBackgroundRole(QPalette::Base);
    editedLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    editedLabel->setScaledContents(true);

    scrollAreaSource->setBackgroundRole(QPalette::Dark);
    scrollAreaSource->setWidget(sourceLabel);
    
    scrollAreaEdited->setBackgroundRole(QPalette::Dark);
    scrollAreaEdited->setWidget(sourceLabel);

    images->addWidget(scrollAreaSource);
    images->addWidget(scrollAreaEdited);

    createActions();
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
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

bool interface::loadFile(const QString filename){
    QImageReader reader(filename);
    reader.setAutoTransform(true);
    QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(filename), reader.errorString()));
        return false;
    }

    engine test = engine(newImage);
    eng = &test;
    sourceLabel->setPixmap(QPixmap::fromImage(eng->original));
    sourceLabel->show();
    updateActions();
}

void interface::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() != QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}
}

void interface::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save edited image", ""));
    if (fileName.isEmpty())
        return;
    else {
        eng->saveEdit(fileName.toStdString());
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


}

void interface::updateActions(){
}