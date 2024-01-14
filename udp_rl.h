/*
 * udp_rl.h
 *
 *  Created on: 12 янв. 2017 г.
 *      Author: real_bastard
 */

#ifndef UDP_RL_H_
#define UDP_RL_H_
#include <stdint.h>
#include <QWidget>
#include <QtNetwork/QUdpSocket>
#include <QMutex>

#define BUF_SIZE	10000





//RX data type
typedef struct {
    int data1;
    int data2;
    int data3;
} udp_data_type;






class ThreadUdp : public QObject {
    Q_OBJECT

public:
    ThreadUdp(int ip);
    ~ThreadUdp();
    void connectToServer(quint16 _port = 12000);
    //	void run();
    void stop();
    int get(udp_data_type* data);
    void send_pack(char* buf,int size);

    int startTime;
    int stopTime;
    int byteCount;
    int ErrorCount;
    int firstError;
    int PacketNum;
    int firstPacket;
    QUdpSocket* udpSocket;
    int counter;
    int rx;

public slots:
    void process();
    void read_sock();
signals:
    void finished();
    void errorUdp(QString errorMessage);
    void UdpPack_rcv(quint8* data);
private:
    bool quit;
    QMutex mutexBuf;
    int usedBufSpace;
    int readPos;
    int writePos;
    quint16 port;
    udp_data_type* dataBuf;
    int ip4;
    QHostAddress currentIP;
};

#endif /* UDP_H_ */
