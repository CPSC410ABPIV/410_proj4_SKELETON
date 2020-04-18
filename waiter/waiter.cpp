#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
    if(myIO.getNext(anOrder) == SUCCESS){
        return SUCCESS; //an order contains new order
    }else{
        return NO_ORDERS; //file io error
    }
}

void Waiter::beWaiter() {
    ORDER order;//holds orders as the loop gets them from fileio
    while(getNext(order) == SUCCESS){
        unique_lock<mutex> lck(mutex_order_inQ);
        order_in_Q.push(order);
        lck.unlock();
        cv_order_inQ.notify_all();
    }
    //signals baker(s) using cv_order_inQ
    unique_lock<mutex> lck(mutex_order_inQ);
    b_WaiterIsFinished = true;
    cv_order_inQ.notify_all();
}

