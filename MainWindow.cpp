#include "MainWindow.h"
#include "App.h"
#include <Application.h>
#include <Screen.h>
#include <View.h>
#include "OutlineView.h"

MainWindow::MainWindow(void)
  :	BWindow(BRect(0,0,300,300), "HourlyFortune", B_TITLED_WINDOW,
  			B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)//, 	fFortune(gFortunePath.String())
{

	// Create all of the controls for our window.
	//
	SetPulseRate(1000000);
	OutlineView *back = new OutlineView(Bounds());
	back->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(back);
	BRect r = BScreen().Frame();
	MoveTo((r.Width()-Bounds().Width()) / 2.0, r.Height() / 4.0);
}

bool
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
