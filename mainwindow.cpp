#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    cmd_en = 0;
    ui->setupUi(this);
    int ip=11; //192.168.1.xxx
    handlerThread
        = new QThread(); // создается ни с чем не связанный объект потока (котоырй пока что вообще не работает)
    _udp = new ThreadUdp(ip); // создается объект, который поместится в поток
    //connect(_udp, SIGNAL(errorUdp(QString)), this, SLOT(get_error(QString)));
    connect(handlerThread, SIGNAL(started()), _udp, SLOT(process()));
    connect(_udp, SIGNAL(finished()), handlerThread, SLOT(quit()));
    connect(_udp, SIGNAL(finished()), _udp, SLOT(deleteLater()));
    connect(handlerThread, SIGNAL(finished()), handlerThread, SLOT(deleteLater()));
    int port=10000; //udp port
    _udp->connectToServer(port);
    _udp->moveToThread(handlerThread); // объект помещается в поток
    handlerThread
        ->start(); // поток запускается (теперь наш объект обрабатывает всякие сообщения в отдельном потоке)

    //rx timer
    connect(&timerRead, SIGNAL(timeout()), this, SLOT(timerExept()));
    timerRead.start(10); //10mS
    cmd_en = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//timer  10mS
void MainWindow::timerExept(void)
{
    udp_data_type data;
    int count = 0;  //сколько принято посылок
    while (_udp->get(&data) != -1) {
        count++;
    }
    if (count == 0)
        return;
    QString str=QString::number(data.data1); //число в строку
    ui->label->setText(str);
}


void MainWindow::on_pushButton_pressed()
{
    simple_conf.conf1=ui->lineEdit->text().toInt();
    buff_tosend = ui->lineEdit->text().toInt();
    send_conf();
}

void MainWindow::send_conf(void)
{
    if (cmd_en == 1)
    {_udp->send_pack((char *) &buff_tosend, sizeof(buff_tosend));}
}

