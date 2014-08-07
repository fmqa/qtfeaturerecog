#ifndef CAMERAUI_HH
#define CAMERAUI_HH

class QWidget;
class QCameraViewfinder;
class QAction;
class QMenuBar;
class QMenu;
class QPushButton;

struct CameraUi {
    void setupUi(QWidget *);
    void retranslateUi();
public:
    QCameraViewfinder *viewFinder;
    QAction *actionExit;
    QAction *actionStartCamera;
    QAction *actionStopCamera;
    QAction *actionSettings;
    QPushButton *takeImageButton;
    QWidget *centralWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuDevices;
};

#endif /* #ifndef CAMERAUI_HH */
