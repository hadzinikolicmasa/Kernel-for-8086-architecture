
#ifndef PCB_H_
#define PCB_H_
#include "thread.h"
#include"System.h"
#include"KerSem.h"


class Thread;

class PCB {
private:
	static ID idd;

	ID id;

	Time timeslice;
    Time timetowait;
	enum Stat{BLOCKED,RUNNING,FINISHED,READY};
	Stat status;

	unsigned int ss,sp,bp;
	unsigned int* stack;

	StackSize stacksize;
	Thread* myThread;
    element *first,*last;

public:

   void block(PCB *pcb);
   volatile void deblock();
   int count();

	PCB(StackSize stackSize, Time timeSlice,Thread* n);

	~PCB();

	static void wrapper();
	void deleteblocked();
	void initstack();

	friend class Thread;
	friend class Lista;
	friend class System;
	friend class Inactive;
    friend class KerSem;
    friend class KernelEv;


    ////////////////////////////////signali//////////////////////////////

            struct node {
            	             SignalHandler fun;
            				 node * next;
            				 node(SignalHandler p):fun(p),next(0){}
             };
             int arrived [15];
             int blocked [15];
             node *firsthandler[15];
             node *lasthandler[15];
             static int globalblocked [15];
             PCB * parent;

             void deleteHandlers();
             static void obradisignale(); //za running nit
             static void signal0();
             void copy(PCB* primalac,PCB* davalac);

             void signal(SignalId signal);
             void registerHandler(SignalId signal, SignalHandler handler);
             void unregisterAllHandlers(SignalId id);
             void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

             void blockSignal(SignalId signal);
             static void blockSignalGlobally(SignalId signal);
             void unblockSignal(SignalId signal);
             static void unblockSignalGlobally(SignalId signal);
};

#endif /* PCB_H_ */
