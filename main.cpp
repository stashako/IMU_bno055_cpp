#include <iostream>
#include "getbno055.h"

using namespace std;

class imuSensor
{

public:
    char senaddr[256] = "0x28";
    char i2c_bus[256] = I2CBUS;
    imuSensor()
    {
        get_i2cbus(i2c_bus, senaddr);
    }
    void mode(opmode_t newmode)
    {
        //set the sensor operational mode
        set_mode(newmode);
        //newmode = acconly;
        //newmode = magonly;
        //newmode = gyronly;
        //newmode = accmag;
        //newmode = accgyro;
        //newmode = maggyro;
        //newmode = amg;
        //newmode = imu;
        //newmode = compass;
        //newmode = m4g;
        //newmode = ndof;
        //newmode = ndof_fmc;
    }

    void power(power_t newmode1)
    {
        //set the sensor power mode
        set_power(newmode1);
        //newmode1 = normal;
        //newmode1 = low;
        //newmode1 = suspend;
    }

    int *data()
    {
        int data[22];

        struct bnoacc bnod1;
        get_acc(&bnod1);
        data[0] = bnod1.adata_x;
        data[1] = bnod1.adata_y;
        data[2] = bnod1.adata_z;

        struct bnomag bnod2;
        get_mag(&bnod2);
        data[3] = bnod2.mdata_x;
        data[4] = bnod2.mdata_y;
        data[5] = bnod2.mdata_z;

        struct bnogyr bnod3;
        get_gyr(&bnod3);
        data[6] = bnod3.gdata_x;
        data[7] = bnod3.gdata_y;
        data[8] = bnod3.gdata_z;

        struct bnoeul bnod4;
        get_eul(&bnod4);
        data[9] = bnod4.eul_head;
        data[10] = bnod4.eul_roll;
        data[11] = bnod4.eul_pitc;

        struct bnoqua bnod5;
        get_qua(&bnod5);
        data[12] = bnod5.quater_w;
        data[13] = bnod5.quater_x;
        data[14] = bnod5.quater_y;
        data[15] = bnod5.quater_z;

        struct bnogra bnod6;
        get_gra(&bnod6);
        data[16] = bnod6.gravityx;
        data[17] = bnod6.gravityy;
        data[18] = bnod6.gravityz;

        struct bnolin bnod7;
        get_lin(&bnod7);
        data[19] = bnod7.linacc_x;
        data[20] = bnod7.linacc_y;
        data[21] = bnod7.linacc_z;

        return data;
    }
};


int main()
{
    imuSensor imu1;
    imu1.mode(ndof_fmc);

    int *imuData;
    imuData = imu1.data();

    for (int i = 0; i < 22; i++)
    {
        cout << *(imuData + i) << endl;
    }

    return 0;
}

