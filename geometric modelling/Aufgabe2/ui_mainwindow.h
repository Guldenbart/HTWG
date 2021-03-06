/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Thu 12. Apr 09:52:32 2012
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox_epsilon_draw;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_epsilon_intersection;
    QFrame *line;
    QCheckBox *checkBox_Intersection;
    QCheckBox *checkBox_SelfIntersection;
    QSpacerItem *verticalSpacer;
    GLWidget *glwidget;
    QHBoxLayout *horizontalLayout;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        MainWindow->setLocale(QLocale(QLocale::German, QLocale::Germany));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setMargin(11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        doubleSpinBox_epsilon_draw = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_epsilon_draw->setObjectName(QString::fromUtf8("doubleSpinBox_epsilon_draw"));
        doubleSpinBox_epsilon_draw->setDecimals(3);
        doubleSpinBox_epsilon_draw->setMinimum(0.001);
        doubleSpinBox_epsilon_draw->setMaximum(10);
        doubleSpinBox_epsilon_draw->setSingleStep(0.01);
        doubleSpinBox_epsilon_draw->setValue(0.05);

        verticalLayout->addWidget(doubleSpinBox_epsilon_draw);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        doubleSpinBox_epsilon_intersection = new QDoubleSpinBox(centralWidget);
        doubleSpinBox_epsilon_intersection->setObjectName(QString::fromUtf8("doubleSpinBox_epsilon_intersection"));
        doubleSpinBox_epsilon_intersection->setDecimals(6);
        doubleSpinBox_epsilon_intersection->setMinimum(1e-05);
        doubleSpinBox_epsilon_intersection->setMaximum(1);
        doubleSpinBox_epsilon_intersection->setSingleStep(1e-05);
        doubleSpinBox_epsilon_intersection->setValue(0.0005);

        verticalLayout->addWidget(doubleSpinBox_epsilon_intersection);

        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        checkBox_Intersection = new QCheckBox(centralWidget);
        checkBox_Intersection->setObjectName(QString::fromUtf8("checkBox_Intersection"));

        verticalLayout->addWidget(checkBox_Intersection);

        checkBox_SelfIntersection = new QCheckBox(centralWidget);
        checkBox_SelfIntersection->setObjectName(QString::fromUtf8("checkBox_SelfIntersection"));

        verticalLayout->addWidget(checkBox_SelfIntersection);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);

        glwidget = new GLWidget(centralWidget);
        glwidget->setObjectName(QString::fromUtf8("glwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(glwidget->sizePolicy().hasHeightForWidth());
        glwidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(glwidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        horizontalLayout_2->addWidget(glwidget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "\316\265 f\303\274r Zeichnen:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "\316\265 f\303\274r Schnitte:", 0, QApplication::UnicodeUTF8));
        checkBox_Intersection->setText(QApplication::translate("MainWindow", "Schnittpunkte", 0, QApplication::UnicodeUTF8));
        checkBox_SelfIntersection->setText(QApplication::translate("MainWindow", "Selbstschnitte", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
