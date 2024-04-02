#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    cmd_en = 0;
    ui->setupUi(this);
    int ip=11; //192.168.1.xxx
    handlerThread = new QThread(); // создается ни с чем не связанный объект потока (котоырй пока что вообще не работает)
    _udp = new ThreadUdp(ip); // создается объект, который поместится в поток
    //connect(_udp, SIGNAL(errorUdp(QString)), this, SLOT(get_error(QString)));
    connect(handlerThread, SIGNAL(started()), _udp, SLOT(process()));
    connect(_udp, SIGNAL(finished()), handlerThread, SLOT(quit()));
    connect(_udp, SIGNAL(finished()), _udp, SLOT(deleteLater()));
    connect(handlerThread, SIGNAL(finished()), handlerThread, SLOT(deleteLater()));
    connect(ui->pushButton_trackFwd,&QPushButton::clicked,this,&MainWindow::pb_trackFwd);
    connect(ui->pushButton_trackRev,&QPushButton::clicked,this,&MainWindow::pb_trackRev);
    connect(ui->pushButton_trackStop,&QPushButton::clicked,this,&MainWindow::pb_trackStop);
    int port=10000; //udp port
    _udp->connectToServer(port);
    _udp->moveToThread(handlerThread); // объект помещается в поток
    handlerThread->start(); // поток запускается (теперь наш объект обрабатывает всякие сообщения в отдельном потоке)
    ershik = new Track(_udp);
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
    //polling board every 1 sec
    static int time1sec;
    time1sec += 10;
    if(time1sec % 100 == 0)
        { 
            ershik->recieve_data();
            updata_data();
        }
    else if(time1sec >= 1000)
    {
        ershik->poll();
        time1sec = 0;
    }

    udp_data_type data;
    int count = 0;  //сколько принято посылок
    while (_udp->get(&data) != -1) {
        count++;
    }
    if (count == 0)
        { return;}
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

void MainWindow::updata_data(void)
{
     QScrollBar *sb = ui->textEdit->verticalScrollBar();
     QString dummy;

     ui->label->setText(dummy.sprintf("PacketCounter: %d",_udp->PacketCounter));
     int save_sc = sb->value();
     ui->textEdit->setText(dummy.sprintf(
    "time: %d\n"
    "Vin_min: %.3f\n"

    "gX: %.3f\n"
    "gY: %.3f\n"
    "gZ: %.3f\n"

    "trackState[0]: %d\n"
    "trackState[1]: %d\n"
    "trackState[2]: %d\n"
    "BoardState: %d\n"

    "[0]HallTicks: %d\n"
    "[0]cur: %d\n"
    "[0]curLimit %d\n"
    "[0]step: %d\n"
    "[0]pwmLimitFlag: %d\n"

    "[1]HallTicks: %d\n"
    "[1]cur: %d\n"
    "[1]curLimit: %d\n"
    "[1]step: %d\n"
    "[1]pwmLimitFlag: %d\n"

    "[2]HallTicks: %d\n"
    "[2]cur: %d\n"
    "[2]curLimit: %d\n"
    "[2]step: %d\n"
    "[2]pwmLimitFlag: %d\n"

    "scan_speed: %d\n"
    "move_speed: %d\n"
    "scan_step: %d\n"
    "acs: %d\n"

    "kSpeed[0]: %.3f\n"
    "kSpeed[1]: %.3f\n"
    "kSpeed[2]: %.3f\n"

    "kAcs[0]: %.3f\n"
    "kAcs[1]: %.3f\n"
    "kAcs[2]: %.3f\n"

    "cur_limit_mA[0]: %d\n"
    "cur_limit_mA[1]: %d\n"
    "cur_limit_mA[2]: %d\n"

    ,ershik->data.time
    ,ershik->data.Vin_min

    ,ershik->data.gX                              
    ,ershik->data.gY
    ,ershik->data.gZ

    ,ershik->data.trackState[0]
    ,ershik->data.trackState[1]
    ,ershik->data.trackState[2]
    ,ershik->data.BoardState

    ,ershik->data.reg[0].HallTicks
    ,ershik->data.reg[0].cur
    ,ershik->data.reg[0].curLimit           
    ,ershik->data.reg[0].step
    ,ershik->data.reg[0].pwmLimitFlag

    ,ershik->data.reg[1].HallTicks
    ,ershik->data.reg[1].cur
    ,ershik->data.reg[1].curLimit
    ,ershik->data.reg[1].step             
    ,ershik->data.reg[1].pwmLimitFlag

    ,ershik->data.reg[2].HallTicks
    ,ershik->data.reg[2].cur           
    ,ershik->data.reg[2].curLimit
    ,ershik->data.reg[2].step
    ,ershik->data.reg[2].pwmLimitFlag

    ,ershik->data.coef.scan_speed
    ,ershik->data.coef.move_speed
    ,ershik->data.coef.scan_step           
    ,ershik->data.coef.acs

    ,ershik->data.coef.kSpeed[0]
    ,ershik->data.coef.kSpeed[1]          
    ,ershik->data.coef.kSpeed[2]

    ,ershik->data.coef.kAcs[0]
    ,ershik->data.coef.kAcs[1]        
    ,ershik->data.coef.kAcs[2]

    ,ershik->data.coef.cur_limit_mA[0]
    ,ershik->data.coef.cur_limit_mA[1]           
    ,ershik->data.coef.cur_limit_mA[2]
    ));
   sb->setValue(save_sc);
}

void MainWindow::pb_trackFwd(void)
{
    ershik->moveFwd();
}
void MainWindow::pb_trackRev(void)
{
    ershik->moveRev();
}
void MainWindow::pb_trackStop(void)
{
    ershik->stop();
}

