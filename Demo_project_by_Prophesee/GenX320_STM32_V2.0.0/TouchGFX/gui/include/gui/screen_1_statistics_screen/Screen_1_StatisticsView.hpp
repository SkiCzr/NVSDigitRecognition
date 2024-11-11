#ifndef SCREEN_1_STATISTICSVIEW_HPP
#define SCREEN_1_STATISTICSVIEW_HPP

#include <gui_generated/screen_1_statistics_screen/Screen_1_StatisticsViewBase.hpp>
#include <gui/screen_1_statistics_screen/Screen_1_StatisticsPresenter.hpp>

class Screen_1_StatisticsView : public Screen_1_StatisticsViewBase
{
public:
    Screen_1_StatisticsView();
    virtual ~Screen_1_StatisticsView() {}

    /* Swipe Attributes */
    int startTouchX;
    int startTouchY;
    const int swipeThreshold = 30;

    /* RTOS Attributes */
    bool statusChecked = false;

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void updateScreen();
    virtual void updateGraph();
    virtual void screenInit();
    virtual void checkI2CStatus();
    virtual void handleClickEvent(const ClickEvent& event);
protected:

};

#endif // SCREEN_1_STATISTICSVIEW_HPP
