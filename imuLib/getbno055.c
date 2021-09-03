/* ------------------------------------------------------------ *
 * file:        getbno055.c                                     *
 * purpose:     Sensor control and data extraction program for  *
 *              the Bosch BNO055 absolute orientation sensor    *
 *                                                              *
 * return:      0 on success, and -1 on errors.                 *
 *                                                              *
 * requires:	I2C headers, e.g. sudo apt install libi2c-dev   *
 *                                                              *
 * compile:	gcc -o getbno055 i2c_bno055.c getbno055.c       *
 *                                                              *
 * example:	./getbno055 -t eul  -o bno055.htm               *
 *                                                              *
 * author:      05/04/2018 Frank4DD                             *
 * ------------------------------------------------------------ */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "getbno055.h"

/* ------------------------------------------------------------ *
 * Global variables and defaults                                *
 * ------------------------------------------------------------ */
int verbose = 0;
int outflag = 0;
int argflag = 0; // 1 dump, 2 reset, 3 load calib, 4 write calib
char opr_mode[9] = {0};
char pwr_mode[8] = {0};
char datatype[256];
char senaddr[256] = "0x28";
char i2c_bus[256] = I2CBUS;
char htmfile[256];
char calfile[256];

/* ------------------------------------------------------------ *
 * print_usage() prints the programs commandline instructions.  *
 * ------------------------------------------------------------ */
void usage() {
   static char const usage[] = "Usage: getbno055 [-a hex i2c-addr] [-m <opr_mode>] [-t acc|gyr|mag|eul|qua|lin|gra|inf|cal|con] [-r] [-w calfile] [-l calfile] [-o htmlfile] [-v]\n\
\n\
Command line parameters have the following format:\n\
   -a   sensor I2C bus address in hex, Example: -a 0x28 (default)\n\
   -b   I2C bus to query, Example: -b /dev/i2c-1 (default)\n\
   -d   dump the complete sensor register map content\n\
   -m   set sensor operational mode. mode arguments:\n\
           config   = configuration mode\n\
           acconly  = accelerometer only\n\
           magonly  = magnetometer only\n\
           gyronly  = gyroscope only\n\
           accmag   = accelerometer + magnetometer\n\
           accgyro  = accelerometer + gyroscope\n\
           maggyro  = magetometer + gyroscope\n\
           amg      = accelerometer + magnetometer + gyroscope\n\
           imu      = accelerometer + gyroscope fusion -> rel. orientation\n\
           compass  = accelerometer + magnetometer fusion -> abs. orientation\n\
           m4g      = accelerometer + magnetometer fusion -> rel. orientation\n\
           ndof     = accelerometer + mag + gyro fusion -> abs. orientation\n\
           ndof_fmc = ndof, using fast magnetometer calibration (FMC)\n\
   -p   set sensor power mode. mode arguments:\n\
          normal    = required sensors and MCU always on (default)\n\
          low       = enter sleep mode during motion inactivity\n\
          suspend   = sensor paused, all parts put to sleep\n\
   -r   reset sensor\n\
   -t   read and output sensor data. data type arguments:\n\
           acc = Accelerometer (X-Y-Z axis values)\n\
           gyr = Gyroscope (X-Y-Z axis values)\n\
           mag = Magnetometer (X-Y-Z axis values)\n\
           eul = Orientation E (H-R-P values as Euler angles)\n\
           qua = Orientation Q (W-X-Y-Z values as Quaternation)\n\
           gra = GravityVector (X-Y-Z axis values)\n\
           lin = Linear Accel (X-Y-Z axis values)\n\
           inf = Sensor info (23 version and state values)\n\
           cal = Calibration data (mag, gyro and accel calibration values)\n\
           con = Continuous data (eul)\n\
   -l   load sensor calibration data from file, Example -l ./bno055.cal\n\
   -w   write sensor calibration data to file, Example -w ./bno055.cal\n\
   -o   output sensor data to HTML table file, requires -t, Example: -o ./bno055.html\n\
   -h   display this message\n\
   -v   enable debug output\n\
\n\
Note: The sensor is executing calibration in the background, but only in fusion mode.\n\
\n\
Usage examples:\n\
./getbno055 -a 0x28 -t inf -v\n\
./getbno055 -t cal -v\n\
./getbno055 -t eul -o ./bno055.html\n\
./getbno055 -m ndof\n\
./getbno055 -w ./bno055.cal\n";
   printf(usage);
}

/* ------------------------------------------------------------ *
 * parseargs() checks the commandline arguments with C getopt   *
 * ------------------------------------------------------------ */
