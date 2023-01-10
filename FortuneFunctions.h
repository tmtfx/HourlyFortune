#ifndef FORTUNEFUNCTIONS_H
#define FORTUNEFUNCTIONS_H

#include <List.h>
#include <Path.h>
#include <String.h>

extern BString gFortunePath;

class FortuneAccess
{
public:
				FortuneAccess(void);
				FortuneAccess(const char *folder);
				~FortuneAccess(void);
	
	status_t	SetFolder(const char *folder);
	status_t	GetFortune(BString &target);
	int32		CountFiles(void) const;
	status_t	LastFilename(BString &target);
	
private:
	status_t		ScanFolder(void);
	void		MakeEmpty(void);
	BPath		path;
	BString		fPath,
				fLastFile;
	BList		fRefList;
};

#endif
