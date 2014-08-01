#ifndef GUI_CIRCLEOPTIONS_HH
#define GUI_CIRCLEOPTIONS_HH

#include <QWidget>
#include <QImage>

class QSpinBox;
class QLabel;
class QPushButton;

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
    };
}

#endif /* #ifndef GUI_CIRCLEOPTIONS_HH */
