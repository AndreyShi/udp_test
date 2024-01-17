/*
 * udp_rl.c
 *
 *  Created on: 12 янв. 2017 г.
 *      Author: real_bastard
 */
#include "udp_rl.h"
#include <windows.h>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QtCore>



/*
 * класс
 */
ThreadUdp::ThreadUdp(int ip)
{
    ip4 = ip;
    port = 10000;
    usedBufSpace = 0;
    readPos = 0;
    writePos = 0;
    dataBuf = new udp_data_type[BUF_SIZE];
    rx = 0;
}

ThreadUdp::~ThreadUdp()
{
    delete dataBuf;
}

void ThreadUdp::connectToServer(quint16 _port)
{
    //QMutexLocker locker(&mutex);
    quit = false;
    usedBufSpace = 0;
    readPos = 0;
    writePos = 0;
    port = _port;
}

void ThreadUdp::stop()
{
    quit = true;
    udpSocket->close();
    emit finished();
}

void ThreadUdp::process()
{
    int OptVal = 1024 * 64 * 1024;
    int OptLen = sizeof(OptLen);
    udpSocket=new QUdpSocket();
    setsockopt(udpSocket->socketDescriptor(), SOL_SOCKET, SO_RCVBUF, (char *) &OptVal, OptLen);
    udpSocket->bind(QHostAddress::Any, port + 1);
    firstPacket = -1;
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(read_sock()));
}

int ThreadUdp::get(udp_data_type *data)
{
    int ret;

    mutexBuf.lock();
    if (usedBufSpace == 0) {
        ret = -1;
    } else {
        *data = dataBuf[readPos % BUF_SIZE];
        readPos++;
        usedBufSpace--;
        ret = usedBufSpace;
    }
    mutexBuf.unlock();
    return ret;
}

void ThreadUdp::send_pack(char *buf, int size)
{
    udpSocket->writeDatagram(buf, size, currentIP, port);
}


void ThreadUdp::read_sock(void){
    int size;

    quint16 localPort = 0;
    quint8 rxData[8000];

    if (udpSocket->hasPendingDatagrams() == true) {
        size = udpSocket->readDatagram((char *) rxData, 2048, &currentIP, &localPort);
        if ((localPort != port) && (localPort != port + 1)) {
        } else {
            if (size == sizeof(udp_data_type)) {
                udp_data_type *udp_packet = (udp_data_type *) rxData;
                mutexBuf.lock();
                usedBufSpace++;
                dataBuf[writePos % BUF_SIZE] = *udp_packet;
                writePos++;
                mutexBuf.unlock();
            }
        }
    }
}
