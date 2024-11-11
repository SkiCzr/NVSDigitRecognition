#ifndef SCREEN_1_LP_STATISTICSPRESENTER_HPP
#define SCREEN_1_LP_STATISTICSPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_1_LP_StatisticsView;

class Screen_1_LP_StatisticsPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_1_LP_StatisticsPresenter(Screen_1_LP_StatisticsView& v);

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

    virtual ~Screen_1_LP_StatisticsPresenter() {}

private:
    Screen_1_LP_StatisticsPresenter();

    Screen_1_LP_StatisticsView& view;
};

#endif // SCREEN_1_LP_STATISTICSPRESENTER_HPP
