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
    
    track_data_type data;
private:
    ThreadUdp* _udp;
};
#endif