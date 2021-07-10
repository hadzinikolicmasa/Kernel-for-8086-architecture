/*
 * Semaphor.cpp
 *
 *  Created on: Sep 5, 2020
 *      Author: OS1
 */

#include "Semaphor.h"
#include"KerSem.h"

int Semaphor::val()const{
	return myImpl->val();
}


int Semaphor::wait (Time m){
	return myImpl->wait(m);
}

int Semaphor::signal(int i){
	return myImpl->signal(i);

}



Semaphor::Semaphor(int i){
	myImpl=new KerSem(i);
}

Semaphor::~Semaphor(){
	delete myImpl;
}
