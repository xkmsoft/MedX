//
// Created by chasank on 02/10/2021.
//

#include "CentralWidget.h"

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent) {
    int w = 1024;
    int h = 768;

    sagittalWidget = new CrossSectionalWidget(Plane::SAGITTAL, this);
    sagittalWidget->setFixedSize(w, h);

    coronalWidget = new CrossSectionalWidget(Plane::CORONAL, this);
    coronalWidget->setFixedSize(w, h);

    axialWidget = new CrossSectionalWidget(Plane::AXIAL, this);
    axialWidget->setFixedSize(w, h);

    volumeRenderingWidget = new VolumeRenderingWidget(this);
    volumeRenderingWidget->setFixedSize(w, h);

    layout = new QGridLayout(this);
    layout->addWidget(sagittalWidget, 0, 0);
    layout->addWidget(coronalWidget, 0, 1);
    layout->addWidget(axialWidget, 1, 0);
    layout->addWidget(volumeRenderingWidget, 1, 1);
}

VolumeRenderingWidget *CentralWidget::getVolumeRenderingWidget() const {
    return volumeRenderingWidget;
}

CrossSectionalWidget *CentralWidget::getSagittalWidget() const {
    return sagittalWidget;
}

CrossSectionalWidget *CentralWidget::getCoronalWidget() const {
    return coronalWidget;
}

CrossSectionalWidget *CentralWidget::getAxialWidget() const {
    return axialWidget;
}

