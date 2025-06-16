#include <sys/ipc.h>     // IPC anahtarlar� i�in
#include <sys/shm.h>     // Payla��ml� bellek i�in
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      // fork, sleep vs.
#include <sys/wait.h>    // wait()

int main() {
    // 1. Payla��ml� bellek segmenti olu�tur (bir adet int i�in)
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // 2. Bellek segmentine attach ol (i�lem adres uzay�na dahil et)
    int *data = (int *)shmat(shmid, NULL, 0);
    *data = 0; // Ba�lang�� de�eri ata

    pid_t pid = fork(); // Yeni bir process olu�tur

    if (pid == 0) {
        // ### �ocuk i�lem ###
        *data += 1;
        printf("Child) Data: %d\n", *data);
        shmdt(data); // Bellekten detach et
        exit(0);
    } else {
        // ### Ana i�lem ###
        wait(NULL); // �ocu�un bitmesini bekle  aksi takdirde s�ra de�i�ebilirdi - race condition
        *data += 1;
        printf("Parent) Data: %d\n", *data);
        shmdt(data); // Bellekten detach et
        shmctl(shmid, IPC_RMID, NULL); // Segmenti kald�r
    }

    return 0;
}

