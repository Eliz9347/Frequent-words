#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Q_DECLARE_METATYPE(TextAnalysis)

void Widget::addAnalysingThread(QStringList files, int number)
{
    QThread* thread = new QThread; // Создание потока
    FileWorker* fworker = new FileWorker(files, number); // Создание воркера для работы с файлами
    fworker->moveToThread(thread); // Перемещение воркера в созданный поток

    connect(thread, &QThread::started, fworker, &FileWorker::doWork); // при запуске потока будет вызван метод doWork(), который создаст построитель отчетов, который будет работать в новом потоке
    connect(fworker, &FileWorker::resultReady, this, &Widget::handleResults); // когда результаты анализа будут готовы, основной поток получит их и выведет в GUI
    //connect(fworker, &FileWorker::resultReady0, this, &Widget::handleResults0);
    connect(fworker, &FileWorker::finished, thread, &QThread::quit); // при завершении работы анализатора текста, обертка анализатора передаст потоку сигнал finished() , вызвав срабатывание слота quit()
    connect(thread, &QThread::finished, fworker, &QObject::deleteLater);  // обертка пометит себя для удаления при окончании анализа текста
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);  // поток пометит себя для удаления по окончании анализа текста. Удаление будет произведено только после полной остановки потока

    thread->start(); // Запускаем поток, он запускает FileWorker::doWork(), который создает textanalysis и запускает анализ текста
    qDebug() << "FileWorker launched ";
    return ;
}

void Widget::handleResults(TextAnalysis chosenPairs){
    qDebug() << "Results handled ";
    ui->listWidget->addItem(QString::number(chosenPairs.getSize())+" наиболее часто встречаемых слов:");
    for (int i = 0; i< chosenPairs.getSize(); i++){
        printStringAtList(chosenPairs.printPair(i));
    }
    ui->findButton->setEnabled(true);
    ui->loadButton->setEnabled(true);
    ui->newButton->setEnabled(true);

}

void Widget::stopThreads()  // принудительная остановка всех потоков
{
    emit stopAll();  // каждый WorkerThread получит сигнал остановиться, остановит свой анализатор текста и вызовет слот quit() своего потока
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // Кодировка
    QTextCodec *coding = QTextCodec::codecForName("utf-8");
    coding->setCodecForLocale(coding);

    //Регистрация своего типа как метатипа для использования в сигналах и слотах
    int id = qRegisterMetaType<TextAnalysis>();
    qDebug() << "Type registered " << id;
}

void Widget::printStringAtList(QString string)
{
    ui->listWidget->addItem(string);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::handleResults0()
{
     qDebug() << "Results0 handled ";
}

//создание списка полных имён файлов, выбранных пользователем
void Widget::on_loadButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);

    if (dialog.exec()){
        QString a;
        fileNames += dialog.selectedFiles();
        ui->listWidget_files->addItem(fileNames.last());
    }

}

// Запуск процесса анализа текстов в дочерних потоках
void Widget::on_findButton_clicked()
{
    if (fileNames.size() != 0){
    // Блокировка кнопок на время поиска слов
    ui->findButton->setEnabled(false);
    ui->loadButton->setEnabled(false);
    ui->newButton->setEnabled(false);

    // Значение счётчика (кол-во искомых слов)
    int number = ui->spinBox->value();

    // Сформированное задание
    qDebug() << "Have to find " << number << " frequent words in " << fileNames.size() << " files: ";
    for (QString file : fileNames)
         qDebug() << file;

        // Создание потока для асинхронного анализа текстов
        addAnalysingThread(fileNames, number);
    }

}

// очистка списка файлов и вывода от предыдущего поиска
void Widget::on_newButton_clicked()
{
    fileNames.clear();
    ui->listWidget_files->clear();
    ui->listWidget->clear();
}
