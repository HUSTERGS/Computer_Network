//
// Created by samuel on 12/10/19.
//

#ifndef STOPWAIT_GBNRDTRECEIVER_H
#define STOPWAIT_GBNRDTRECEIVER_H
#include "RdtReceiver.h"
class GBNRdtReceiver :public RdtReceiver {
private:
    int expectSequenceNumberRcvd;
    Packet lastAckPkt;
public:
    GBNRdtReceiver();
    virtual ~GBNRdtReceiver();

public:
    void receive(const Packet &packet);
};
#endif //STOPWAIT_GBNRDTRECEIVER_H
