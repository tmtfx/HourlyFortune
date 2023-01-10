#include "MainWindow.h"

#ifdef __HAIKU__
#	include <Alert.h>
#endif

#include "OutlineView.h"
#include "App.h"
#include <TextView.h>
#include <Path.h>
#include <FindDirectory.h>
#include <InterfaceDefs.h>
#include <ScrollView.h>
#include <View.h>
#include <cstdio>
#include <pthread.h>
#include <stdlib.h>
#include <OS.h>
#include "FortuneFunctions.h"
#include <StorageKit.h>
#include <String.h>
#include <File.h>
#include <String.h>
#include <Roster.h>
#include <Directory.h>


const float kDraggerSize = 7;

OutlineView::OutlineView(BRect frame)
	:
	BView(frame, kAppName, B_FOLLOW_ALL, B_WILL_DRAW|B_PULSE_NEEDED),
	fTextView(NULL)
{
	app_info appInfo;
	be_app -> GetAppInfo(&appInfo);
	entry_ref ref = appInfo.ref;
	BPath localpath(&ref);
	status_t stt = localpath.GetParent(&localpath);
	if (stt == B_OK)
	{
		BFile file;
		
		localpath.Append("custompath");
		status_t stato = file.SetTo(localpath.Path(), B_READ_ONLY);
		if (stato != B_OK){
			printf("Errore nell'aprire il file di configurazione personalizzata %s, err:47\n",localpath.Path());
			TryNPFortunePath();
		}
		else
		{
			// custompath exists, reading inside
			BDirectory pathfortunes;
			BString filecontent;
			char buf[1024];
			ssize_t amt_read;
			if ((amt_read = file.Read((void *)buf, 1024)) < 0)
			{
				printf("errore nel leggere il contenuto del file %s",localpath.Path());//TODO bisogna cmq caricare qualcosa
				TryNPFortunePath();
			}
			else
			{
				//looking for saved path in custompath file
				buf[amt_read]=0;
				filecontent.SetTo(buf);
				status_t customized = pathfortunes.SetTo(filecontent.String());
				if (customized == B_OK)
				{
					
					gFortunePath = filecontent.String();
					status_t rit = fFortune.SetFolder(gFortunePath);
					if (rit != B_OK)
					{
						printf("Nessun aforisma nella cartella %s\n",filecontent.String());
						SetDefFortunePath();
/*						BPath path;
						#ifdef __HAIKU__
						find_directory(B_SYSTEM_DATA_DIRECTORY,&path);
						#else
						find_directory(B_BEOS_ETC_DIRECTORY,&path);
						#endif
						path.Append("fortunes");
						gFortunePath = path.Path();
						fFortune.SetFolder(gFortunePath);*/
					}
					
				}
				else
				TryNPFortunePath();
			}
		}	
	}
	// If we want the rand() function to actually be pretty close to random
	// we will need to seed the random number generator with the time. If we
	// don't, we will get the same "random" numbers each time the program is
	// run. 
	srand(system_time());
	fReplicated = false;

	frame.left = frame.right - kDraggerSize;
	frame.top = frame.bottom - kDraggerSize;

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	BRect r(15,15,Bounds().right - B_V_SCROLL_BAR_WIDTH - 15, Frame().bottom - 15);
	fTextView = new BTextView(r, "textview", r.OffsetToCopy(0,0).InsetByCopy(10,10), B_FOLLOW_ALL,
								B_WILL_DRAW | B_PULSE_NEEDED | B_FRAME_EVENTS);
	fTextView->MakeEditable(false);

	// BScrollViews are a little weird. You can't create one without having created its target.
	// It also automatically calls AddChild() to attach its target to itself, so all that is
	// necessary is to instantiate it and attach it to the window. It's not even necessary (or
	// possible) to specify the size of the BScrollView because it calculates its size based
	// on that of its target.
	BScrollView *sv = new BScrollView("scrollview", fTextView, B_FOLLOW_ALL, 0, false, true);
	AddChild(sv);
	BDragger *dragger = new BDragger(frame, this, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	AddChild(dragger);
	int conte;
	conte = 0;
	BString fortune;
	status_t status = fFortune.GetFortune(fortune);
	if (status == B_OK)
		{
			fTextView->SetText(fortune.String());
		}
		else	
		{
			fTextView->SetText("\nHourlyFortune had a problem getting a fortune.\n\n"
				"Please make sure that you have installed fortune files to "
				"the folder ");
			fTextView->Insert(gFortunePath.String());
		}
}

void
OutlineView::TryNPFortunePath(){
			BPath tmp;
			BPath path;
			BDirectory testtmp;
			#ifdef __HAIKU__
			find_directory(B_SYSTEM_DATA_DIRECTORY,&path);
			find_directory(B_SYSTEM_NONPACKAGED_DIRECTORY, &tmp);
			#else
			find_directory(B_BEOS_ETC_DIRECTORY,&path);
			#endif
			tmp.Append("data/fortunes");
			path.Append("fortunes");
			status_t mh = testtmp.SetTo(tmp.Path());
			if (mh == B_OK)
			{
				//trying to look for fortunes in non-packaged/data/fortunes
				gFortunePath  = tmp.Path();
				status_t lst = fFortune.SetFolder(gFortunePath);
				if (lst!=B_OK)
				{
					//no fortunes inside
					gFortunePath = path.Path();
					fFortune.SetFolder(gFortunePath);
				}
			}
			else
			{
				//there's no such directory, using system default fortunes
				gFortunePath = path.Path();
				fFortune.SetFolder(gFortunePath);
			}
			
}


void
OutlineView::SetDefFortunePath(){
	BPath path;
	#ifdef __HAIKU__
	find_directory(B_SYSTEM_DATA_DIRECTORY,&path);
	#else
	find_directory(B_BEOS_ETC_DIRECTORY,&path);
	#endif
	path.Append("fortunes");
	gFortunePath = path.Path();
	fFortune.SetFolder(gFortunePath);
}
void
OutlineView::Pulse(){

	BString fortune;
	status_t status = fFortune.GetFortune(fortune);
	conte = conte + 1;
	if ( conte % 600 == 0 ){
	if (status == B_OK)
		{
			fTextView->SetText(fortune.String());
		}
		else	
		{
			fTextView->SetText("\nHourlyFortune had a problem getting a fortune.\n\n"
				"Please make sure that you have installed fortune files to "
				"the folder ");
			fTextView->Insert(gFortunePath.String());
		}
	if (!IsHidden()) {
			Draw(Bounds());
		}
	}
}

OutlineView::OutlineView(BMessage *archive)
	:
	BView(archive)
{
	
	fReplicated = true;

/*
// originale
	BPath path;

	find_directory(B_SYSTEM_DATA_DIRECTORY,&path);
	
	path.Append("fortunes");
	gFortunePath = path.Path();
	fFortune.SetFolder(gFortunePath);*/
	BPath path;
	find_directory(B_SYSTEM_NONPACKAGED_DIRECTORY,&path);
	path.Append("data/fortunes");
	gFortunePath = path.Path();
	status_t really = fFortune.SetFolder(gFortunePath);
	// se non ci sono aforismi nella cartella non-packaged si cerca nella cartella di sistema predefinita
	if (really != B_OK)
	{
		#ifdef __HAIKU__
		find_directory(B_SYSTEM_DATA_DIRECTORY,&path);
		#else
		find_directory(B_BEOS_ETC_DIRECTORY,&path);
		#endif
		path.Append("fortunes");
		gFortunePath = path.Path();
		fFortune.SetFolder(gFortunePath);
	}
	SetViewColor(B_TRANSPARENT_COLOR);
	conte = 0;
	
	fTextView = dynamic_cast<BTextView*>(FindView("textview"));
	//fTextView->SetViewColor(B_TRANSPARENT_COLOR);
}

OutlineView::~OutlineView()
{
	delete fTextView;
}


BArchivable *OutlineView::Instantiate(BMessage *data)
{
	return new OutlineView(data);
}

status_t
OutlineView::Archive(BMessage *archive, bool deep) const
{
	status_t status = BView::Archive(archive, deep);
	if (status == B_OK)
		status = archive->AddString("add_on", kAppSignature);

	return status;
}


void
OutlineView::MessageReceived(BMessage *msg)
{
	switch (msg->what) {
		case B_ABOUT_REQUESTED:
		{
			BAlert *alert = new BAlert("about",
			"Hourly Fortune\nCopyright 2023\n\n\tby Fabio Tomat\n\tStefano Ceccherini\n\t",
			"OK");
			alert->SetFlags(alert->Flags() | B_CLOSE_ON_ESCAPE);
			alert->Go();
			break;
		}
		default:
			BView::MessageReceived(msg);
			break;
	}
}

void
OutlineView::FrameResized(float w, float h)
{
	// The BWindow::FrameResized() method is called whenever the window is resized.
	// In this case, we change the size of the text rectangle in the text view used
	// for the fortune. We have to do this because it won't do it itself. Lazy.
	BRect textrect = fTextView->TextRect();
	
	textrect.right = textrect.left + (w - B_V_SCROLL_BAR_WIDTH - 40);
	fTextView->SetTextRect(textrect);
}
