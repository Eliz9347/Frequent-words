#ifndef TEXTANALYSIS_H
#define TEXTANALYSIS_H
//Для  русского текста
#include <QTextCodec>
#include <QMutex>
#include <QMutexLocker>
#include <QString>
//Для сравнения строк
const int STR_EQUAL=0;

#include <QDebug>

class TextAnalysis
{
public:
    TextAnalysis() = default;
    TextAnalysis(const TextAnalysis&);
    TextAnalysis(QList<QPair<QString, int>> pairs);
    int getSize();
    void AnalyseText(QString text);  //составление списка посчитанных слов, отсортированных в порядке убывания использований в тексте
    void SortPairs(); // сортировка списка пар "слово - кол-во использований" по кол-ву использований в порядке убывания
    void CutNList(int n); // Оставляет n слов и удаляет остальные
    void JoinNtoList(int n, QList<QPair<QString, int>> &selected); // добавление n самых часто встречающихся слов в список selected
    void PrintNPairs(int n); // вывод n верних пар из списка в debug
    QString printPair(int i);
     ~TextAnalysis() = default;

private:
    QList<QPair<QString, int>> pairs;  // список пар "слово - кол-во использований"
    QMutex mutex;
};

#endif // TEXTANALYSIS_H
