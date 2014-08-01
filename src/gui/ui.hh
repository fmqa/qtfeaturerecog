#ifndef GUI_UI_HH
#define GUI_UI_HH

#include <QMainWindow>

class QMenu;
class QToolBar;
class QAction;
class QTabWidget;
class QPushButton;
class QLabel;

namespace mm {
    class EdgeOptions;
    class CircleOptions;
    class EllipseOptions;
    class ImageTabs;
    
    class Ui : public QMainWindow {
        Q_OBJECT
    private:
        struct uimenus { QMenu *file, *view; };
        struct uitoolbars { QToolBar *file; };
        struct uiactions { QAction *open, *save, *copy, *exit, *viewsrc, *viewedges, *viewtransfm, *fullscr; };
        struct uioptions { EdgeOptions *edges; CircleOptions *circles; EllipseOptions *ellipses; };
        struct uicontrols { QPushButton *detectedges, *applytransfm, *stop; };
    public:
        Ui();
        QString requestImage();
        void alertInvalidSourceImage(const QString &);
        void alertExtractEdges();
    public slots:
        void enableControls();
        void disableControls();
    public:
        uimenus menus;
        uitoolbars toolbars;
        uiactions actions;
        uioptions options;
        uicontrols controls;
        ImageTabs *images;
        QTabWidget *transfmtabs;
    };
}

#endif /* #ifndef GUI_UI_HH */
