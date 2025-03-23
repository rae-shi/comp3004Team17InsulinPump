/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *mainFrame;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *buttonLayout;
    QPushButton *startButton;
    QPushButton *chargeButton;
    QPushButton *disconnectButton;
    QHBoxLayout *topLayout;
    QProgressBar *batteryBar;
    QProgressBar *progressBar;
    QPushButton *bolusButton;
    QLabel *glucoseLabel;
    QLabel *label;
    QPlainTextEdit *logOutput;
    QFrame *profileFrame;
    QLabel *profileHeaderLabel;
    QPushButton *createProfileButton;
    QWidget *layoutWidget1;
    QHBoxLayout *defaultProfileLayout;
    QRadioButton *defaultProfileRadioButton;
    QSpacerItem *spacer1;
    QDoubleSpinBox *defaultProfileBasalSpinBox;
    QSpacerItem *spacer2;
    QDoubleSpinBox *deafultProfileGlucoseSpinBox;
    QWidget *layoutWidget2;
    QHBoxLayout *profileLabelLayout;
    QLabel *profileLabel;
    QLabel *basalLabel;
    QLabel *targetGlucoseLabel;
    QWidget *layoutWidget3;
    QHBoxLayout *ProfileButtonLayout;
    QPushButton *deleteProfileButton;
    QPushButton *submitProfileButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1496, 446);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        mainFrame = new QFrame(centralwidget);
        mainFrame->setObjectName(QString::fromUtf8("mainFrame"));
        mainFrame->setGeometry(QRect(10, 10, 481, 361));
        mainFrame->setFrameShape(QFrame::StyledPanel);
        mainFrame->setFrameShadow(QFrame::Raised);
        layoutWidget = new QWidget(mainFrame);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 451, 318));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        startButton = new QPushButton(layoutWidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));

        buttonLayout->addWidget(startButton);

        chargeButton = new QPushButton(layoutWidget);
        chargeButton->setObjectName(QString::fromUtf8("chargeButton"));

        buttonLayout->addWidget(chargeButton);

        disconnectButton = new QPushButton(layoutWidget);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));

        buttonLayout->addWidget(disconnectButton);


        verticalLayout_3->addLayout(buttonLayout);

        topLayout = new QHBoxLayout();
        topLayout->setObjectName(QString::fromUtf8("topLayout"));
        batteryBar = new QProgressBar(layoutWidget);
        batteryBar->setObjectName(QString::fromUtf8("batteryBar"));
        batteryBar->setStyleSheet(QString::fromUtf8("QProgressBar{ text-align:center; border: 2px solid grey; border-radius: 3px;} QProgressBar::chunk {background-color: #93C572; margin: 0.5px;}"));
        batteryBar->setMinimum(0);
        batteryBar->setMaximum(100);
        batteryBar->setValue(100);

        topLayout->addWidget(batteryBar);

        progressBar = new QProgressBar(layoutWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setStyleSheet(QString::fromUtf8("QProgressBar{ text-align:center; border: 2px solid grey; border-radius: 3px;}  QProgressBar::chunk {background-color: #89CFF0;margin: 0.5px;;}  "));
        progressBar->setMaximum(300);
        progressBar->setValue(300);
        progressBar->setTextVisible(true);
        progressBar->setInvertedAppearance(false);

        topLayout->addWidget(progressBar);


        verticalLayout_3->addLayout(topLayout);

        bolusButton = new QPushButton(layoutWidget);
        bolusButton->setObjectName(QString::fromUtf8("bolusButton"));

        verticalLayout_3->addWidget(bolusButton);

        glucoseLabel = new QLabel(layoutWidget);
        glucoseLabel->setObjectName(QString::fromUtf8("glucoseLabel"));

        verticalLayout_3->addWidget(glucoseLabel);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_3->addWidget(label);

        logOutput = new QPlainTextEdit(layoutWidget);
        logOutput->setObjectName(QString::fromUtf8("logOutput"));
        logOutput->setStyleSheet(QString::fromUtf8(""));
        logOutput->setReadOnly(true);

        verticalLayout_3->addWidget(logOutput);

        profileFrame = new QFrame(centralwidget);
        profileFrame->setObjectName(QString::fromUtf8("profileFrame"));
        profileFrame->setGeometry(QRect(500, 10, 401, 361));
        profileFrame->setFrameShape(QFrame::StyledPanel);
        profileFrame->setFrameShadow(QFrame::Raised);
        profileHeaderLabel = new QLabel(profileFrame);
        profileHeaderLabel->setObjectName(QString::fromUtf8("profileHeaderLabel"));
        profileHeaderLabel->setGeometry(QRect(11, 11, 105, 17));
        createProfileButton = new QPushButton(profileFrame);
        createProfileButton->setObjectName(QString::fromUtf8("createProfileButton"));
        createProfileButton->setGeometry(QRect(310, 10, 84, 25));
        layoutWidget1 = new QWidget(profileFrame);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 80, 371, 31));
        defaultProfileLayout = new QHBoxLayout(layoutWidget1);
        defaultProfileLayout->setObjectName(QString::fromUtf8("defaultProfileLayout"));
        defaultProfileLayout->setContentsMargins(0, 0, 0, 0);
        defaultProfileRadioButton = new QRadioButton(layoutWidget1);
        defaultProfileRadioButton->setObjectName(QString::fromUtf8("defaultProfileRadioButton"));

        defaultProfileLayout->addWidget(defaultProfileRadioButton);

        spacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        defaultProfileLayout->addItem(spacer1);

        defaultProfileBasalSpinBox = new QDoubleSpinBox(layoutWidget1);
        defaultProfileBasalSpinBox->setObjectName(QString::fromUtf8("defaultProfileBasalSpinBox"));

        defaultProfileLayout->addWidget(defaultProfileBasalSpinBox);

        spacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        defaultProfileLayout->addItem(spacer2);

        deafultProfileGlucoseSpinBox = new QDoubleSpinBox(layoutWidget1);
        deafultProfileGlucoseSpinBox->setObjectName(QString::fromUtf8("deafultProfileGlucoseSpinBox"));

        defaultProfileLayout->addWidget(deafultProfileGlucoseSpinBox);

        layoutWidget2 = new QWidget(profileFrame);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(40, 50, 361, 22));
        profileLabelLayout = new QHBoxLayout(layoutWidget2);
        profileLabelLayout->setObjectName(QString::fromUtf8("profileLabelLayout"));
        profileLabelLayout->setContentsMargins(0, 0, 0, 0);
        profileLabel = new QLabel(layoutWidget2);
        profileLabel->setObjectName(QString::fromUtf8("profileLabel"));

        profileLabelLayout->addWidget(profileLabel);

        basalLabel = new QLabel(layoutWidget2);
        basalLabel->setObjectName(QString::fromUtf8("basalLabel"));

        profileLabelLayout->addWidget(basalLabel);

        targetGlucoseLabel = new QLabel(layoutWidget2);
        targetGlucoseLabel->setObjectName(QString::fromUtf8("targetGlucoseLabel"));

        profileLabelLayout->addWidget(targetGlucoseLabel);

        layoutWidget3 = new QWidget(profileFrame);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(220, 310, 168, 27));
        ProfileButtonLayout = new QHBoxLayout(layoutWidget3);
        ProfileButtonLayout->setObjectName(QString::fromUtf8("ProfileButtonLayout"));
        ProfileButtonLayout->setContentsMargins(0, 0, 0, 0);
        deleteProfileButton = new QPushButton(layoutWidget3);
        deleteProfileButton->setObjectName(QString::fromUtf8("deleteProfileButton"));

        ProfileButtonLayout->addWidget(deleteProfileButton);

        submitProfileButton = new QPushButton(layoutWidget3);
        submitProfileButton->setObjectName(QString::fromUtf8("submitProfileButton"));

        ProfileButtonLayout->addWidget(submitProfileButton);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1496, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        startButton->setText(QCoreApplication::translate("MainWindow", "Power On", nullptr));
        chargeButton->setText(QCoreApplication::translate("MainWindow", "Charge Battery", nullptr));
        disconnectButton->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
        batteryBar->setFormat(QCoreApplication::translate("MainWindow", "Battery: %p%", nullptr));
        progressBar->setFormat(QCoreApplication::translate("MainWindow", "Cartridge: %vu", nullptr));
        bolusButton->setText(QCoreApplication::translate("MainWindow", "Inject Bolus", nullptr));
        glucoseLabel->setText(QCoreApplication::translate("MainWindow", "Current Glucose: 5.5 mmol/L", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Insulin On Board (IOB): 2.5u | 3:45 hrs", nullptr));
        profileHeaderLabel->setText(QCoreApplication::translate("MainWindow", "Personal Profile:", nullptr));
        createProfileButton->setText(QCoreApplication::translate("MainWindow", "Create New", nullptr));
        defaultProfileRadioButton->setText(QCoreApplication::translate("MainWindow", "   Regular", nullptr));
        profileLabel->setText(QCoreApplication::translate("MainWindow", "Profile:", nullptr));
        basalLabel->setText(QCoreApplication::translate("MainWindow", "Basal Rate:", nullptr));
        targetGlucoseLabel->setText(QCoreApplication::translate("MainWindow", "Target Glucose:", nullptr));
        deleteProfileButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        submitProfileButton->setText(QCoreApplication::translate("MainWindow", "Submit", nullptr));
        (void)MainWindow;
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
