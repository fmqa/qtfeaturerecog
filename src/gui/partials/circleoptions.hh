#ifndef GUI_CIRCLEOPTIONS_HH
#define GUI_CIRCLEOPTIONS_HH

#include <QWidget>

class QSpinBox;

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
    private slots:
        void onValueChanged(int);
    signals:
        void changed();
    private:
        QSpinBox *minRadiusSpinBox;
        QSpinBox *maxRadiusSpinBox;
        QSpinBox *minScoreSpinBox;
        QSpinBox *maxScoreSpinBox;
    };
}

#endif /* #ifndef GUI_CIRCLEOPTIONS_HH */
