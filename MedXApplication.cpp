//
// Created by chasank on 02/10/2021.
//

#include "MedXApplication.h"

bool MedXApplication::notify(QObject *object, QEvent *event) {
    try {
        return QApplication::notify(object, event);
    } catch (std::exception &exception) {
        std::cerr << "std::exception caught: " << exception.what() << std::endl;
    }
    return false;
}
