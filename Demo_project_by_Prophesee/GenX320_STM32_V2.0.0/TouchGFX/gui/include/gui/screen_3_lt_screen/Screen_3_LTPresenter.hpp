#ifndef SCREEN_3_LTPRESENTER_HPP
#define SCREEN_3_LTPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_3_LTView;

class Screen_3_LTPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_3_LTPresenter(Screen_3_LTView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Screen_3_LTPresenter() {}

private:
    Screen_3_LTPresenter();

    Screen_3_LTView& view;
};

#endif // SCREEN_3_LTPRESENTER_HPP
