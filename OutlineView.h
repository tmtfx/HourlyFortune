#ifndef OUTLINEVIEW_H
#define OUTLINEVIEW_H

#include <stdio.h>
#include <TextView.h>
#include <Alert.h>

//#include <TranslationUtils.h>
#include <Dragger.h>
#include <View.h>
#include <Window.h>

#include "FortuneFunctions.h"


class _EXPORT OutlineView;

class OutlineView : public BView {
public:
						OutlineView(BRect frame);
						OutlineView(BMessage *data);
						~OutlineView();
	void				Pulse();
	void				TryNPFortunePath();
	void				SetDefFortunePath();
	virtual void 		MessageReceived(BMessage *msg);
	void				FrameResized(float w, float h);
	static 				BArchivable *Instantiate(BMessage *archive);
	virtual status_t 	Archive(BMessage *data, bool deep = true) const;
//	void				OutlineAboutRequested();
	BTextView		*fTextView;
	

private:
	
	FortuneAccess	fFortune;
	bool				fReplicated;
	int		conte;
};

#endif	// OUTLINE_VIEW_H
