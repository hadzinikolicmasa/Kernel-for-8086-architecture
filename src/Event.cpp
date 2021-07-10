/*
 * Event.cpp
 *
 *  Created on: Sep 8, 2020
 *      Author: OS1
 */

#include "Event.h"
#include"KernelEv.h"

Event::Event(IVTNo ivtNo) {

myImpl=new KernelEv(ivtNo);

}

Event::~Event() {

delete myImpl;

}

void Event::wait(){
	myImpl->wait();
}


void Event::signal(){
	myImpl->signal();
}
