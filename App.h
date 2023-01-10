#ifndef APP_H
#define APP_H
#define kAppSignature "application/x-vnd.HourlyFortune"
#define kAppName "Hourly Fortune"
#include <Application.h>

	
class App : public BApplication
{
public:
	App(void);
	virtual	void	AboutRequested();
};

#endif
