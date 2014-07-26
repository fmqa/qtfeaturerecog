#ifndef GUI_IMAGETABS_HH
#define GUI_IMAGETABS_HH

#include <QWidget>

class QTabWidget;
class QScrollArea;
class QLabel;
class QPixmap;

namespace mm {
    class ImageTabs : public QWidget {
        Q_OBJECT
    public:
        enum tab { invalidtab = -1, srctab, edgetab, transformtab, histogramtab };
    public:
        explicit ImageTabs(QWidget * = 0);
        tab currentTab() const;
        void switchTab(tab);
        void setTabPixmap(tab, QPixmap const &);
    private:
        QTabWidget *tabWidget;
        QScrollArea *scrollAreaSrc, *scrollAreaEdges, *scrollAreaTransform,
                    *scrollAreaHistogram;
        QLabel *labelSrc, *labelEdges, *labelTransform, *labelHistogram;
    };
}

#endif /* #ifndef GUI_IMAGETABS_HH */
