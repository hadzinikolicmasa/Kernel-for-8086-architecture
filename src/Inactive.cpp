
#include "Inactive.h"
#include"Thread.h"
#include"PCB.h"
#include"System.h"
#include<iostream.h>
#include"KerSem.h"


void Inactive::run(){

	while(1);
}


void Inactive::start(){

	lock
	myPCB->status=PCB::READY;
	myPCB->initstack();
	unlock
}
