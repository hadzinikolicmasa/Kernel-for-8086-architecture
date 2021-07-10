/*
 * KernelEv.cpp
 *
 *  Created on: Sep 8, 2020
 *      Author: OS1
 */

#include "KernelEv.h"
#include"PCB.h"
#include"Schedule.h"
#include"IVTEntry.h"

KernelEv::KernelEv(IVTNo ivtNo) {
lock
creator=(PCB*)System::running;
blocked=0;
IVTEntry::table[ivtNo]->set(this);
value=0;
unlock
}

KernelEv::~KernelEv() {
	lock
creator=0;
blocked=0;
unlock
}



/*void KernelEv::wait(){
	lock
	if ((System::running==creator)&&(blocked==0)){
		blocked=(PCB*)System::running;
		System::running->status=PCB::BLOCKED;
		dispatch();
	}
	unlock
}

void KernelEv::signal(){
	lock
	if (blocked!=0){
		blocked=0;
		creator->status=PCB::READY;
		Scheduler::put(creator);
		dispatch();

	}

unlock
}*/
void KernelEv::wait(){
	lock
	if ((PCB*)System::running==creator){
		if (value==1)value=0;
		else{
			blocked=creator;
			System::running->status=PCB::BLOCKED;
			dispatch();
		}
	}
	unlock
}

void KernelEv::signal(){
	lock
if(blocked!=0){
	creator->status=PCB::READY;
	Scheduler::put(creator);

}
else value=1;

	dispatch();
	unlock
}
