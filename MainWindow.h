#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include "OutlineView.h"

class MainWindow : public BWindow
{
public:
					MainWindow(void);
		virtual bool 	QuitRequested();
	
private:
	OutlineView		*back;
};

#endif
