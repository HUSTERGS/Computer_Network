//
// Created by samuel on 12/11/19.
//

#ifndef STOPWAIT_TCPRECEIVER_H
#define STOPWAIT_TCPRECEIVER_H
#include "RdtReceiver.h"
class TCPReceiver :public RdtReceiver {
private:
    int expectSequenceNumberRcvd;
    Packet lastAckPkt;
public:
    TCPReceiver();
    virtual ~TCPReceiver();

public:
    void receive(const Packet &packet);
};
#endif //STOPWAIT_TCPRECEIVER_H
