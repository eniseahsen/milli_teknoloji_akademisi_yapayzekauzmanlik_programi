#include <sys/ipc.h>     // IPC anahtarlarý için
#include <sys/shm.h>     // Paylaþýmlý bellek için
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      // fork, sleep vs.
#include <sys/wait.h>    // wait()

int main() {
    // 1. Paylaþýmlý bellek segmenti oluþtur (bir adet int için)
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // 2. Bellek segmentine attach ol (iþlem adres uzayýna dahil et)
    int *data = (int *)shmat(shmid, NULL, 0);
    *data = 0; // Baþlangýç deðeri ata

    pid_t pid = fork(); // Yeni bir process oluþtur

    if (pid == 0) {
        // ### Çocuk iþlem ###
        *data += 1;
        printf("Child) Data: %d\n", *data);
        shmdt(data); // Bellekten detach et
        exit(0);
    } else {
        // ### Ana iþlem ###
        wait(NULL); // Çocuðun bitmesini bekle  aksi takdirde sýra deðiþebilirdi - race condition
        *data += 1;
        printf("Parent) Data: %d\n", *data);
        shmdt(data); // Bellekten detach et
        shmctl(shmid, IPC_RMID, NULL); // Segmenti kaldýr
    }

    return 0;
}

