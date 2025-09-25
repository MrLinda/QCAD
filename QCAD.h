#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QCAD.h"

class QCAD : public QMainWindow
{
    Q_OBJECT

public:
    QCAD(QWidget *parent = nullptr);
    ~QCAD();

private:
    Ui::QCADClass ui;
};
