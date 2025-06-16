#include <semaphore.h>   // POSIX semaphores
#include <fcntl.h>       // O_CREAT, O_* bayraklarý
#include <stdio.h>
#include <unistd.h>      // fork, sleep
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    // 1. Adlandýrýlmýþ bir semafor oluþtur (baþlangýç deðeri: 1)
    sem_t* sem = sem_open("/mysem", O_CREAT, 0644, 1);

    pid_t pid = fork();

    if (pid == 0) {
        // ### Çocuk iþlem ###
        sem_wait(sem); // Kilitle (critical section'a giriþ)
        printf("[Child] In critical section\n");
        sleep(2);
        printf("[Child] Exiting critical section\n");
        sem_post(sem); // Serbest býrak
        sem_close(sem);
        exit(0);
    } else {
        // ### Ana iþlem ###
        sleep(1);
        sem_wait(sem);
        printf("[Parent] In critical section\n");
        sleep(2);
        printf("[Parent] Exiting critical section\n");
        sem_post(sem);
        wait(NULL);         // Çocuðu bekle
        sem_close(sem);     // Semaforu kapat
        sem_unlink("/mysem"); // Sistemden sil
    }

    return 0;
}
