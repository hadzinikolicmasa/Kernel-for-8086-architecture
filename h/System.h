#ifndef SYSTEM_H_
#define SYSTEM_H_


#include "DOS.H"
#include"Inactive.h"

typedef void interrupt (*InterruptRoutine)(...);

#define lock asm{pushf; cli;}

#define unlock asm {popf;}


class PCB;
class Thread;

struct element {
				 PCB *pcb;
				 element * next;
				 element(PCB * p):pcb(p),next(0){}
 };




class System {

private:
	static	element *first,*last;

	static volatile int context_on_demand;
	static volatile int counter;


public:

	static Thread* getThread(int id);
    static void deleteone(PCB* pcb);

	static void add(PCB* pcb);
    static void dispatch();

    static volatile int signal;

    static void deleteall();
	static void init();
	static void restore();
    static int count();
	static void interrupt timer(...);

	static volatile PCB* running;
	static  PCB* inactive;
	static Thread* mainThread;



	static  InterruptRoutine oldtimer;




};

#endif /* SYSTEM_H_ */
