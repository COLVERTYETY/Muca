//https://www.newhavendisplay.com/appnotes/datasheets/touchpanel/FT5x16_registers.pdf
//https://www.buydisplay.com/download/ic/FT5206.pdf

// https://github.com/focaltech-systems/drivers-input-touchscreen-FTS_driver/blob/master/ft5x06.c

#include "Wire.h"

#define CTP_INT           2
#define I2C_ADDRESS       0x38


#define MODE_NORMAL       0x00
#define MODE_TEST         0x40

// NORMAL
#define TOUCH_REGISTERS   31
#define STATUS            0x02

// RAW

#define NUM_ROWS          21
#define NUM_COLUMNS       12


#include "Wire.h"


class TouchPoint {
  public:
    unsigned int flag;
    unsigned int x;
    unsigned int y;
    unsigned int weight;
    unsigned int area;
    unsigned int id;
};


class Muca {
  public:
    Muca();
    void init(bool raw = false);

    bool poll();

    // CONFIG
    void setGain(int val, bool returnNormal);
    void autocal();
    void printInfo();
    void setConfig(byte peak, byte cal, byte thresh, byte diff);
    void setResolution(unsigned short w, unsigned short h);

    // TOUCH
    bool updated();
    int getNumberOfTouches();
    TouchPoint getTouch(int i);

    // TMP
    void printAllRegisters();

    //RAW
    void pollRaw();
    bool useRaw = false;
    short grid[NUM_ROWS * NUM_COLUMNS];

    // I2C
    byte readRegister(byte reg,short numberBytes);
    byte setRegister(byte reg, byte val);

  private:
    bool isInit = false;
    unsigned short width = 800;
    unsigned short height = 480;

    // TOUCH
    TouchPoint touchpoints[5];
    byte touchRegisters[TOUCH_REGISTERS];
    void getTouchData();
    void setTouchPoints();
    byte numTouches = 0;

    //RAW
    void getRawData();
};
