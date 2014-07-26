#ifndef GUI_MAINWINDOW_HH
#define GUI_MAINWINDOW_HH

#include <QMainWindow>
#include <QImage>
#include <raster/util/view2d.hh>
#include "../model/ellipse_detector.hh"

class QAction;
class QMenu;
class QMenuBar;
class QTabWidget;
class QPushButton;
class QLabel;
class QThread;
class QTimer;

namespace mm {
    class EdgeOptions;
    class CircleOptions;
    class EllipseOptions;
    class ImageTabs;
    
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow();
    private slots:
        void open();
        void displaySourceImage();
        void displayEdgeImage();
        void displayTransformedImage();
        void extractEdgesAndDisplay();
        void extractCirclesAndDisplay();
        void extractEllipsesAndDisplay();
        void dispatchExtractionMethod();
        void onEdgeOptionChange();
        void stopProcessing();
        void activateFullScreen(bool);
    private:
        void createActions();
        void createMenus();
        void createToolBars();
        
        void loadSourceImage();
        void extractEdges();
        void fillEdgeImage();
        
        void extractCircles();
        void extractEllipses();
        
        void setControlState(bool);
        
        void resetState();
        
        QMenu *fileMenu;
        QToolBar *fileToolBar;
        QAction *openAct, *exitAct;
        
        QMenu *viewMenu;
        QAction *viewSourceImageAct, *viewEdgeImageAct, *viewTransformedImageAct, *activateFullScreenAct;
        
        QLabel *statusLabel;
        
        EdgeOptions *edgeOptions;
        QLabel *edgeCountLabel;
        
        QTabWidget *optionTabWidget;
        CircleOptions *circleOptions;
        EllipseOptions *ellipseOptions;
        
        QLabel *featureCountLabel;
        
        QPushButton *showEdgesButton;
        QPushButton *startTransformButton;
        QPushButton *stopButton;
        
        ImageTabs *imageTabsWidget;
        
        QString sourceImagePath;
        QImage sourceImage;
        QImage transformedImage;
        QImage edgeImage;
        
        QVector<std::pair<int,int>> edgeList;
        
        ellipse_detector ellipseModel;
        
        std::vector<bool> bits;
        raster::view2d<std::vector<bool>,int> edgeMap;
        
        QThread *workerThread;
        QTimer *progressTimer;
    };
}

#endif /* #ifndef GUI_MAINWINDOW_HH */
