#ifndef SCREEN_1VIEW_HPP
#define SCREEN_1VIEW_HPP

#include <gui_generated/screen_1_screen/Screen_1ViewBase.hpp>
#include <gui/screen_1_screen/Screen_1Presenter.hpp>

class Screen_1View : public Screen_1ViewBase
{
public:
    Screen_1View();
    virtual ~Screen_1View() {}

    /* Swipe Attributes */
    int startTouchX;
    int startTouchY;
    const int swipeThreshold = 30;

    /* RTOS Attributes */
    bool statusChecked = false;

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void updateScreen();
    virtual void updateEvents();
    virtual void screenInit();
    virtual void checkI2CStatus();
    virtual void handleClickEvent(const ClickEvent& event);
protected:

};

#endif // SCREEN_1VIEW_HPP
