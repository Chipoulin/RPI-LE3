#include "rfid.h"

uint32_t spi_speed_rfid = 1000L; // speed for SP (4 < >125) overruled by config file value
uint8_t gpio_rfid=22;           // GPIO for hard-reset
uint8_t loop_rfid = 10; // main loop

void initRFID()
{
    // catch signals
    set_signals();

    /* read /etc/rc522.conf */
    if (get_config_file()) exit(1);

    /* set BCM2835 Pins correct */
    if (HW_init(spi_speed_rfid,gpio_rfid)) close_out(1);

    /* initialise the RC522 */
    InitRc522();

    /* read & set GID and UID from config file */
    if (read_conf_uid()!= 0) close_out(1);
}

// int main(int argc, char *argv[]) {

//     // // printf("ICI0\n");

//     // // catch signals
// 	// set_signals();

//     // // printf("ICI1\n");
	    
//     // /* read /etc/rc522.conf */
//     // if (get_config_file()) exit(1);

//     // // printf("ICI2\n");

//     // /* set BCM2835 Pins correct */
//     // if (HW_init(spi_speed_rfid,gpio_rfid)) close_out(1);

//     // // printf("ICI3\n");
    
//     // /* initialise the RC522 */
// 	// InitRc522();

//     // // printf("ICI4\n");

//     // /* read & set GID and UID from config file */
//     // if (read_conf_uid()!= 0) close_out(1);

//     initRFID();

//     // printf("ICI5\n");
	
//     char nuc_uid[23];
//     while (loop_rfid > 0)
//     {
//         get_card_details(nuc_uid);
//         printf("%s\n",nuc_uid);
//         loop_rfid--;
//     }   // loop

//     close_out(0);
    
//     // stop -Wall complaining
//     exit(0);
// }