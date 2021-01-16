#ifndef TEXTWORKER_H
#define TEXTWORKER_H

#include <QObject>
#include <QThread>
#include <textanalysis.h>

class TextWorker : public QObject
{
    Q_OBJECT
public:
    explicit TextWorker(QObject *parent = nullptr);
    TextWorker(QString filename, int num, QSharedPointer<QList<QPair<QString, int>>> s);
    ~TextWorker();

private:
    QString text;  // строка текста
    int number;   // кол-во искомых слов
    QSharedPointer<QList<QPair<QString, int>>> selectedPairs;   // указатель на общий список

public slots:
    void doWork();      // создает и запускает анализатор текстов
    void stopWork();    	// останавливает анализ текста

signals:
    void finished();
    void trouble(); //если не понадобится, удалить

};

#endif // TEXTWORKER_H
