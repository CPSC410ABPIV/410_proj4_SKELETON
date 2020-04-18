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
    ORDER order;
    while(getNext(order) == SUCCESS){
        unique_lock<mutex> order_inQ_lock(mutex_order_inQ);
        order_in_Q.push(order);
        order_inQ_lock.unlock();
        cv_order_inQ.notify_all();
    }
    b_WaiterIsFinished = true;
}

