#include "track.h"

Track::Track(ThreadUdp* udp)
{
    _udp = udp;
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
