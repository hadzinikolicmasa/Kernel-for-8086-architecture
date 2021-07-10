/*
 * KerSem.h
 *
 *  Created on: Sep 5, 2020
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_
#include"System.h"


typedef unsigned int Time;




class KerSem {
public:
	KerSem (int init=1);

	 ~KerSem ();

	int wait (Time maxTimeToWait);

	void block();

	void unblock();

	int signal(int n=0);

	void add(PCB* pcb,int status);

    void deleteone(PCB* pcb);

     void passed();

     static void addsemaphore(KerSem *k);

     static void call();

	 int val () const;

	 static void deletesem();

private:
	 void deleteall();
	 struct element {
	 				 PCB *pcb;
	 				 element * next;
	 				 element(PCB * p):pcb(p),next(0){}
	 				 int status;
	  };

element *firstblocked,*lastblocked;

struct sem{
	                 KerSem *kersem;
					 sem * next;
					 sem(KerSem * p):kersem(p),next(0){}

};

static sem *first,*last;

int countblocked;
int value;

};





#endif /* KERSEM_H_ */
