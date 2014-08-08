#ifndef TTS_HH
#define TTS_HH

#include <QObject>
#include <QProcess>
#include <QString>

namespace tts {
    class festival : public QObject {
        Q_OBJECT
    public:
        explicit festival(QObject * = 0);
        void speak(const QString &, const QString &);
    signals:
        void failed();
        void success();
    private:
        QString resolveLang(const QString &) const;
    private slots:
        void started();
        void error(QProcess::ProcessError);
        void finished(int, QProcess::ExitStatus);
    private:
        QProcess *process;
        QString text;
        QString language;
    };
}

#endif /* #ifndef TTS_HH */
