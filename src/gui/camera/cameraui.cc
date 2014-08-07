#include <QWidget>
#include <QCameraViewfinder>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>
#include "cameraui.hh"

void CameraUi::setupUi(QWidget *parent) {
    actionExit = new QAction(parent);
    actionStartCamera = new QAction(parent);
    actionStopCamera = new QAction(parent);
    actionSettings = new QAction(parent);
    centralWidget = new QWidget();
    viewFinder = new QCameraViewfinder();
    
    takeImageButton = new QPushButton();
    
    {
        QVBoxLayout *vbox = new QVBoxLayout(centralWidget);
        vbox->addWidget(viewFinder);
        vbox->addWidget(takeImageButton);
    }
    
    menubar = new QMenuBar();
    menuFile = new QMenu(menubar);
    menuFile->addAction(actionStartCamera);
    menuFile->addAction(actionStopCamera);
    menuFile->addSeparator();
    menuFile->addAction(actionSettings);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);
    menuDevices = new QMenu(menubar);
    menubar->addAction(menuFile->menuAction());
    menubar->addAction(menuDevices->menuAction());
    
    {
        QVBoxLayout *layout = new QVBoxLayout(parent);
        layout->addWidget(menubar);
        layout->addWidget(centralWidget);
    }
    
    retranslateUi();
}

void CameraUi::retranslateUi() {
    actionExit->setText(QObject::tr("Exit"));
    actionStartCamera->setText(QObject::tr("Start Camera"));
    actionStopCamera->setText(QObject::tr("Stop Camera"));
    actionSettings->setText(QObject::tr("Settings"));
    menuFile->setTitle(QObject::tr("File"));
    menuDevices->setTitle(QObject::tr("Devices"));
    takeImageButton->setText(QObject::tr("Snapshot"));
}

