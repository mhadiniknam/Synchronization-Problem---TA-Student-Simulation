#include <iostream>
#include <pthread.h>
#define NUM_STUDENT 5
#define NUM_CHAIRS 5
#define NUM_TEACHERS 1
#define QUESTION_CHANCE_PROBABILITY 0.2
#include <vector>

int students[NUM_STUDENT];
int Student_done[NUM_STUDENT];
int start = 0;
int end = 0;
int buffer[NUM_CHAIRS];

pthread_mutex_t mutex;
pthread_cond_t cond_student_and_teacher;

int teacher_threasds_loop = 1;
void* TA(void* arg){
    while (teacher_threasds_loop){
        pthread_mutex_lock(&mutex);
        while (start == end){
            std::cout << "TA is sleeping" << std::endl;
            pthread_cond_wait(&cond_student_and_teacher, &mutex);
        }
        int student = buffer[start];
        start = (start + 1) % NUM_CHAIRS;
        std::cout << "TA is helping student " << student << std::endl;
        Student_done[student] = 1;
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

void addStudent(int student){
    pthread_mutex_lock(&mutex);
    if ((end + 1) % NUM_CHAIRS == start){
    //  std::cout << "Student " << student << " is waiting" << std::endl;
    } else {
        buffer[end] = student;
        end = (end + 1) % NUM_CHAIRS;
        std::cout << "Student " << student << " is added to the buffer" << std::endl;
        pthread_cond_signal(&cond_student_and_teacher);
    }
    pthread_mutex_unlock(&mutex);
}

int main(){
    pthread_t ta_thread[NUM_TEACHERS];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_student_and_teacher, NULL);


    // We create one thread for each TA and each Student

    for (int i = 0; i < NUM_TEACHERS; i++){
        pthread_create(&ta_thread[i], NULL, TA, NULL);
    }

    for (int i = 0 ; i < NUM_STUDENT; i++){
        students[i] = i;
    }

    while (true){
        for (int i = 0; i < NUM_STUDENT; i++){
            double random_value = static_cast<double>(rand()) / RAND_MAX;
            if (random_value < QUESTION_CHANCE_PROBABILITY){
                addStudent(students[i]);
            }
        }

        // Signal TA to exit

        // Let's assume that the TA will do the task for all of the student only one 
        // and if it done for every one at le t one time it will exit
        int flag = 1;
        for (int i = 0; i < NUM_STUDENT; i++){
            if (flag){
                if(Student_done[i] == 0){
                    flag = 0;
                    break;
                }
            }
        }
        if (flag){
            pthread_mutex_lock(&mutex);
                teacher_threasds_loop = 0;
            pthread_cond_broadcast(&cond_student_and_teacher); 
        pthread_mutex_unlock(&mutex);
               for (int i = 0; i < NUM_TEACHERS; i++){
                    pthread_join(ta_thread[i], NULL);
                }
        }

        if (flag){
            break;
        }
        
    std::cout << "All students have been helped" << std::endl;
    }

    std::cout<< "All threads have finished" << std::endl;

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_student_and_teacher);
    return 0;
}
