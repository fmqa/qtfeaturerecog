#ifndef GUI_ELLIPSEOPTIONS_HH
#define GUI_ELLIPSEOPTIONS_HH

#include <QWidget>

class QSpinBox;

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
        EllipseOptions& setMinMajor(int);
        EllipseOptions& setMaxMajor(int);
        EllipseOptions& setMinMinor(int);
        EllipseOptions& setMaxMinor(int);
        EllipseOptions& setMinScore(int);
        EllipseOptions& setMaxScore(int);
    private slots:
        void onValueChanged(int);
    signals:
        void changed();
    private:
        QSpinBox *minMajorSpinBox;
        QSpinBox *maxMajorSpinBox;
        QSpinBox *minMinorSpinBox;
        QSpinBox *maxMinorSpinBox;
        QSpinBox *minScoreSpinBox;
        QSpinBox *maxScoreSpinBox;
    };
}

#endif /* #ifndef GUI_ELLIPSEOPTIONS_HH */