void parseargs(int argc, char* argv[]) {
   int arg;
   opterr = 0;

   if(argc == 1) { usage(); exit(-1); }

   while ((arg = (int) getopt (argc, argv, "a:b:dm:p:rt:l:w:o:hv")) != -1) {
      switch (arg) {
         // arg -v verbose, type: flag, optional
         case 'v':
            verbose = 1; break;

         // arg -a + sensor address, type: string
         // mandatory, example: 0x29
         case 'a':
            if(verbose == 1) printf("Debug: arg -a, value %s\n", optarg);
            if (strlen(optarg) != 4) {
               printf("Error: Cannot get valid -a sensor address argument.\n");
               exit(-1);
            }
            strncpy(senaddr, optarg, sizeof(senaddr));
            break;

         // arg -b + I2C bus, type: string
         // optional, example: "/dev/i2c-1"
         case 'b':
            if(verbose == 1) printf("Debug: arg -b, value %s\n", optarg);
            if (strlen(optarg) >= sizeof(i2c_bus)) {
               printf("Error: invalid i2c bus argument.\n");
               exit(-1);
            }
            strncpy(i2c_bus, optarg, sizeof(i2c_bus));
            break;

         // arg -d
         // optional, dumps the complete register map data
         case 'd':
            if(verbose == 1) printf("Debug: arg -d, value %s\n", optarg);
            argflag = 1;
            break;

         // arg -m sets operations mode, type: string
         case 'm':
            if(verbose == 1) printf("Debug: arg -m, value %s\n", optarg);
            if (strlen(optarg) >= sizeof(opr_mode)) {
               printf("Error: invalid opr_mode argument.\n");
               exit(-1);
            }
            strncpy(opr_mode, optarg, sizeof(opr_mode));
            break;

         // arg -p sets power mode, type: string
         case 'p':
            if(verbose == 1) printf("Debug: arg -p, value %s\n", optarg);
            if (strlen(optarg) >= sizeof(pwr_mode)) {
               printf("Error: invalid pwr_mode argument.\n");
               exit(-1);
            }
            strncpy(pwr_mode, optarg, sizeof(pwr_mode));
            break;

         // arg -r
         // optional, resets sensor
         case 'r':
            if(verbose == 1) printf("Debug: arg -r, value %s\n", optarg);
            argflag = 2;
            break;

         // arg -t + sensor component, type: string
         // mandatory, example: mag (magnetometer)
         case 't':
            if(verbose == 1) printf("Debug: arg -t, value %s\n", optarg);
            if (strlen(optarg) != 3) {
               printf("Error: Cannot get valid -t data type argument.\n");
               exit(-1);
            }
            strncpy(datatype, optarg, sizeof(datatype));
            break;

         // arg -l + calibration file name, type: string
         // loads the sensor calibration from file. example: ./bno055.cal
         case 'l':
            argflag = 3;
            if(verbose == 1) printf("Debug: arg -l, value %s\n", optarg);
            if (strlen(optarg) >= sizeof(calfile)) {
               printf("Error: invalid calfile argument.\n");
               exit(-1);
            }
            strncpy(calfile, optarg, sizeof(calfile));
            break;

         // arg -w + calibration file name, type: string
         // writes sensor calibration to file. example: ./bno055.cal
         case 'w':
            argflag = 4;
            if(verbose == 1) printf("Debug: arg -w, value %s\n", optarg);
            if (strlen(optarg) >= sizeof(calfile)) {
               printf("Error: invalid calfile argument.\n");
               exit(-1);
            }
            strncpy(calfile, optarg, sizeof(calfile));
            break;

         // arg -o + dst HTML file, type: string, requires -t
         // writes the sensor output to file. example: /tmp/sensor.htm
         case 'o':
            outflag = 1;
            if(verbose == 1) printf("Debug: arg -o, value %s\n", optarg);
            if (strlen(optarg) >= sizeof(htmfile)) {
               printf("Error: invalid htmfile argument.\n");
               exit(-1);
            }
            strncpy(htmfile, optarg, sizeof(htmfile));
            break;

         // arg -h usage, type: flag, optional
         case 'h':
            usage(); exit(0);
            break;

         case '?':
            if(isprint (optopt))
               printf ("Error: Unknown option `-%c'.\n", optopt);
            else
               printf ("Error: Unknown option character `\\x%x'.\n", optopt);
            usage();
            exit(-1);
            break;

         default:
            usage();
            break;
      }
   }
}

/* ----------------------------------------------------------- *
 *  print_calstat() - Read and print calibration status        *
 * ----------------------------------------------------------- */
void print_calstat() {
   struct bnocal bnoc;
   /* -------------------------------------------------------- *
    *  Check the sensors calibration state                     *
    * -------------------------------------------------------- */
   int res = get_calstatus(&bnoc);
   if(res != 0) {
      printf("Error: Cannot read calibration state.\n");
      exit(-1);
   }

   /* -------------------------------------------------------- *
    *  Convert the status code into a status message           *
    * -------------------------------------------------------- */
    printf("Sensor System Calibration = ");
    switch(bnoc.scal_st) {
      case 0:
         printf("Uncalibrated\n");
         break;
      case 1:
         printf("Minimal Calibrated\n");
         break;
      case 2:
         printf("Mostly Calibrated\n");
         break;
      case 3:
         printf("Fully calibrated\n");
         break;
   }

   printf("    Gyroscope Calibration = ");
   switch(bnoc.gcal_st) {
      case 0:
         printf("Uncalibrated\n");
         break;
      case 1:
         printf("Minimal Calibrated\n");
         break;
      case 2:
         printf("Mostly Calibrated\n");
         break;
      case 3:
         printf("Fully calibrated\n");
         break;
   }

   printf("Accelerometer Calibration = ");
   switch(bnoc.acal_st) {
      case 0:
         printf("Uncalibrated\n");
         break;
      case 1:
         printf("Minimal Calibrated\n");
         break;
      case 2:
         printf("Mostly Calibrated\n");
         break;
      case 3:
         printf("Fully calibrated\n");
         break;
   }

   printf(" Magnetometer Calibration = ");
   switch(bnoc.mcal_st) {
      case 0:
         printf("Uncalibrated\n");
         break;
      case 1:
         printf("Minimal Calibrated\n");
         break;
      case 2:
         printf("Mostly Calibrated\n");
         break;
      case 3:
         printf("Fully calibrated\n");
         break;
   }
}

