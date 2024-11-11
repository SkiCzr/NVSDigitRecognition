#ifndef SCREEN_1_LP_STATISTICSVIEW_HPP
#define SCREEN_1_LP_STATISTICSVIEW_HPP

#include <gui_generated/screen_1_lp_statistics_screen/Screen_1_LP_StatisticsViewBase.hpp>
#include <gui/screen_1_lp_statistics_screen/Screen_1_LP_StatisticsPresenter.hpp>

class Screen_1_LP_StatisticsView : public Screen_1_LP_StatisticsViewBase
{
public:
    Screen_1_LP_StatisticsView();
    virtual ~Screen_1_LP_StatisticsView() {}

    /* Swipe Attributes */
    int startTouchX;
    const int swipeThreshold = 30;
    const int totalCounters = 16;

    /* RTOS Attributes */
    bool statusChecked = false;

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void updateScreen();
    virtual void readLpCounters();
    virtual void screenInit();
    virtual void checkI2CStatus();
    virtual void handleClickEvent(const ClickEvent& event);

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

#endif // SCREEN_1_LP_STATISTICSVIEW_HPP
