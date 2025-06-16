#include <semaphore.h>   // POSIX semaphores
#include <fcntl.h>       // O_CREAT, O_* bayraklar�
#include <stdio.h>
#include <unistd.h>      // fork, sleep
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    // 1. Adland�r�lm�� bir semafor olu�tur (ba�lang�� de�eri: 1)
    sem_t* sem = sem_open("/mysem", O_CREAT, 0644, 1);

    pid_t pid = fork();

    if (pid == 0) {
        // ### �ocuk i�lem ###
        sem_wait(sem); // Kilitle (critical section'a giri�)
        printf("[Child] In critical section\n");
        sleep(2);
        printf("[Child] Exiting critical section\n");
        sem_post(sem); // Serbest b�rak
        sem_close(sem);
        exit(0);
    } else {
        // ### Ana i�lem ###
        sleep(1);
        sem_wait(sem);
        printf("[Parent] In critical section\n");
        sleep(2);
        printf("[Parent] Exiting critical section\n");
        sem_post(sem);
        wait(NULL);         // �ocu�u bekle
        sem_close(sem);     // Semaforu kapat
        sem_unlink("/mysem"); // Sistemden sil
    }

    return 0;
}
