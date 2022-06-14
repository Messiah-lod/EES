#include "FileJob.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>

FileAbout::FileAbout(QObject *parent)
    : QObject{parent}
{

}

QString FileAbout::read()
{
    qDebug() << mSource;
    if (mSource.isEmpty()){
        emit error("source is empty");
        return QString();
    }

    QFile file(mSource);
    QString fileContent;
    if ( file.open(QIODevice::ReadOnly) ) {
        QByteArray data;
        data = file.readAll();
        file.close();
        fileContent = QString(data);
    } else {
        emit error("Unable to open the file");
        return QString();
    }
 //   qDebug() << fileContent;
    return fileContent;
}

bool FileAbout::write(const QString& data)
{
    if (mSource.isEmpty())
        return false;

    QFile file(mSource);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    QTextStream out(&file);
    out << data;

    file.close();

    return true;
}
