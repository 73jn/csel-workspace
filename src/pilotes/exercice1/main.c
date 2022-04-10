#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include <unistd.h>

#define STARTADDR 0x01c14200
#define STOPADDR 0x01c1420c
int main()
{
    // Ouverture du fichier correspondant au pilote
    int fd = open ("/dev/mem", O_RDWR); //descripteur du fichier correspondant au pilote

    // Appel de l’opération mmap afin de placer dans la mémoire virtuelle du processus les registres du périphérique
    void* addr = 0;  //généralement NULL, adresse de départ en mémoire virtuelle
    size_t length = getpagesize(); //taille de la zone à placer en mémoire virtuelle
    int prot =  PROT_READ | PROT_WRITE; //droits d’accès à la mémoire: read, write, execute
    int flags = MAP_SHARED; //visibilité de la page pour d’autres processus: shared, private
    off_t offset = STARTADDR - (STARTADDR % length);  //offset des registres en mémoire

    printf("OFS : %d\n",(STARTADDR % length));
    volatile uint32_t* regs = mmap(addr, length, prot, flags, fd, offset);

    // Opérations sur le périphérique à l’aide de l’adresse virtuelle retournée par l’opération mmap
    uint32_t chipid[4] = {
    *(regs + (STARTADDR % length)/4), 
    *(regs + ((STARTADDR % length) + 0x04)/4),
    *(regs + ((STARTADDR % length) + 0x08)/4),
    *(regs + ((STARTADDR % length) + 0x0c)/4), //Divide by 4 because we want only 8 bits
    };

    printf("NanoPi NEO Plus2 chipid=%08x'%08x'%08x'%08x\n",
            chipid[0],
            chipid[1],
            chipid[2],
            chipid[3]);

    // Après utilisation, appel de l’opération munmap pour libérer l’espace mémoire (munmap (…))
    // Fermeture du fichier (close (…))
    munmap((void*)regs, length);
    close(fd);
}