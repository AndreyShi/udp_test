#include "udp_rl.h"
#include "board_data_type.h"

typedef struct {
	uint32_t pac_type;
	int8_t data[1024 - 4];	//������ � ����������
} pack_type;

class Track {
public:
    Track(ThreadUdp* udp);
    void stop();
    void moveFwd();
    void moveRev();
private:
    ThreadUdp* _udp;
};
