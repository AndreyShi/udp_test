#ifndef TRACK_H_
#define TRACK_H_
#include "udp_rl.h"
#include "board_data_type.h"

class Track {
public:
    Track(ThreadUdp* udp);
    void stop();
    void moveFwd();
    void moveRev();
    void recieve_data();
    void poll();

    void track_pwr1();
    void track_pwr2();
    void track_pwr3();
    track_data_type data;
    uint32_t type;
private:
    ThreadUdp* _udp;
    udp_motor_pack_type tmp_motor;
};
#endif