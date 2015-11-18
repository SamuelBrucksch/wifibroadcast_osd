#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdio.h>
#include <stdlib.h>


#include "telemetry.h"


void telemetry_init(telemetry_data_t *td) {
	td->voltage = 0;
	td->altitude = 0;
	td->longitude = 0;
	td->latitude = 0;
	td->heading = 0;
	td->speed = 0;
	td->x = 0;
	td->y = 0;
	td->z = 0;
	td->ew = 0;
	td->ns = 0;
	td->rx_status = telemetry_wbc_status_memory_open();
}


wifibroadcast_rx_status_t *telemetry_wbc_status_memory_open(void) {
        int fd = shm_open("/wifibroadcast_rx_status_0", O_RDWR, S_IRUSR | S_IWUSR);

        if(fd < 0) {
                fprintf(stderr, "Could not open wifibroadcast rx status\n");
		return NULL;
        }

        if (ftruncate(fd, sizeof(wifibroadcast_rx_status_t)) == -1) {
                perror("ftruncate");
                exit(1);
        }

        void *retval = mmap(NULL, sizeof(wifibroadcast_rx_status_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (retval == MAP_FAILED) {
                perror("mmap");
                exit(1);
        }


        return (wifibroadcast_rx_status_t*)retval;

}


