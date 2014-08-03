#ifndef GUI_ELLIPSEOPTIONS_HH
#define GUI_ELLIPSEOPTIONS_HH

#include <QWidget>
#include <QImage>
#include <QColor>

class QSpinBox;
class QCheckBox;
class QPushButton;
class QLabel;

namespace mm {
    class EllipseOptions : public QWidget {
        Q_OBJECT
    public:
        explicit EllipseOptions(QWidget * = 0);
        int minMajor() const;
        int maxMajor() const;
        int minMinor() const;
        int maxMinor() const;
        int minScore() const;
        int maxScore() const;
        bool rhtEnabled() const;
        int rhtPercentage() const;
        QColor getColor() const;
        EllipseOptions& setMinMajor(int);
        EllipseOptions& setMaxMajor(int);
        EllipseOptions& setMinMinor(int);
        EllipseOptions& setMaxMinor(int);
        EllipseOptions& setMinScore(int);
        EllipseOptions& setMaxScore(int);
    private:
        void setColor();
    private slots:
        void onValueChanged(int);
        void rhtStateChanged(int);
        void pickColor();
    signals:
        void changed();
    private:
        QSpinBox *minMajorSpinBox;
        QSpinBox *maxMajorSpinBox;
        QSpinBox *minMinorSpinBox;
        QSpinBox *maxMinorSpinBox;
        QSpinBox *minScoreSpinBox;
        QSpinBox *maxScoreSpinBox;
        QCheckBox *chkRHT;
        QSpinBox *rhtEdgePercentage;
        QImage markerImage;
        QColor markerColor;
        QLabel *colorLabel;
        QPushButton *colorPickBtn;
        QCheckBox *chkHistogram;
    };
}

#endif /* #ifndef GUI_ELLIPSEOPTIONS_HH */
