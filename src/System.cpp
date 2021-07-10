#include "System.h"
#include "PCB.h"
#include"Inactive.h"
#include "SCHEDULE.H"
#include "IOSTREAM.H"
#include"KerSem.h"
//#include "DOS.H"

#include "thread.h"

element* System::first=0;
element* System::last=0;




volatile PCB* System::running=0;
 PCB* System::inactive=0;
Thread * System::mainThread=0;

volatile int System::context_on_demand=0;
InterruptRoutine System::oldtimer=0;
volatile int System::counter=0;
volatile int System::signal=0; //blokira prekide

extern void tick();



void interrupt System::timer(...){


static unsigned pomss,pomsp,pombp;



if(System::context_on_demand==0 && counter>0){System::counter--;  KerSem::call(); tick();  }

if ((context_on_demand==1)||(counter==0)){

asm{
mov pomsp,sp
mov pomss,ss
mov pombp,bp
}


System::running->sp=pomsp;
System::running->ss=pomss;
System::running->bp=pombp;
if ((System::running->status!=PCB::FINISHED)&&(System::running->status!=PCB::BLOCKED)/*&&(System::running->myThread->getId()!=2)*/&&(System::running->myThread->getId()!=1)){
				System::running->status=PCB::READY;
				Scheduler::put((PCB*)running);
			}


System::running=Scheduler::get();


if(System::running==0)System::running=System::inactive;

System::running->status=PCB::READY;



pomsp=System::running->sp;
pomss=System::running->ss;
pombp=System::running->bp;

System::counter=System::running->timeslice;
asm{
mov sp,pomsp
mov ss,pomss
mov bp,pombp
}
PCB::obradisignale();

}

if(System::context_on_demand==0)asm int 60h;
System::context_on_demand=0;

}





void System::add(PCB* pcb){

if(first==0)first=last=new element(pcb);
else{last=last->next=new element(pcb);
}

 }



void System::deleteone(PCB* pcb){

	element *tek=first,*pret=0;
	while(tek){
		if(tek->pcb!=pcb){
			pret=tek;
			tek=tek->next;
		}else{
	tek=tek->next;
	if(!pret)first=tek;
	else pret->next=tek;

		}
	}

}

void System::init(){
	lock

	#ifndef BCC_BLOCK_IGNORE
	oldtimer = getvect(0x8);
	setvect(0x8,&timer);
	setvect(0x60, oldtimer);
	#endif

Inactive * in=new Inactive();
	System::inactive=in->myPCB;
	System::inactive->myThread->start();
	System::mainThread=new Thread(500,1);
	System::mainThread->myPCB->status=PCB::READY;
	System::running=(volatile PCB*)System::mainThread->myPCB;
System::counter=1;

	unlock

}

void System::restore(){
	lock

	#ifndef BCC_BLOCK_IGNORE
	setvect(0x8,oldtimer);
	#endif
	delete System::inactive;

	deleteall();
	KerSem::deletesem();
	unlock
}

void System::deleteall(){
element* pom=first;
while(System::first){
first=first->next;
delete pom;
pom=first;
}
first=last=0;
}

int System::count(){
	int i=0;
	for(element *pom=first;pom!=0;pom=pom->next)i++;
	return i;
}


Thread* System::getThread(ID id){

element* pom=System::first;
while(pom->pcb->id!=id)pom=pom->next;
return pom->pcb->myThread;

 }



 void System::dispatch(){

	 lock
	 	System::context_on_demand=1;
	 	System::timer();
	 	System::context_on_demand=0;
	unlock

 }
