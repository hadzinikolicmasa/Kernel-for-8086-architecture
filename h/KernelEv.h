/*
 * KernelEv.h
 *
 *  Created on: Sep 8, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_
#include "Event.h"
#include"System.h"
#include"IVTEntry.h"

#define PREPAREENTRY(ivtp,flag)\
	void interrupt routine##ivtp (...);\
	IVTEntry entry##ivtp(ivtp,routine##ivtp);\
	void interrupt routine##ivtp(...){\
		entry##ivtp.signal();\
		if(flag==1)entry##ivtp.old();\
	}

class KernelEv {
public:

    void wait();
    void signal();

	KernelEv(IVTNo ivtNo);
	virtual ~KernelEv();

private:
	PCB* creator;
	PCB* blocked;
int value;
};

#endif /* KERNELEV_H_ */
