#ifndef SCREEN_3_LTVIEW_HPP
#define SCREEN_3_LTVIEW_HPP

#include <gui_generated/screen_3_lt_screen/Screen_3_LTViewBase.hpp>
#include <gui/screen_3_lt_screen/Screen_3_LTPresenter.hpp>

class Screen_3_LTView : public Screen_3_LTViewBase
{
public:
    Screen_3_LTView();
    virtual ~Screen_3_LTView() {}

    /* Swipe Attributes */
    int startTouchX;
    const int swipeThreshold = 30;

    /* Screen Attributes */
    const int xStart = 0;
    const int xEnd = 320;
    const int yStart = 0;
    const int yEnd = 272;
    const int guiDifferenceConst = 272;

    /* RTOS Attributes */
    bool statusChecked = false;

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleClickEvent(const ClickEvent& event);
    virtual void updateScreen();
    virtual void updateCoordinates();
    virtual void screenInit();
    virtual void checkI2CStatus();
protected:

};

#endif // SCREEN_3_LTVIEW_HPP
