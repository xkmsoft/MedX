//
// Created by chasank on 02/10/2021.
//

#include <QtWidgets>
#include "MedXMainWindow.h"
#include <Threads/MedicalImageReaderThread.h>
#include <iostream>

using namespace std;

MedXMainWindow::MedXMainWindow(QWidget *parent) {
    centralWidget = new CentralWidget(this);
    setCentralWidget(centralWidget);

    imageReaderThread = new MedicalImageReaderThread(this);
    connect(imageReaderThread, &MedicalImageReaderThread::readFinished, this, &MedXMainWindow::readFinished);

    this->initUI();
}

MedXMainWindow::~MedXMainWindow() {
    delete this->fileMenu;
    delete this->aboutMenu;
    delete this->openNiftiAction;
    delete this->openDICOMAction;
    delete this->quitAction;
    delete this->aboutAction;
}

void MedXMainWindow::initUI() {
    this->setWindowTitle("MedX");
    this->createActions();
    this->createMenus();
    this->createStatusBar();
}

void MedXMainWindow::createMenus() {
    // File Menu
    this->fileMenu = this->menuBar()->addMenu(tr("&File"));
    this->fileMenu->addAction(this->openNiftiAction);
    this->fileMenu->addAction(this->openDICOMAction);
    this->fileMenu->addAction(this->quitAction);
    // About Menu
    this->aboutMenu = this->menuBar()->addMenu(tr("&Help"));
    this->aboutMenu->addAction(this->aboutAction);
}

void MedXMainWindow::createActions() {
    // Open Nifti file Action
    this->openNiftiAction = new QAction(tr("&Open Nifti"), this);
    this->openNiftiAction->setShortcut(QKeySequence::Open);
    connect(this->openNiftiAction, &QAction::triggered, this, &MedXMainWindow::openNiftiFile);

    // Open DICOM directory action
    this->openDICOMAction = new QAction(tr("&Open DICOM directory"), this);
    connect(this->openDICOMAction, &QAction::triggered, this, &MedXMainWindow::openDICOMDirectory);

    // About Action
    aboutAction = new QAction(tr("&About"), this);
    connect(this->aboutAction, &QAction::triggered, this, &MedXMainWindow::about);

    // Quit Action
    quitAction = new QAction(tr("&Quit"), this);
    connect(this->quitAction, &QAction::triggered, this, &MedXMainWindow::quit);
}

void MedXMainWindow::createStatusBar() {
    this->statusBar()->showMessage(tr("Ready"));
}

void MedXMainWindow::openNiftiFile() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()) {
        QStringList selectedFiles = dialog.selectedFiles();
        QString file = selectedFiles.first();
        if (!imageReaderThread->isRunning()) {
            imageReaderThread->begin(ImageType::NIFTI, file.toStdString());
        }
    }
}

void MedXMainWindow::openDICOMDirectory() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()) {
        QStringList selectedDirectories = dialog.selectedFiles();
        QString directory = selectedDirectories.first();
        if (!imageReaderThread->isRunning()) {
            imageReaderThread->begin(ImageType::DICOM, directory.toStdString());
        }
    }
}

void MedXMainWindow::about() {
    QMessageBox::about(this, tr("About MedX"),
                       tr("The <b>MedX</b> visualizes NIFTI and DICOM files in cross-sectional planes and 3D volume rendering"));
}

void MedXMainWindow::quit() {
    this->close();
}

void MedXMainWindow::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);
}

void MedXMainWindow::readFinished(vtkImageData *imageData) {
    centralWidget->getVolumeRenderingWidget()->updatePipeline(imageData);
    centralWidget->getSagittalWidget()->updatePipeline(imageData);
    centralWidget->getCoronalWidget()->updatePipeline(imageData);
    centralWidget->getAxialWidget()->updatePipeline(imageData);
}
