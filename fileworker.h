#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <QObject>
#include <QWidget>

//Для чтения файла
#include <QTextStream>
#include <QFile>
//Для  русского текста
#include <QTextCodec>
//Для выбора файла
#include <QFileDialog>
#include <QString>

//Для запуска стороннего процесса
#include <QProcess>

#include <textanalysis.h>
#include <textworker.h>

#include <QDebug>

class FileWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileWorker(QObject *parent = nullptr);
    FileWorker(QStringList filenames, int num);
    QString normaliseText(QString filename, int numf); // Нормализация текста номер numf утилитой MyStem в отдельном процессе
    QString readFile(QString filename); //Чтение файла
    void addTextWorker(QString text, int num, QSharedPointer<QList<QPair<QString, int>>> selected); // Добавление потока, анализирующего один текст и вставляющего num самых часто встречающихся слов в список selected
     ~FileWorker();

private:
    QStringList files; // Список файлов
    int number; // кол-во искомых слов
    int analysed; // кол-во проанализированных текстов
    QList<QPair<QString, int>> selectedPairs; // список пар "слово - кол-во использований", отобранных из выбранных файлов (по number из каждого)
    QSharedPointer<QList<QPair<QString, int>>> s  = nullptr; // указатель на суммарный список пар

public slots:
    void doWork(); // чтение файлов и распределение заданий на обработку текстов по потокам
    void checkTextAnalysers(); // проверка выполнения потоками заданий и окончательная обработка результатов

// сигналы о завершении работы анализатора текстов
signals:
    void finished();
    void resultReady0();
    void resultReady(TextAnalysis sel);

};

#endif // FILEWORKER_H
