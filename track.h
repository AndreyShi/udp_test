#include "udp_rl.h"

class Track {
public:
    Track(ThreadUdp* udp);
    void stop();
    void moveFwd();
    void moveRev();
private:
    ThreadUdp* _udp;
};
