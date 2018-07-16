# Embedding Python Multithread
A python module count to 100000  and a C API to call this module from c++ are defined in file .pyx
The main function try to call this module twice from c++ multithreading-ly. 

Steps to call python multithreading-ly from c++ through cython
1.Run main thread
2.Create new sub interpreter (Py_NewInterpreter)
3.Get thread state of main thread (PyThreadState_Get)
4.Release gil lock in main thread (PyEval_ReleaseLock)
5.Acquire gil lock for new thread (PyEval_AcquireLock)
6.Create a new thread state for new thread (PyThreadState_New)
7.Swap to new thread (PyThreadState_Swap)
8.Run code in new thread
9.Swap back to Null (PyThreadState_Swap) to release running thread
10.Clear and delete expired thread state (PyThreadState_Clear, PyThreadState_Delete)
11.Release lock that is held by expired thread (PyEval_ReleaseLock)
11.Swap back to main thread state (PyThreadState_Swap)
12.Py_Finalize will destroy all sub interpreters. In case want to delete sub interpreter manually, call Py_EndInterpreter before swap the threads this interpreter owns (when its thread is still current thread)
