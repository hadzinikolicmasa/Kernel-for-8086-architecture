/*
 * IVTEntry.cpp
 *
 *  Created on: Sep 8, 2020
 *      Author: OS1
 */

#include "IVTEntry.h"
#include"DOS.h"
#include"Event.h"


IVTEntry** IVTEntry::table=new IVTEntry*[255];


IVTEntry::IVTEntry(IVTNo ivtNo,InterruptRoutine routine) {
lock
 table[ivtNo]=this;
	this->ivtNo=ivtNo;
#ifndef BCC_BLOCK_IGNORE
	old = getvect(ivtNo);
	setvect(ivtNo, routine);
	#endif
kernelEv=0;
unlock
}

IVTEntry::~IVTEntry() {

#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo,old);
	#endif

}

void IVTEntry::signal(){
	lock
	kernelEv->signal();
	unlock


}

void IVTEntry::set(KernelEv* kernel){

kernelEv=kernel;

}
