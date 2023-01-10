#include "App.h"
#include "MainWindow.h"


App::App(void)
  :	BApplication(kAppSignature)
{

	MainWindow *win = new MainWindow();
	win->Show();

}

void
App::AboutRequested()
{
}

int
main(void)
{
	srand(system_time());
	
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
