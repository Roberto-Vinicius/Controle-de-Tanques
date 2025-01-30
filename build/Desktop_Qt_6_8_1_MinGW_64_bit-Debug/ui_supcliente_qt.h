/********************************************************************************
** Form generated from reading UI file 'supcliente_qt.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUPCLIENTE_QT_H
#define UI_SUPCLIENTE_QT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SupClienteQt
{
public:
    QAction *actionLogin;
    QAction *actionLogout;
    QAction *actionQuit;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QLabel *labelActuators;
    QLabel *labelV1;
    QPushButton *buttonV1;
    QPushButton *buttonV2;
    QLabel *labelV2;
    QLabel *labelPumpSlider;
    QSlider *sliderPump;
    QLabel *labelSensors;
    QLabel *labelH1;
    QLCDNumber *lcdH1Cm;
    QLCDNumber *lcdH1Perc;
    QLCDNumber *lcdOverflow;
    QLabel *unitH1Cm;
    QLabel *unitH1Perc;
    QLabel *unitH2Cm;
    QLCDNumber *lcdH2Cm;
    QLCDNumber *lcdH2Perc;
    QLabel *unitH2Perc;
    QLabel *labelH2;
    QLabel *labelPump;
    QLCDNumber *lcdPumpVal;
    QLCDNumber *lcdPumpPerc;
    QLabel *unitPumpPerc;
    QLCDNumber *lcdFlowLMin;
    QLabel *unitFlowLMin;
    QLCDNumber *lcdFlowPerc;
    QLabel *unitFlowPerc;
    QLabel *labelFlow;
    QLabel *labelVisualization;
    QRadioButton *showLevel;
    QRadioButton *showGraph;
    QLabel *labelRefresh;
    QSpinBox *spinRefresh;
    QLabel *unitRefreshS;
    QMenuBar *menubar;
    QMenu *menuConnection;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SupClienteQt)
    {
        if (SupClienteQt->objectName().isEmpty())
            SupClienteQt->setObjectName("SupClienteQt");
        SupClienteQt->resize(984, 661);
        actionLogin = new QAction(SupClienteQt);
        actionLogin->setObjectName("actionLogin");
        actionLogout = new QAction(SupClienteQt);
        actionLogout->setObjectName("actionLogout");
        actionQuit = new QAction(SupClienteQt);
        actionQuit->setObjectName("actionQuit");
        centralwidget = new QWidget(SupClienteQt);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(360, 600));
        frame->setMaximumSize(QSize(16777215, 16777215));
        frame->setBaseSize(QSize(360, 600));
        frame->setFrameShape(QFrame::Box);
        labelActuators = new QLabel(frame);
        labelActuators->setObjectName("labelActuators");
        labelActuators->setGeometry(QRect(25, 25, 310, 25));
        labelActuators->setFrameShape(QFrame::Box);
        labelActuators->setAlignment(Qt::AlignCenter);
        labelV1 = new QLabel(frame);
        labelV1->setObjectName("labelV1");
        labelV1->setGeometry(QRect(25, 75, 40, 25));
        buttonV1 = new QPushButton(frame);
        buttonV1->setObjectName("buttonV1");
        buttonV1->setGeometry(QRect(65, 75, 75, 25));
        buttonV1->setCheckable(true);
        buttonV1->setFlat(false);
        buttonV2 = new QPushButton(frame);
        buttonV2->setObjectName("buttonV2");
        buttonV2->setGeometry(QRect(260, 75, 75, 25));
        buttonV2->setCheckable(true);
        buttonV2->setFlat(false);
        labelV2 = new QLabel(frame);
        labelV2->setObjectName("labelV2");
        labelV2->setGeometry(QRect(220, 75, 40, 25));
        labelPumpSlider = new QLabel(frame);
        labelPumpSlider->setObjectName("labelPumpSlider");
        labelPumpSlider->setGeometry(QRect(25, 125, 40, 25));
        sliderPump = new QSlider(frame);
        sliderPump->setObjectName("sliderPump");
        sliderPump->setGeometry(QRect(65, 125, 270, 25));
        sliderPump->setMaximum(65535);
        sliderPump->setSingleStep(100);
        sliderPump->setPageStep(1000);
        sliderPump->setOrientation(Qt::Horizontal);
        labelSensors = new QLabel(frame);
        labelSensors->setObjectName("labelSensors");
        labelSensors->setGeometry(QRect(25, 225, 310, 25));
        labelSensors->setFrameShape(QFrame::Box);
        labelSensors->setAlignment(Qt::AlignCenter);
        labelH1 = new QLabel(frame);
        labelH1->setObjectName("labelH1");
        labelH1->setGeometry(QRect(40, 285, 25, 25));
        lcdH1Cm = new QLCDNumber(frame);
        lcdH1Cm->setObjectName("lcdH1Cm");
        lcdH1Cm->setGeometry(QRect(65, 275, 75, 23));
        lcdH1Cm->setSmallDecimalPoint(false);
        lcdH1Cm->setDigitCount(5);
        lcdH1Cm->setSegmentStyle(QLCDNumber::Filled);
        lcdH1Cm->setProperty("value", QVariant(0.000000000000000));
        lcdH1Cm->setProperty("intValue", QVariant(0));
        lcdH1Perc = new QLCDNumber(frame);
        lcdH1Perc->setObjectName("lcdH1Perc");
        lcdH1Perc->setGeometry(QRect(65, 300, 75, 23));
        lcdH1Perc->setFrameShadow(QFrame::Raised);
        lcdH1Perc->setSmallDecimalPoint(false);
        lcdH1Perc->setDigitCount(5);
        lcdH1Perc->setSegmentStyle(QLCDNumber::Filled);
        lcdH1Perc->setProperty("value", QVariant(0.000000000000000));
        lcdH1Perc->setProperty("intValue", QVariant(0));
        lcdOverflow = new QLCDNumber(frame);
        lcdOverflow->setObjectName("lcdOverflow");
        lcdOverflow->setGeometry(QRect(65, 325, 75, 23));
        lcdOverflow->setFrameShape(QFrame::Box);
        lcdOverflow->setFrameShadow(QFrame::Raised);
        lcdOverflow->setSmallDecimalPoint(false);
        lcdOverflow->setDigitCount(5);
        lcdOverflow->setSegmentStyle(QLCDNumber::Filled);
        lcdOverflow->setProperty("value", QVariant(0.000000000000000));
        lcdOverflow->setProperty("intValue", QVariant(0));
        unitH1Cm = new QLabel(frame);
        unitH1Cm->setObjectName("unitH1Cm");
        unitH1Cm->setGeometry(QRect(145, 275, 20, 25));
        unitH1Perc = new QLabel(frame);
        unitH1Perc->setObjectName("unitH1Perc");
        unitH1Perc->setGeometry(QRect(145, 300, 20, 25));
        unitH2Cm = new QLabel(frame);
        unitH2Cm->setObjectName("unitH2Cm");
        unitH2Cm->setGeometry(QRect(305, 275, 20, 25));
        lcdH2Cm = new QLCDNumber(frame);
        lcdH2Cm->setObjectName("lcdH2Cm");
        lcdH2Cm->setGeometry(QRect(225, 275, 75, 23));
        lcdH2Cm->setSmallDecimalPoint(false);
        lcdH2Cm->setDigitCount(5);
        lcdH2Cm->setSegmentStyle(QLCDNumber::Filled);
        lcdH2Cm->setProperty("value", QVariant(0.000000000000000));
        lcdH2Cm->setProperty("intValue", QVariant(0));
        lcdH2Perc = new QLCDNumber(frame);
        lcdH2Perc->setObjectName("lcdH2Perc");
        lcdH2Perc->setGeometry(QRect(225, 300, 75, 23));
        lcdH2Perc->setSmallDecimalPoint(false);
        lcdH2Perc->setDigitCount(5);
        lcdH2Perc->setSegmentStyle(QLCDNumber::Filled);
        lcdH2Perc->setProperty("value", QVariant(0.000000000000000));
        lcdH2Perc->setProperty("intValue", QVariant(0));
        unitH2Perc = new QLabel(frame);
        unitH2Perc->setObjectName("unitH2Perc");
        unitH2Perc->setGeometry(QRect(305, 300, 20, 25));
        labelH2 = new QLabel(frame);
        labelH2->setObjectName("labelH2");
        labelH2->setGeometry(QRect(200, 285, 25, 25));
        labelPump = new QLabel(frame);
        labelPump->setObjectName("labelPump");
        labelPump->setGeometry(QRect(25, 385, 40, 25));
        lcdPumpVal = new QLCDNumber(frame);
        lcdPumpVal->setObjectName("lcdPumpVal");
        lcdPumpVal->setGeometry(QRect(65, 375, 75, 23));
        lcdPumpVal->setSmallDecimalPoint(false);
        lcdPumpVal->setDigitCount(5);
        lcdPumpVal->setSegmentStyle(QLCDNumber::Filled);
        lcdPumpVal->setProperty("value", QVariant(0.000000000000000));
        lcdPumpVal->setProperty("intValue", QVariant(0));
        lcdPumpPerc = new QLCDNumber(frame);
        lcdPumpPerc->setObjectName("lcdPumpPerc");
        lcdPumpPerc->setGeometry(QRect(65, 400, 75, 23));
        lcdPumpPerc->setSmallDecimalPoint(false);
        lcdPumpPerc->setDigitCount(5);
        lcdPumpPerc->setSegmentStyle(QLCDNumber::Filled);
        lcdPumpPerc->setProperty("value", QVariant(0.000000000000000));
        lcdPumpPerc->setProperty("intValue", QVariant(0));
        unitPumpPerc = new QLabel(frame);
        unitPumpPerc->setObjectName("unitPumpPerc");
        unitPumpPerc->setGeometry(QRect(145, 400, 20, 25));
        lcdFlowLMin = new QLCDNumber(frame);
        lcdFlowLMin->setObjectName("lcdFlowLMin");
        lcdFlowLMin->setGeometry(QRect(225, 375, 75, 23));
        lcdFlowLMin->setSmallDecimalPoint(false);
        lcdFlowLMin->setDigitCount(5);
        lcdFlowLMin->setSegmentStyle(QLCDNumber::Filled);
        lcdFlowLMin->setProperty("value", QVariant(0.000000000000000));
        lcdFlowLMin->setProperty("intValue", QVariant(0));
        unitFlowLMin = new QLabel(frame);
        unitFlowLMin->setObjectName("unitFlowLMin");
        unitFlowLMin->setGeometry(QRect(305, 375, 30, 25));
        lcdFlowPerc = new QLCDNumber(frame);
        lcdFlowPerc->setObjectName("lcdFlowPerc");
        lcdFlowPerc->setGeometry(QRect(225, 400, 75, 23));
        lcdFlowPerc->setSmallDecimalPoint(false);
        lcdFlowPerc->setDigitCount(5);
        lcdFlowPerc->setSegmentStyle(QLCDNumber::Filled);
        lcdFlowPerc->setProperty("value", QVariant(0.000000000000000));
        lcdFlowPerc->setProperty("intValue", QVariant(0));
        unitFlowPerc = new QLabel(frame);
        unitFlowPerc->setObjectName("unitFlowPerc");
        unitFlowPerc->setGeometry(QRect(305, 400, 20, 25));
        labelFlow = new QLabel(frame);
        labelFlow->setObjectName("labelFlow");
        labelFlow->setGeometry(QRect(190, 385, 25, 25));
        labelVisualization = new QLabel(frame);
        labelVisualization->setObjectName("labelVisualization");
        labelVisualization->setGeometry(QRect(25, 500, 310, 25));
        labelVisualization->setFrameShape(QFrame::Box);
        labelVisualization->setAlignment(Qt::AlignCenter);
        showLevel = new QRadioButton(frame);
        showLevel->setObjectName("showLevel");
        showLevel->setGeometry(QRect(25, 545, 70, 25));
        showLevel->setChecked(true);
        showGraph = new QRadioButton(frame);
        showGraph->setObjectName("showGraph");
        showGraph->setGeometry(QRect(85, 545, 70, 25));
        labelRefresh = new QLabel(frame);
        labelRefresh->setObjectName("labelRefresh");
        labelRefresh->setGeometry(QRect(185, 545, 81, 25));
        spinRefresh = new QSpinBox(frame);
        spinRefresh->setObjectName("spinRefresh");
        spinRefresh->setGeometry(QRect(270, 545, 50, 25));
        spinRefresh->setMinimum(10);
        spinRefresh->setMaximum(200);
        spinRefresh->setSingleStep(10);
        spinRefresh->setValue(20);
        unitRefreshS = new QLabel(frame);
        unitRefreshS->setObjectName("unitRefreshS");
        unitRefreshS->setGeometry(QRect(325, 545, 10, 25));

        horizontalLayout->addWidget(frame);

        SupClienteQt->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SupClienteQt);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 984, 22));
        menuConnection = new QMenu(menubar);
        menuConnection->setObjectName("menuConnection");
        SupClienteQt->setMenuBar(menubar);
        statusbar = new QStatusBar(SupClienteQt);
        statusbar->setObjectName("statusbar");
        SupClienteQt->setStatusBar(statusbar);

        menubar->addAction(menuConnection->menuAction());
        menuConnection->addAction(actionLogin);
        menuConnection->addAction(actionLogout);
        menuConnection->addSeparator();
        menuConnection->addAction(actionQuit);

        retranslateUi(SupClienteQt);

        QMetaObject::connectSlotsByName(SupClienteQt);
    } // setupUi

    void retranslateUi(QMainWindow *SupClienteQt)
    {
        SupClienteQt->setWindowTitle(QCoreApplication::translate("SupClienteQt", "Supervisorio - Tanques", nullptr));
        actionLogin->setText(QCoreApplication::translate("SupClienteQt", "Login...", nullptr));
        actionLogout->setText(QCoreApplication::translate("SupClienteQt", "Logout", nullptr));
        actionQuit->setText(QCoreApplication::translate("SupClienteQt", "Quit", nullptr));
        labelActuators->setText(QCoreApplication::translate("SupClienteQt", "ACTUATORS", nullptr));
        labelV1->setText(QCoreApplication::translate("SupClienteQt", "Valve 1", nullptr));
        buttonV1->setText(QCoreApplication::translate("SupClienteQt", "CLOSED", nullptr));
        buttonV2->setText(QCoreApplication::translate("SupClienteQt", "CLOSED", nullptr));
        labelV2->setText(QCoreApplication::translate("SupClienteQt", "Valve 2", nullptr));
        labelPumpSlider->setText(QCoreApplication::translate("SupClienteQt", "Pump", nullptr));
        labelSensors->setText(QCoreApplication::translate("SupClienteQt", "SENSORS", nullptr));
        labelH1->setText(QCoreApplication::translate("SupClienteQt", "H1", nullptr));
        unitH1Cm->setText(QCoreApplication::translate("SupClienteQt", "cm", nullptr));
        unitH1Perc->setText(QCoreApplication::translate("SupClienteQt", "%", nullptr));
        unitH2Cm->setText(QCoreApplication::translate("SupClienteQt", "cm", nullptr));
        unitH2Perc->setText(QCoreApplication::translate("SupClienteQt", "%", nullptr));
        labelH2->setText(QCoreApplication::translate("SupClienteQt", "H2", nullptr));
        labelPump->setText(QCoreApplication::translate("SupClienteQt", "Pump", nullptr));
        unitPumpPerc->setText(QCoreApplication::translate("SupClienteQt", "%", nullptr));
        unitFlowLMin->setText(QCoreApplication::translate("SupClienteQt", "l/min", nullptr));
        unitFlowPerc->setText(QCoreApplication::translate("SupClienteQt", "%", nullptr));
        labelFlow->setText(QCoreApplication::translate("SupClienteQt", "Flow", nullptr));
        labelVisualization->setText(QCoreApplication::translate("SupClienteQt", "VISUALIZATION", nullptr));
        showLevel->setText(QCoreApplication::translate("SupClienteQt", "Level", nullptr));
        showGraph->setText(QCoreApplication::translate("SupClienteQt", "Graph", nullptr));
        labelRefresh->setText(QCoreApplication::translate("SupClienteQt", "Refresh period:", nullptr));
        unitRefreshS->setText(QCoreApplication::translate("SupClienteQt", "s", nullptr));
        menuConnection->setTitle(QCoreApplication::translate("SupClienteQt", "Connection", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SupClienteQt: public Ui_SupClienteQt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUPCLIENTE_QT_H
