#include "thread.h"
#include "PCB.h"
#include "Schedule.h"
#include "System.h"
#include "IOSTREAM.H"


Thread::Thread (StackSize stackSize, Time timeSlice){
lock
	myPCB=new PCB(stackSize, timeSlice, this);
unlock
}

void Thread::start(){
	lock

	this->myPCB->status=PCB::READY;
	Scheduler::put(this->myPCB);
    myPCB->initstack();

	unlock
}

ID Thread::getId(){

	 return myPCB->id;

 }

ID Thread::getRunningId(){

	return System::running->id;

}


Thread * Thread::getThreadById(ID id){

	 return System::getThread(id);

 }


void Thread::waitToComplete(){
	lock


	if ((myPCB!=(PCB*)System::running)&&(myPCB->myThread->getId()!=1)/*&&(myPCB->myThread->getId()!=2)*/&&(myPCB->status!=PCB::FINISHED)){

					System::running->status=PCB::BLOCKED;
					myPCB->block((PCB*)System::running);
					dispatch();

		}


	unlock
}


Thread::~Thread(){
lock

	waitToComplete();
	System::deleteone(myPCB);
	delete myPCB;

unlock
}


void dispatch (){

	System::dispatch();

}

void Thread::signal(SignalId signal){
	myPCB->signal(signal);

}
void Thread::registerHandler(SignalId signal, SignalHandler handler){
	myPCB->registerHandler(signal,handler);
}
void Thread::unregisterAllHandlers(SignalId id){
	myPCB->unregisterAllHandlers(id);
}
void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	myPCB->swap(id,hand1,hand2);
}
void Thread::blockSignal(SignalId signal){
	myPCB->blockSignal(signal);
}
void Thread::blockSignalGlobally(SignalId signal){
	PCB::blockSignalGlobally(signal);
}
void Thread::unblockSignal(SignalId signal){
	myPCB->unblockSignal(signal);
}
void Thread::unblockSignalGlobally(SignalId signal){
	PCB::unblockSignalGlobally(signal);
}



