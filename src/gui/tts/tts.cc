#include "tts.hh"

tts::festival::festival(QObject *parent) : QObject(parent) {}

void tts::festival::speak(const QString &txt, const QString &lang) {
    text = txt;
    language = resolveLang(lang);
    process = new QProcess(this);
    connect(process, SIGNAL(started()), this, SLOT(started()));
    connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    process->start("festival", QStringList() << "--tts" << QString("--language=%1").arg(language));
}

QString tts::festival::resolveLang(const QString &lname) const {
    if (lname == "de") return "german";
    return "english";
}

void tts::festival::started() {
    process->write(text.toUtf8());
    process->closeWriteChannel();
}

void tts::festival::error(QProcess::ProcessError) {
    emit failed();
}

void tts::festival::finished(int, QProcess::ExitStatus) {
    emit success();
}
