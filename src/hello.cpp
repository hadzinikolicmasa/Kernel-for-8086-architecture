/*
 * hello.cpp
 *
 *  Created on: Aug 24, 2020
 *      Author: OS1
 */

#include "System.h"
#include"Thread.h"

extern int userMain(int,char**);

int main(int argc,char *argv[]){

int os;
System::init();
os=userMain(argc,argv);
System::restore();
return os;


}
