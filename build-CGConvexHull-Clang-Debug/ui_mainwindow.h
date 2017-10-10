/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "mylistview.h"
#include "paintcanvas.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_3;
    PaintCanvas *canvas;
    MyListView *primitivesList;
    QPushButton *applyButton;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(494, 372);
        gridLayout = new QGridLayout(MainWindow);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        canvas = new PaintCanvas(MainWindow);
        canvas->setObjectName(QStringLiteral("canvas"));
        canvas->setFrameShape(QFrame::StyledPanel);
        canvas->setFrameShadow(QFrame::Raised);

        gridLayout_3->addWidget(canvas, 1, 0, 1, 1);

        primitivesList = new MyListView(MainWindow);
        primitivesList->setObjectName(QStringLiteral("primitivesList"));

        gridLayout_3->addWidget(primitivesList, 1, 1, 1, 1);

        applyButton = new QPushButton(MainWindow);
        applyButton->setObjectName(QStringLiteral("applyButton"));

        gridLayout_3->addWidget(applyButton, 2, 0, 1, 2);

        gridLayout_3->setColumnStretch(0, 1);

        gridLayout->addLayout(gridLayout_3, 0, 0, 1, 1);


        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        applyButton->setText(QApplication::translate("MainWindow", "Apply", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
