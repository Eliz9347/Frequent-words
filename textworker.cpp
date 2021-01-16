#include "textworker.h"
#include <QDebug>


TextWorker::TextWorker(QObject *parent) : QObject(parent)
{

}

TextWorker::TextWorker(QString normText, int num, QSharedPointer<QList<QPair<QString, int>>> selected)
    :text(normText), number(num), selectedPairs(selected)
{

}

void TextWorker::doWork()
{
    qDebug() << "Try to analyse in textWorker ";
    if(text == 0) {
       emit trouble();
       return;
    }
    TextAnalysis ta;
    ta.AnalyseText(text);
    ta.SortPairs();
    qDebug() << " Try to join in textWorker ";
    ta.JoinNtoList(number, *selectedPairs);

    emit finished();
    return;
}





TextWorker::~TextWorker()
{

}


void TextWorker::stopWork()
{

}
