#ifndef GUI_CIRCLEOPTIONS_HH
#define GUI_CIRCLEOPTIONS_HH

#include <QWidget>
#include <QImage>

class QSpinBox;
class QLabel;
class QPushButton;
class QCheckBox;
class QGroupBox;

namespace mm {
    class CircleOptions : public QWidget {
        Q_OBJECT
    public:
        explicit CircleOptions(QWidget * = 0);
        void setMaxRadiusBound(int);
        int minRadius() const;
        int maxRadius() const;
        int minScore() const;
        int maxScore() const;
        CircleOptions& setMinRadius(int);
        CircleOptions& setMaxRadius(int);
        CircleOptions& setMinScore(int);
        CircleOptions& setMaxScore(int);
        QColor getColor() const;
        bool histogram() const;
        void retranslateUi();
    private slots:
        void onValueChanged(int);
        void pickColor();
    signals:
        void changed();
    private:
        void setColor();
    private:
        QImage markerImage;
        QColor markerColor;
        QSpinBox *minRadiusSpinBox;
        QSpinBox *maxRadiusSpinBox;
        QSpinBox *minScoreSpinBox;
        QSpinBox *maxScoreSpinBox;
        QLabel *colorLabel;
        QPushButton *colorPickBtn;
        QCheckBox *chkHistogram;
        QGroupBox *circlesGroup;
        QLabel *lblRadiusMinMax, *lblScoreMinMax, *lblMarkerColor, *lblHistogram;
    };
}

#endif /* #ifndef GUI_CIRCLEOPTIONS_HH */
