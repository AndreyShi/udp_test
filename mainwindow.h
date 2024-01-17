#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include "udp_rl.h"
typedef struct
{
    uint16_t		conf1;
    uint16_t		conf2;
    uint16_t		conf3;
}conf_t;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_pressed();
    void timerExept(void);
private:
    Ui::MainWindow *ui;
    ThreadUdp* _udp;
    QThread *handlerThread;
    QTimer timerRead;
    int cmd_en;
    conf_t simple_conf;
    uint32_t buff_tosend;
    void send_conf(void);
};
#endif // MAINWINDOW_H
