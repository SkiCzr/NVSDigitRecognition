#ifndef SCREEN_2_PM0VIEW_HPP
#define SCREEN_2_PM0VIEW_HPP

#include <gui_generated/screen_2_pm0_screen/Screen_2_PM0ViewBase.hpp>
#include <gui/screen_2_pm0_screen/Screen_2_PM0Presenter.hpp>

class Screen_2_PM0View : public Screen_2_PM0ViewBase
{
public:
    Screen_2_PM0View();
    virtual ~Screen_2_PM0View() {}

    /* RTOS Attributes */
    bool statusChecked = false;

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void updateScreen();
    virtual void screenInit();
    virtual void checkI2CStatus();
    virtual void readGPIO();
protected:
    bool wakeup_ignored = false;
    bool pm0_enabled = false;
};

#endif // SCREEN_2_PM0VIEW_HPP
