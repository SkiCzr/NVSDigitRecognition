#ifndef SCREEN_2_PM2VIEW_HPP
#define SCREEN_2_PM2VIEW_HPP

#include <gui_generated/screen_2_pm2_screen/Screen_2_PM2ViewBase.hpp>
#include <gui/screen_2_pm2_screen/Screen_2_PM2Presenter.hpp>

class Screen_2_PM2View : public Screen_2_PM2ViewBase
{
public:
    Screen_2_PM2View();
    virtual ~Screen_2_PM2View() {}

    /* ESP Variables */
    int timerCount = 0;
    bool lockState = false;
    bool deviceState = false;
    bool screenUpdated = false;

    /* RTOS Attributes */
    bool statusChecked = false;

    const int totalCounters = 16;

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void readLPW();
    virtual void updateScreen();
    virtual void lowPowerMode();
    virtual void screenInit();
    virtual void checkI2CStatus();
protected:

    touchgfx::BoxWithBorder* lpCounters[16];

    void initCounters() {
    	lpCounters[0] = &Counter_0;
    	lpCounters[1] = &Counter_1;
    	lpCounters[2] = &Counter_2;
    	lpCounters[3] = &Counter_3;
    	lpCounters[4] = &Counter_4;
    	lpCounters[5] = &Counter_5;
    	lpCounters[6] = &Counter_6;
    	lpCounters[7] = &Counter_7;
    	lpCounters[8] = &Counter_8;
    	lpCounters[9] = &Counter_9;
    	lpCounters[10] = &Counter_10;
    	lpCounters[11] = &Counter_11;
    	lpCounters[12] = &Counter_12;
    	lpCounters[13] = &Counter_13;
    	lpCounters[14] = &Counter_14;
    	lpCounters[15] = &Counter_15;
    }

};

#endif // SCREEN_2_PM2VIEW_HPP
