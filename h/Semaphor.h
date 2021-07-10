/*
 * Semaphor.h
 *
 *  Created on: Sep 5, 2020
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_
#include"KerSem.h"
typedef unsigned int Time;
class KernelSem;

class Semaphor
{

public:

Semaphor (int init=1);

virtual ~Semaphor ();

virtual int wait (Time maxTimeToWait);

virtual int signal(int n=0);

int val () const; // Returns the current value of the semaphore

private:

KerSem* myImpl;

int value;

};


#endif /* SEMAPHOR_H_ */
