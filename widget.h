#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>

//Для  русского текста
#include <QTextCodec>
//Для чтения файла
#include <QTextStream>
#include <QFile>
//Для выбора файла
#include <QFileDialog>
//Для запуска стороннего процесса
#include <QProcess>

//Воркер работающий в отдельном потоке
#include "fileworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void printStringAtList(QString string);
    ~Widget();

// Получение и вывод результатов анализа
public slots:
    void handleResults0();
    void handleResults(TextAnalysis chosenPairs);

private slots:
    void on_loadButton_clicked(); // Загрузка файлов
    void on_findButton_clicked(); // Нахождение часто встречающихся слов
    void on_newButton_clicked(); // Очистка результатов предыдущего поиска

private:
    Ui::Widget *ui;
    QStringList fileNames; // список файлов для анализа
    void addAnalysingThread(QStringList normfiles, int number); // добавление потока для анализа текстов
    void stopThreads(); // остановка потоков

signals:
    void stopAll(); // остановка всех потоков
};
#endif // WIDGET_H
