//
// Created by chasank on 02/10/2021.
//

#ifndef MEDX_CENTRALWIDGET_H
#define MEDX_CENTRALWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QComboBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include "VolumeRenderingWidget.h"
#include "CrossSectionalWidget.h"

class CentralWidget: public QWidget{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent= nullptr);
    VolumeRenderingWidget *getVolumeRenderingWidget() const;
    CrossSectionalWidget *getSagittalWidget() const;
    CrossSectionalWidget *getCoronalWidget() const;
    CrossSectionalWidget *getAxialWidget() const;

private:
    QGridLayout *layout;
    VolumeRenderingWidget *volumeRenderingWidget;
    CrossSectionalWidget *sagittalWidget;
    CrossSectionalWidget *coronalWidget;
    CrossSectionalWidget *axialWidget;
};


#endif //MEDX_CENTRALWIDGET_H
