//
// Created by chasank on 02/10/2021.
//

#ifndef MEDX_MEDXAPPLICATION_H
#define MEDX_MEDXAPPLICATION_H

#include <QApplication>
#include <QObject>
#include <iostream>

class MedXApplication : public QApplication{
    Q_OBJECT
public:
    MedXApplication(int &argc, char **argv) : QApplication(argc, argv) {}
    bool notify(QObject *object, QEvent *event) override;
};


#endif //MEDX_MEDXAPPLICATION_H
