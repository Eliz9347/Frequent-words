#include "textanalysis.h"

TextAnalysis::TextAnalysis(const TextAnalysis & newpairs)
{
    if (getSize() == 0 && newpairs.pairs.size() !=0){
        for (int i = 0; i < newpairs.pairs.size(); i++){
            pairs.append(newpairs.pairs[i]);
        }
    }
}

TextAnalysis::TextAnalysis(QList<QPair<QString, int>> newpairs){
    for (QPair<QString, int> pair : newpairs){
        pairs.append(pair);
    }
}

int TextAnalysis::getSize()
{
    return pairs.size();
}

void TextAnalysis::AnalyseText(QString line)
{
    qDebug() << "Try to analyse (inner) ";

    //Разделение текста на отдельные слова со словоформами
    QStringList items = line.split("}");
    if(items.size()==0){
        qDebug() << "size = 0";
        return;
    }
    QStringListIterator it(items);

    //разбираем строчку, берём начальную форму и заносим в словарь (список пар "слово - кол-во"), если её там ещё нет, если есть, то инкрементируем соответствующее значение
    while (it.hasNext()) {
        QString words = it.next();
        int a = words.indexOf("{")+1;
        int b = words.indexOf("|");
        QString word = words.mid(a, b-a);
        bool have = false;

        for (int i = 0; i<pairs.size(); i++){
            QString nstr(pairs[i].first);
            if (QString::compare(nstr, word) == STR_EQUAL){
                pairs[i].second++;
                have = true;
                break;
            } else {
                have = false;
            }
        }

        if (!have){
            QPair<QString, int> newpair(word, 1);
            pairs.append(newpair);
        }
    }
    pairs.removeLast();
    qDebug() << "Text analysed " << pairs.size();
}

void TextAnalysis::SortPairs(){
    qDebug() << "Try to sort (inner) ";
    //Сортировка списка пар по значению (частоте)
    std::sort(pairs.begin(), pairs.end(), [](QPair<QString, int> a, QPair<QString, int> b) {
        return a.second > b.second;
    });
}

void TextAnalysis::CutNList(int n){
    qDebug() << "Try to cut (inner) " << n;
    int  nn = pairs.size()-n;
    for (int i = 0; i<nn; i++){
        pairs.removeLast();
    }
    qDebug() << "new size " << pairs.size();
}

void TextAnalysis::JoinNtoList(int n, QList<QPair<QString, int>> &selected){
    qDebug() << "Try to join (inner) ";
    QMutexLocker locker(&mutex);
    for (int i = 0; i<n; i++){
        bool have = false;

        for (int j = 0; j<selected.size(); j++){
            if (QString::compare(pairs[i].first, selected[j].first) == STR_EQUAL){
                selected[j].second += pairs[i].second;
                have = true;
                break;
            }
        }

        if (!have){
            selected.append(pairs[i]);
        }
    }

    for (int i = 0; i<selected.size(); i++){
      qDebug() << selected.at(i).first;
      qDebug() << QString::number(selected.at(i).second); //pairs[i].second
    }
}

void TextAnalysis::PrintNPairs(int n){
    qDebug() << "Try to print (inner) " << n;
    for (int i = 0; i<n; i++){
      qDebug() << pairs.at(i).first;
      qDebug() << QString::number(pairs.at(i).second); //pairs[i].second
    }
}

QString TextAnalysis::printPair(int i)
{
    return pairs[i].first+"   "+QString::number(pairs[i].second);
}


