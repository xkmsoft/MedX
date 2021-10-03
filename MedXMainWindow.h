//
// Created by chasank on 02/10/2021.
//

#ifndef MEDX_MEDXMAINWINDOW_H
#define MEDX_MEDXMAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QAction>
#include <QMenu>
#include "QWidgets/CentralWidget.h"
#include "Threads/MedicalImageReaderThread.h"
#include <vtkImageData.h>


class MedXMainWindow: public QMainWindow {
    Q_OBJECT
public:
    explicit MedXMainWindow(QWidget *parent= nullptr);
    ~MedXMainWindow() override;
    void initUI();
public slots:
    void readFinished(vtkImageData *image);
private slots:
    void openNiftiFile();
    void openDICOMDirectory();
    void quit();
    void about();
private:
    void createMenus();
    void createActions();
    void createStatusBar();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    CentralWidget *centralWidget;
    MedicalImageReaderThread *imageReaderThread;
    QMenu *fileMenu{};
    QMenu *aboutMenu{};
    QAction *openNiftiAction{};
    QAction *openDICOMAction{};
    QAction *quitAction{};
    QAction *aboutAction{};
};


#endif //MEDX_MEDXMAINWINDOW_H
