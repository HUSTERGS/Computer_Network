// StopWait.cpp : 定义控制台应用程序的入口点。
//


#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "StopWaitRdtSender.h"
#include "StopWaitRdtReceiver.h"
#include "GBNRdtReceiver.h"
#include "GBNRdtSender.h"
#include "SRRdtReceiver.h"
#include "SRRdtSender.h"
#include "TCPSender.h"
#include "TCPReceiver.h"
int main(int argc, char* argv[])
{
//	RdtSender *ps = new StopWaitRdtSender();
//	RdtReceiver * pr = new StopWaitRdtReceiver();
//    RdtSender *ps = new GBNRdtSender();
//    RdtReceiver * pr = new GBNRdtReceiver();
//    RdtReceiver * pr = new SRRdtReceiver();
//    RdtSender * ps = new SRRdtSender();
    RdtReceiver * pr = new TCPReceiver();
    RdtSender * ps = new TCPSender();
	pns->setRunMode(0);  //VERBOS模式
	if (TESTING || TESTQR) {
        pns->setRunMode(1);  //安静模式
	}
	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr);
	pns->setInputFile("/home/samuel/桌面/input.txt");
	pns->setOutputFile("/home/samuel/桌面/output.txt");

	pns->start();

	delete ps;
	delete pr;
	delete pUtils;									//指向唯一的工具类实例，只在main函数结束前delete
	delete pns;										//指向唯一的模拟网络环境类实例，只在main函数结束前delete

	return 0;
}

