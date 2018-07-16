#include <Python.h>
#include <thread>
#include "testmultithread.h"
#include <QDebug>

using namespace std;
//function to assign a new thread state for combination of new sub interpreter and thread then acquire gil lock and run code. Release gil lock when done.
void do_stuff_in_thread(PyInterpreterState* interp, int tid)
{
    // acquire the GIL
    PyEval_AcquireLock();
    qDebug() << "Acquire lock";

    // create a new thread state for the the sub interpreter interp
    PyThreadState* ts = PyThreadState_New(interp);
    qDebug() << "Create a new state" << tid;

    // make ts the current thread state
    PyThreadState_Swap(ts);
    qDebug() << "Swap from current state to this state";
    // at this point:
    // 1. You have the GIL
    // 2. You have the right thread state - a new thread state (this thread was not created by python) in the context of interp

    // PYTHON WORK HERE
    qDebug() << "Run python code";
    PyInit_testmultithread();
    PyObject* id = PyLong_FromLong((long) tid);
    PyObject* ins = createInstance(id);
    testprocess(ins);
    qDebug() << "Run python code done";

    // release ts
    PyThreadState_Swap(NULL);

    // clear and delete ts
    qDebug() << "Clear and delete state";
    PyThreadState_Clear(ts);
    PyThreadState_Delete(ts);

    // release the GIL
    qDebug() << "Release lock";
    PyEval_ReleaseLock();
}

int main(int argc, char** argv)
{
    Py_Initialize(); // Initialize Python main interpreter
    PyEval_InitThreads(); //Init and acquire GIL, should call in main thread to create and acquire GIL for main thread

    PyThreadState* maints = PyThreadState_Get(); //get thread state of main thread

    //Create 2 sub interpreters
    PyThreadState* ts1 = Py_NewInterpreter();
    PyThreadState* ts2 = Py_NewInterpreter();
    qDebug() << "Create 2 interpreters done";

    PyEval_ReleaseLock();
    qDebug() << "Release lock of main thread done";

//    PyEval_AcquireLock();
//    PyInterpreterState* maininterstate = maints->interp;


    //make 2 threads, assign new interpreter for each thread
    thread th1(do_stuff_in_thread, ts1->interp, 1);
    thread th2(do_stuff_in_thread, ts2->interp, 2);
    th1.join();
    th2.join();


  //Swap current thread state with given tstate, hold the GIL
   //At this point, swap back to main thread
    PyThreadState_Swap(maints);
    Py_Finalize();

}
