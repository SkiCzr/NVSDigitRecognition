#ifndef SCREEN_2VIEW_HPP
#define SCREEN_2VIEW_HPP

#include <gui_generated/screen_2_screen/Screen_2ViewBase.hpp>
#include <gui/screen_2_screen/Screen_2Presenter.hpp>

class Screen_2View : public Screen_2ViewBase
{
public:
    Screen_2View();
    virtual ~Screen_2View() {}

    /* Swipe Attributes */
    int startTouchX;
    const int swipeThreshold = 30;

    /* RTOS Attributes */
    bool statusChecked = false;

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void updateScreen();
    virtual void screenInit();
    virtual void checkI2CStatus();
    virtual void handleClickEvent(const ClickEvent& event);
protected:
};

#endif // SCREEN_2VIEW_HPP
