#include "PCB.h"
#include "DOS.H"
#include "System.h"
#include"Schedule.h"

ID PCB::idd=1;
int PCB::globalblocked [15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
PCB::PCB( StackSize size,Time time,Thread *thread){
stacksize=size;
timeslice=time;
stack=0;
ss=bp=sp=0;
id=idd++;
System::add(this);
myThread=thread;
first=last=0;
timetowait=0;
for(int i=0;i<16;i++){
	blocked[i]=0;
	arrived[i]=0;
	firsthandler[i]=0;
	lasthandler[i]=0;
	this->registerHandler(0,signal0);
}
if(id==1){
	parent=0;
}else{
parent=(PCB*)System::running;
copy(this,parent);
}
}
void PCB::deleteHandlers(){
	node* tmp = 0;

		for(int i =0;i<16;i++)
			while (firsthandler[i] != 0) {
					tmp = firsthandler[i];
					firsthandler[i] = firsthandler[i]->next;
					delete tmp;
				}
}


void PCB::deleteblocked(){
element * pom=first;
while(first){
	first=first->next;
	delete pom;
	pom=first;
}
first=last=0;
}



PCB::~PCB(){
	lock
    delete stack;
	deleteHandlers();
		deleteblocked();
	unlock
}



void PCB::wrapper(){

	System::running->myThread->run();

	lock
	System::running->status=PCB::FINISHED;
	((PCB*)System::running)->deblock();
	if(System::running->parent!=0)System::running->parent->signal(1);
			if(System::running!=0)System::running->myThread->signal(2);
	dispatch();

	unlock

}



void PCB::initstack(){

unsigned cs,ip;
		stack=new unsigned[stacksize];
		#ifndef BCC_BLOCK_IGNORE
		stack[stacksize-1]=0x200;
		cs=FP_SEG(&wrapper);
		ip=FP_OFF(&wrapper);
		sp = FP_OFF(stack+stacksize-12);
		ss = FP_SEG(stack+stacksize-12);
		#endif

		stack[stacksize-2] = cs;
		stack[stacksize-3] = ip;

		bp=sp;

}

volatile void PCB::deblock(){

while(first!=0){
first->pcb->status=PCB::READY;
Scheduler::put(first->pcb);

last=first->next;
first->next=0;
first=last;
}
first=last=0;

}



void PCB::block(PCB * pcb){

if(first==0)first=last=new element(pcb);
else last=last->next=new element(pcb);

}


int PCB::count(){
	int i=0;
	for(element *pom=first;pom!=0;pom=pom->next)i++;
	return i;
}


void PCB::signal(SignalId signal){
lock

    System::running->arrived[signal]++;

unlock
}


void PCB::registerHandler(SignalId signal, SignalHandler handler){
lock

if(firsthandler[signal]==0)firsthandler[signal]=lasthandler[signal]=new node(handler);
else lasthandler[signal]=lasthandler[signal]->next=new node(handler);

unlock
}


void PCB::unregisterAllHandlers(SignalId id){
lock

node *stari;
while (firsthandler[id]) {
stari = firsthandler[id];
firsthandler[id] = firsthandler[id]->next;
delete stari;
}
firsthandler[id]=lasthandler[id]=0;
unlock
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
lock

if(hand1==0 || hand2==0 )return;

node *a=firsthandler[id],*b=firsthandler[id],*tmp;

while(a->fun!=hand1)a=a->next;
while(a->fun!=hand2)b=b->next;
if(a!=0 && b!=0){
	tmp->fun = a->fun;
	a->fun = b->fun;
    b->fun = tmp->fun;

}
unlock
}
void PCB::blockSignal(SignalId signal){
lock
blocked[signal]=1;
unlock
}

void PCB::blockSignalGlobally(SignalId signal){
lock
globalblocked[signal]=1;
unlock
}

void PCB::unblockSignal(SignalId signal){
lock
blocked[signal]=0;
unlock
}

void PCB::unblockSignalGlobally(SignalId signal){
lock
globalblocked[signal]=0;
unlock
}


void PCB::obradisignale(){

lock


for(int i=0;i<16;i++)
	if(System::running->arrived[i]!=0 && System::running->blocked[i]==0 && PCB::globalblocked[i]==0){
for(node * temp=System::running->firsthandler[i];temp!=0;temp=temp->next){

	while(System::running->arrived[i]-- !=0)temp->fun();
	System::running->arrived[i]=0;
}

}


unlock

}



void PCB::copy(PCB* primalac,PCB* davalac){

for(int i=0;i<16;i++){
	primalac->arrived[i]=davalac->arrived[i];
	primalac->blocked[i]=davalac->blocked[i];

}

}

void PCB::signal0(){

System::running->status=PCB::FINISHED;
if(System::running->parent!=0)System::running->parent->signal(1);
System::running->myThread->signal(2);
dispatch();

}
