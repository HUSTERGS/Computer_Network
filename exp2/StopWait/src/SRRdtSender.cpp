//
// Created by samuel on 12/10/19.
//

#include <assert.h>
#include "SRRdtSender.h"
#include "Global.h"
bool SRRdtSender::getWaitingState() {
    return waitingState;
}

SRRdtSender::SRRdtSender():base(1), nextseqnum(1), waitingState(false){}
SRRdtSender::~SRRdtSender(){}

bool SRRdtSender::send(const struct Message & message) {
    if (packges.size() >= GBN_WINDOW_SIZE) {
        return false;
    }
    Packet * packet = new Packet();
    packet->acknum = -1; // 忽略该字段
    packet->checksum = 0;
    packet->seqnum = nextseqnum;
    memcpy(packet->payload, message.data, sizeof(message.data));
    packet->checksum = pUtils->calculateCheckSum(*packet);
    packges.push_back(packet);
    // 同时记录状态
    packetState.push_back(UNKWON);
    pUtils->printPacket("发送方发送报文", *packet);
    pns->startTimer(SENDER, Configuration::TIME_OUT,packet->seqnum);			//启动发送方定时器
    nextseqnum++;
    nextseqnum %= MAX_SEQ;
    assert(nextseqnum < MAX_SEQ);
    waitingState = packges.size() >= GBN_WINDOW_SIZE;
    pns->sendToNetworkLayer(RECEIVER, *packet);								//调用模拟网络环境的sendToNetworkLayer，通过网络层发送到对方
    packet = nullptr;
    return true;
}

void SRRdtSender::receive(const struct Packet & ackPkt) {
    cout << "收到ack = " << ackPkt.acknum << endl;
    int checkSum = pUtils->calculateCheckSum(ackPkt);
    if (checkSum == ackPkt.checksum) {
        if (((nextseqnum > base) && (ackPkt.acknum >= base) && (ackPkt.acknum < nextseqnum)) || ((nextseqnum < base) && ((ackPkt.acknum < nextseqnum) || (ackPkt.acknum >= base)))) {
            // 落在当前窗口
            // 停止计时
            cout << "落在当前窗口之内" << endl;
            pns->stopTimer(SENDER, ackPkt.acknum);
            int index = 0;
            if (ackPkt.acknum >= base) {
                // 正常情况
                index = ackPkt.acknum - base;
            } else {
                // 在边界上
                index = MAX_SEQ - base + ackPkt.acknum;
                // 假设 base = 2 ^ k -1，并且收到ack = 1，那么应该是第3个包, 即2 ^ k -1, 0, 1
            }
            // 更改对应的包的状态
            packetState[index] = RECEIVED;
//            cout << "收到第" << index << "个ack" << endl;
            if (ackPkt.acknum == base) {
                // 如果等于base的话
                cout << "等于base" << endl;
                while(packetState.size() > 0 && packetState.at(0) == RECEIVER) {
                    // 不断去除已经标记为收到的包
                    packetState.erase(packetState.begin());
                    packges.erase(packges.begin());
                    cout << "清除一个标记为收到的包" << endl;
                    base++;
                }
                // 移动窗口开始位置
                base %= MAX_SEQ;
                cout << "此时base更新为 " << base << endl;
                // 理论上base 应该始终小于等于nextseqnum
//                assert(base <= nextseqnum);
            }
        } else {
            cout << "没有落在当前窗口 ack = " << ackPkt.acknum << " base = " << base << " nextseqnum = " << nextseqnum << endl;
        }
    }
    waitingState = packges.size() >= GBN_WINDOW_SIZE;
}



void SRRdtSender::timeoutHandler(int seqNum) {
    // 需要通过seqNum找到对应的序号
    int index = 0;
    if (seqNum >= base) {
        // 如果是正常情况超时
        index = seqNum - base;
    } else {
        // 如果是在边界上
        index = MAX_SEQ - base + seqNum;
        // 假设此时 base = 2 ^ k - 1, seqNum = 1, 那么就是第三个包超时了
    }
    // 找到对应的包并发送，并且重启计时器
    // 理论上找到的包和序号应该对应的上
    assert(packges.at(index)->seqnum == seqNum);
    pns->sendToNetworkLayer(RECEIVER, *packges.at(index));
    pns->startTimer(SENDER, Configuration::TIME_OUT,seqNum);
}