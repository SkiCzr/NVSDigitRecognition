/* TouchGFX Includes */
#include <gui/screen_1_statistics_screen/Screen_1_StatisticsView.hpp>
#include <touchgfx/Color.hpp>

/* Standard Includes */
#include <vector>
#include <stdio.h>

/* User Includes */
#include "main.h"
#include "genx320.h"
#include "tasks.h"
#include "app_state.h"

Screen_1_StatisticsView::Screen_1_StatisticsView()
{

}

void Screen_1_StatisticsView::setupScreen()
{
    Screen_1_StatisticsViewBase::setupScreen();
}

void Screen_1_StatisticsView::tearDownScreen()
{
    Screen_1_StatisticsViewBase::tearDownScreen();
}

/**
 * @brief Function to handle the screen swipe.
 */
void Screen_1_StatisticsView::handleClickEvent(const ClickEvent& event){

	if (event.getType() == ClickEvent::PRESSED)
	{
		/* Record the initial touch position */
		startTouchX = event.getX();
		startTouchY = event.getY();
	}
	else if (event.getType() == ClickEvent::RELEASED)
	{
		/* Calculate the distance moved */
		int32_t deltaX = event.getX() - startTouchX;
		int32_t deltaY = event.getY() - startTouchY;

		/* Check if it's a Down swipe */
		if (deltaY > swipeThreshold)
		{
			Screen_1_StatisticsView::Change_Screen_Up();
		}
		/* Check if it's a left swipe */
		else if (deltaX < -swipeThreshold)
		{
			Screen_1_StatisticsView::Change_Screen_Right();
		}
	}
}

/**
 * @brief Function to initialize the screen.
 */
void Screen_1_StatisticsView::screenInit() {

	statusChecked = false;

}

/**
 * @brief Function to check the status of the IC2 driver.
 */
void Screen_1_StatisticsView::checkI2CStatus() {

	if(statusChecked != true ) {

		/* Check if the Lock is available before suspending tasks */
		UBaseType_t lockState = get_lock_sem_count(drv_i2c);
		if(lockState == 1) {
			Screen_1_StatisticsView::updateScreen();
			statusChecked = true;
		}
	}
}

/**
 * @brief Function to update the selected screen.
 */
void Screen_1_StatisticsView::updateScreen()
{
	enter_statistics_view();
}

/**
 * @brief Function to update the graph.
 */
void Screen_1_StatisticsView::updateGraph()
{
	/* Update Pixel Event Counter - Statistics */
	Events_Graph.addDataPoint(total_ro_evt_cd_cnt);
	Unicode::snprintf(EV_TextBuffer, EV_TEXT_SIZE, "%04u", total_ro_evt_cd_cnt);
	EV_Text.invalidate();

	/* Update AFK - Statistics */
	AFK_Graph.addDataPoint(afk_flickering_ratio);
	Unicode::snprintf(Flicker_TextBuffer, FLICKER_TEXT_SIZE, "%02u", afk_flickering_ratio);
	Flicker_Text.invalidate();
}
