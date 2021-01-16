#include "fileworker.h"

FileWorker::FileWorker(QObject *parent) : QObject(parent)
{
    QList<QPair<QString, int>>* ptr = &selectedPairs;
    s = QSharedPointer<QList<QPair<QString, int>>>(ptr);
    analysed = 0;

}

FileWorker::FileWorker(QStringList filenames, int num)
    :files(filenames), number(num){
    QList<QPair<QString, int>>* ptr = &selectedPairs;
    s = QSharedPointer<QList<QPair<QString, int>>>(ptr);
    analysed = 0;

}

// Запуск нормализации с помощью утилиты MyStem в отдельном процессе
QString FileWorker::normaliseText(QString filename, int numf)
{
    qDebug() << "Try to normalise (inner) " << filename;
    QString program = "D:\\MyPrograms\\Programs_Qt\\Frequent_words\\mystem.exe";

    //Файл с нормализованным текстом
    QString res = "D:\\MyPrograms\\Programs_Qt\\Frequent_words\\normalised"+QString::number(numf)+".txt";
    QFile file("normalised"+QString::number(numf)+".txt");

    file.close(); // Закрытие файла

    //Аргументы для MyStem (входной и выходной файлы)
    QStringList arguments;
    arguments << filename << res;

    //Запуск программы MyStem в отдельном процессе
    QProcess *myProcess = new QProcess();
    myProcess->start(program, arguments);
    myProcess->waitForFinished();

    return res;
}

// Чтение файла с нормализованным текстом
QString FileWorker::readFile(QString filename){

    //QTextStream out(stdout);
    QFile file(filename);
    qDebug() << "Try to read (inner) " << filename;

    //Открытие файла в режиме чтения. Если файл не найден, то выводим предупреждение и завершаем программу
    if(!file.open(QIODevice::ReadOnly)){
        qWarning("Cannot open file for reading, try");
        //QThread::msleep(100);
        file.open(QIODevice::ReadOnly);
        return 0;
    }

    QTextStream in(&file);  // Создание входящего потока, из которого будут считываться данные и связывание его с файлом
    QString line = in.readLine();  // Считывание файла

    file.close();   // Закрытие файла
    QFile::remove(filename); // Удаление файла с нормализованным текстом - пока попробуем
    return line;
}

//проверить
void FileWorker::addTextWorker(QString text, int num, QSharedPointer<QList<QPair<QString, int>>> s){
    QThread* tthread = new QThread;
    TextWorker *textworker = new TextWorker(text, num, s);
    textworker->moveToThread(tthread);

    connect(tthread, &QThread::started, textworker, &TextWorker::doWork); // при запуске потока будет вызван метод doWork(), который создаст построитель отчетов, который будет работать в новом потоке
    connect(textworker, &TextWorker::finished, tthread, &QThread::quit); // при завершении работы анализатора текста, обертка анализатора передаст потоку сигнал finished() , вызвав срабатывание слота quit()
    connect(textworker, &TextWorker::finished, this, &FileWorker::checkTextAnalysers);
    connect(textworker, &TextWorker::finished, textworker, &QObject::deleteLater);  // обертка пометит себя для удаления при окончании анализа текста
    connect(tthread, &QThread::finished, tthread, &QObject::deleteLater);  // поток пометит себя для удаления, по окончании анализа текста. Удаление будет произведено только после полной остановки потока

    tthread->start(); // Запускаем поток, он запускает WorkerThread::doWork(), который создает NormalizedText и запускает  анализ текста
    qDebug() << "FileWorker launched ";
    return ;
}

FileWorker::~FileWorker()
{
    //s = nullptr;
}

// Запуск нормализации текстов, чтение нормализованных файлов, и запуск анализа текстов в отдельных потоках
void FileWorker::doWork(){

    for (int i = 0; i<files.size(); i++){

        qDebug() << "doWork " << files[i];

        QString normFile = normaliseText(files[i], i);

        qDebug() << "doWork " << normFile;
        QString text = readFile(normFile);

        addTextWorker(text, number, s);

    }
}

// Проверка воркера, работающего с текстом при получении сигнала о его готовности
void FileWorker::checkTextAnalysers(){
    analysed++;
    qDebug() << "Check Analysers " << analysed << "/" << files.size();
    if(analysed == files.size()){
        TextAnalysis selected(selectedPairs);
        selected.SortPairs();
        selected.CutNList(number);
        selected.PrintNPairs(selected.getSize());
        qDebug() << "Try to pass results ";
        //emit resultReady0();
        emit resultReady(selected);
    }
}
