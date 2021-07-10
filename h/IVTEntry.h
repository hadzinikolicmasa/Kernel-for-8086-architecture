/*
 * IVTEntry.h
 *
 *  Created on: Sep 8, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include"KernelEv.h"

typedef void interrupt (*InterruptRoutine)(...);



class IVTEntry {
public:

	IVTEntry(IVTNo ivtNo,InterruptRoutine routine);
	 ~IVTEntry();

	InterruptRoutine old;
    void signal();
	IVTNo ivtNo;
	void set(KernelEv* kernel);
	KernelEv * kernelEv;
	static IVTEntry **table;

};

#endif /* IVTENTRY_H_ */
