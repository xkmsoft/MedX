//
// Created by chasank on 02/10/2021.
//

#include "MedXApplication.h"
#include "MedXMainWindow.h"
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>

int main(int argc, char **argv) {
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    MedXApplication application(argc, argv);
    MedXMainWindow mainWindow;
    mainWindow.show();
    return MedXApplication::exec();
}
