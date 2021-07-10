/*
 * Inactive.h
 *
 *  Created on: Aug 24, 2020
 *      Author: OS1
 */

#ifndef INACTIVE_H_
#define INACTIVE_H_
#include"Thread.h"

class Inactive:public Thread {
public:
	Inactive():Thread(500,1){}
	virtual void run();
	void start();
	friend class System;
};

#endif /* INACTIVE_H_ */
