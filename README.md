# Synchronization-Problem---TA-Student-Simulation
On the Pdf of the Repo , (Persian)

`pthread_cond_wait()` We shall use this pthread_cond_wait it's something that we can message the function to stop 

`pthread_cond_broadcast` : This awake all of the waited threads

### The problem that get solved by me here 

I want to join each process at the end of the program , but it seems that I make a Deadlock , I try to access the mutex in the teacher thread but I gain the access before that on the broad casting so the teacher thread could not regain the mutex we stuck in the deadlock