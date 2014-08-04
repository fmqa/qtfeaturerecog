#ifndef GUI_UI_HH
#define GUI_UI_HH

#include <QMainWindow>

class QMenu;
class QToolBar;
class QAction;
class QTabWidget;
class QPushButton;
class QLabel;
class QEvent;
class QDropEvent;
class QString;
class QImage;
class QDragEnterEvent;
class QTranslator;

namespace mm {
    class EdgeOptions;
    class CircleOptions;
    class EllipseOptions;
    class ImageTabs;
    
    class Ui : public QMainWindow {
        Q_OBJECT
    private:
        struct uimenus { QMenu *file, *edit, *view, *language; };
        struct uitoolbars { QToolBar *file; };
        struct uiactions { QAction *open, *save, *savecirc, *copy, *copycirc, *exit, *viewsrc, *viewedges, *viewtransfm, *fullscr, *german, *english; };
        struct uioptions { EdgeOptions *edges; CircleOptions *circles; EllipseOptions *ellipses; };
        struct uicontrols { QPushButton *detectedges, *applytransfm, *stop; };
    private:
        QTranslator *translator;
    public:
        Ui();
        QString requestImage();
        QString requestSaveImage();
        void alertInvalidSourceImage(const QString &);
        void alertExtractEdges();
        void alertEmptySourceImage();
        void alertNoParametricCircles();
        void alertSaveError(const QString &);
    public slots:
        void enableControls();
        void disableControls();
    private slots:
        void changeLanguageGerman();
        void changeLanguageEnglish();
    private:
        void retranslateUi();
    signals:
        void fileDropped(const QString &);
        void imageDropped(const QImage &);
    public:
        uimenus menus;
        uitoolbars toolbars;
        uiactions actions;
        uioptions options;
        uicontrols controls;
        ImageTabs *images;
        QTabWidget *transfmtabs;
        QLabel *coordLabel;
    protected:
        void dragEnterEvent(QDragEnterEvent *) override;
        void dropEvent(QDropEvent *) override;
        void changeEvent(QEvent *) override;
    };
}

#endif /* #ifndef GUI_UI_HH */
