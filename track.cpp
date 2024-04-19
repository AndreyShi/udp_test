#include "track.h"

Track::Track(ThreadUdp* udp)
{
    _udp = udp;
    memset(&tmp_motor,0,sizeof(udp_motor_pack_type));
}

void Track::stop()
{
    uint32_t cmd = 0x20000000;
    _udp->send_pack((char*)&cmd,4);
}
void Track::moveFwd()
{
    uint32_t cmd = 0x20000002;
    _udp->send_pack((char*)&cmd,4);
}
void Track::moveRev()
{
    uint32_t cmd = 0x20000003;
    _udp->send_pack((char*)&cmd,4);
}

void Track::track_pwr1()
{
    tmp_motor.type =  0x10000002;
    if(tmp_motor.data[0] == 0)
        {tmp_motor.data[0] = 1;}
    else
        {tmp_motor.data[0] = 0;}
    _udp->send_pack((char*)&tmp_motor,sizeof(udp_motor_pack_type));
}

void Track::track_pwr2()
{
    tmp_motor.type =  0x10000002;
    if(tmp_motor.data[1] == 0)
        {tmp_motor.data[1] = 1;}
    else
        {tmp_motor.data[1] = 0;}
    _udp->send_pack((char*)&tmp_motor,sizeof(udp_motor_pack_type));
}

void Track::track_pwr3()
{
    tmp_motor.type =  0x10000002;
    if(tmp_motor.data[2] == 0)
        {tmp_motor.data[2] = 1;}
    else
        {tmp_motor.data[2] = 0;}
    _udp->send_pack((char*)&tmp_motor,sizeof(udp_motor_pack_type));
}


void Track::recieve_data()
{
    memcpy(&data,&_udp->udp_Pack.data,sizeof(track_data_type));
}

void Track::poll()
{
    _udp->send_pack((char*)"\x00\x00\x00\x00", 4);
}
