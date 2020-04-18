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
    while(!order_in_Q.empty() || !b_WaiterIsFinished){ //stop once waiter stops and no orders left in Q
        unique_lock<mutex> lockIn(mutex_order_inQ);

        while(order_in_Q.empty() && !b_WaiterIsFinished) //wait for orders
            cv_order_inQ.wait(lockIn);

        if(!order_in_Q.empty()){ // bake order if there
            bake_and_box(order_in_Q.front());
            unique_lock<mutex> lockOut(mutex_order_outQ);
            order_out_Vector.push_back(order_in_Q.front());
            order_in_Q.pop(); //I hate how pop doesnt return the object that got popped
        }
        cv_order_inQ.notify_all();
    }
}
