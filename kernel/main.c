#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

void
stamp()
{
    printf("\033[1;31m");
    printf(" .d8888b.   .d8888b.   d888      d8888        d888  888888888      d8888      d8888   .d8888b.\n");
    printf("d88P  Y88b d88P  Y88b d8888     d8P888       d8888  888           d8P888     d8P888  d88P  Y88b \n");
    printf("       888 888    888   888    d8P 888         888  888          d8P 888    d8P 888  888    888 \n");
    printf("     .d88P 888    888   888   d8P  888         888  8888888b.   d8P  888   d8P  888  Y88b. d888 \n");
    printf(" .od888P\"  888    888   888  d88   888         888       \"Y88b d88   888  d88   888   \"Y888P888 \n");
    printf("d88P\"      888    888   888  8888888888 888888 888         888 8888888888 8888888888        888 \n");
    printf("888\"       Y88b  d88P   888        888         888  Y88b  d88P       888        888  Y88b  d88P \n");
    printf("888888888   \"Y8888P\"  8888888      888       8888888 \"Y8888P\"        888        888   \"Y8888P\" \n"); 
    printf("\033[0m");
    printf("\033[1;33m");
    printf(" .d8888b. Y88b   d88P      888    888 888       888        d8888 888b    888  .d8888b.\n");
    printf("d88P  Y88b Y88b d88P       888    888 888   o   888       d88888 8888b   888 d88P  Y88b\n");
    printf("Y88b.       Y88o88P        888    888 888  d8b  888      d88P888 88888b  888 888    888\n"); 
    printf(" \"Y888b.     Y888P         8888888888 888 d888b 888     d88P 888 888Y88b 888 888       \n");
    printf("    \"Y88b.    888          888    888 888d88888b888    d88P  888 888 Y88b888 888  88888\n"); 
    printf("      \"888    888          888    888 88888P Y88888   d88P   888 888  Y88888 888    888\n"); 
    printf("Y88b  d88P    888          888    888 8888P   Y8888  d8888888888 888   Y8888 Y88b  d88P  \n"); 
    printf(" \"Y888P\"     888          888    888 888P     Y888 d88P     888 888    Y888  \"Y8888P88\n\n\n");
    printf("\033[0m");
}

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode cache
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    stamp();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
