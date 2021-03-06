#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>

#include "../includes/box.h"
#include "../includes/constants.h"
#include "../includes/waiter.h"
#include "../includes/baker.h"
#include "../includes/datastructs.h"
#include "../includes/PRINT.h"

using namespace std;

//*************************************************
//NOTE:  most of these globals are needed
//by both bakers and waiters, so be sure to include
//externs.h in appropriate cpp implementations
//*************************************************
//the mutexes
mutex mutex_order_inQ;
mutex mutex_order_outQ;

//the condition variable
condition_variable cv_order_inQ;

//when true its time for the baker to quit
bool b_WaiterIsFinished = false;

//where orders are stored
queue<ORDER> order_in_Q;
vector<ORDER> order_out_Vector;

//used to make PRINT statements work properly
mutex printMutex;

//*************************************************
//runs waiter until orders all read and placed
//on order_in_Q then exits
void doWaiter(int id,string fn) {
	Waiter myWaiter(id,fn);
	myWaiter.beWaiter();
}

//takes orders from order_in_Q, processes
//them and then puts them on order_out_Vector
void doBaker(int id) {
	Baker myBaker(id);
	myBaker.beBaker();
}

//prints what is in order_out_Vector
//DO NOT CALL THIS WHEN MULTIPLE THREADS ARE ACCESSING order_out_Vector
void audit_results() {
	std::vector<ORDER>::iterator itOrder;
	std::vector<Box>::iterator itBox;

	int total_donuts = 0;
	int total_orders = 0;
	int total_boxes  = 0;

	//first go through every order
	for (itOrder = order_out_Vector.begin(); itOrder != order_out_Vector.end(); itOrder++) {
		int numDonuts = 0;
		total_orders++;

		//for each order go through all the boxes and add up all the donuts
		for (itBox = (itOrder->boxes).begin(); itBox != (itOrder->boxes).end(); ++itBox) {
			total_boxes++;
			numDonuts += itBox->size();
			total_donuts += itBox->size();
		}

		//if one order was screwed up say so
		if (numDonuts != itOrder->number_donuts)
			PRINT6("ERROR Order", itOrder->order_number, " Expected ", itOrder->number_donuts, " found ", numDonuts);
	}

	PRINT2("Total donuts made   = ", total_donuts);
	PRINT2("Total number boxes  = ", total_boxes);
	PRINT2("Total orders filled = ", total_orders);
}
///////////////////
vector<thread> threads;
void joinThreads(){
    for(thread &t : threads){
        t.join();
    }
    threads.clear();
}
void clean(){
    b_WaiterIsFinished = false;
    while(!order_in_Q.empty()){
        order_in_Q.pop();
    }
    order_out_Vector.clear();
}
void Test(int bakers, string file){
    string  testDescription = "Testing " + to_string(bakers) + " bakers on file: "+file;
    PRINT1(testDescription);
    threads.push_back(thread(doWaiter, 1, file));
    for(int i = 0; i < bakers; i++){
        threads.push_back(thread(doBaker, i));
    }
    joinThreads();
    audit_results();
    clean();
    PRINT1("End Test: "+testDescription + "\n");
}

int main()
{
    for(int i = 1; i< 101; i++){
        Test(i, "in1.txt");//15,5,5
        Test(i, "in2.txt");//40/5/5
        Test(i, "in3.txt");//65/8/5
        Test(i, "in4.txt");//660/60/14
        PRINT1("---------------------------------------------")
    }
	return SUCCESS;
}

