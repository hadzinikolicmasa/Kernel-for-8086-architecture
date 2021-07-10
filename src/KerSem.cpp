/*
 * KerSem.cpp
 *
 *  Created on: Sep 5, 2020
 *      Author: OS1
 */

#include "KerSem.h"
#include"System.h"
#include"PCB.h"
#include"Schedule.h"
#include<iostream.h>

KerSem::sem* KerSem::first=0;
KerSem::sem* KerSem::last=0;


KerSem::KerSem(int i) {
lock
	firstblocked=lastblocked=0;
	countblocked=0;
	value=i;
	KerSem::addsemaphore(this);
unlock
}

KerSem::~KerSem() {
	lock
	deleteall();
	unlock

}

int KerSem::val()const{

	return value;

}

void KerSem::deletesem(){

	 sem* pom=first;
	 while(first){
	 first=first->next;
	 delete pom;
	 pom=first;
	 }
	 first=last=0;
}

void KerSem::deleteall(){

	 element* pom=firstblocked;
	 while(firstblocked){
		 firstblocked=firstblocked->next;
	 delete pom;
	 pom=firstblocked;
	 }
	 firstblocked=lastblocked=0;
}

void KerSem::block(){

	System::running->status=PCB::BLOCKED;
	countblocked++;
	dispatch();


}

void KerSem::unblock(){
	if(firstblocked!=0){

		firstblocked->pcb->status=PCB::READY;
		Scheduler::put(firstblocked->pcb);
    element *tek=firstblocked;
		firstblocked=firstblocked->next;
delete tek;
	}
}

int KerSem::wait(Time maxTimeToWait){
lock


if(--value>=0){
unlock
return 1;

}else{

if(maxTimeToWait>0){ //sa vremenom

	System::running->timetowait=maxTimeToWait;
    add((PCB*)System::running,0);

    block();
    unlock

    return 0;

}
else if(maxTimeToWait==0){ //bez vremena

	    System::running->timetowait=0;
	    add((PCB*)System::running,1);

	    block();
	    unlock

	    return 1;
}
}

unlock
return 1;
}



int KerSem::signal(int n){
lock

if(n<0){
	unlock
	return n;
}

if(n>0){//n niti odblokiraj

		value+=n;
	int i;

	for(i=n;i>0 && countblocked>0;i--){unblock(); countblocked--;}

	unlock
	return n-i;


}else if(value++<0) { //standardno

 if(countblocked>0){
	unblock();
	countblocked--;
}

}

unlock
return 0;


}



 void KerSem::addsemaphore(KerSem *k){

	if(KerSem::first==0)KerSem::first=KerSem::last=new sem(k);
		else KerSem::last=KerSem::last->next=new sem(k);

}

 void KerSem::add(PCB* pcb,int status){

  	if(firstblocked==0)
  	{
  	firstblocked=lastblocked=new element(pcb);
  	firstblocked->status=status;
  	}
  	else {
  		lastblocked=lastblocked->next=new element(pcb);
  		lastblocked->status=status;
  	}

  }


void KerSem::deleteone(PCB* pcb){
element *tek=firstblocked,*pret=0,*stari=0;
while(tek){
	if(tek->pcb!=pcb){
		pret=tek;
		tek=tek->next;
	}else{
stari=tek;
tek=tek->next;
if(!pret)firstblocked=tek;
else pret->next=tek;
delete stari;

	}
}

	countblocked--;
}


void KerSem::call(){
lock
	 for(KerSem::sem* pom=KerSem::first;pom!=0;pom=pom->next)
	  pom->kersem->passed();

unlock
}

void KerSem::passed(){

	element* pom,*first=firstblocked;

for(pom=firstblocked;pom!=0;pom=pom->next)
	if(pom->status==0)
	pom->pcb->timetowait--;


pom=firstblocked;

while(first!=0){
 first=first->next;
 if((pom->status==0)&&(pom->pcb->timetowait==0)){
 pom->pcb->status=PCB::READY;
  Scheduler::put(pom->pcb);
  value++;
  deleteone(pom->pcb);
 }
 pom=first;
}

}

