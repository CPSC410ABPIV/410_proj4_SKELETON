#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

/**helper*/
void addDonutsToBox(int donuts, Box &box){
    for(int i = 0; i < donuts; i++){
        DONUT donut;
        box.addDonut(donut);
    }
}

void Baker::bake_and_box(ORDER &anOrder) {
	for(int i = 0; i < anOrder.number_donuts/DOZEN; i++){ //make full boxes
	    Box box;
        addDonutsToBox(DOZEN, box);
        anOrder.boxes.push_back(box);
	}
	if(anOrder.number_donuts%DOZEN != 0){// add unfull box of donuts if needed
        Box box;
	    addDonutsToBox(anOrder.number_donuts%DOZEN, box);
	    anOrder.boxes.push_back(box);
	}
}


void Baker::beBaker() {
    while(!b_WaiterIsFinished || !order_in_Q.empty()){
        unique_lock<mutex> order_inQ_lock(mutex_order_inQ);
        while(order_in_Q.empty()){//wait for more orders if needed
            cv_order_inQ.wait(order_inQ_lock);
        }
        ORDER orderToFill;
        orderToFill = order_in_Q.front(); order_in_Q.pop();
        order_inQ_lock.unlock();

        bake_and_box(orderToFill);

        lock_guard<mutex> order_outQ_lock(mutex_order_outQ);
        order_out_Vector.push_back(orderToFill);
    }
}
