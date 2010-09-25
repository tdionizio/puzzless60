
// INCLUDE FILES
#include <eikstart.h>
#include "PuzzlesApplication.h"

LOCAL_C CApaApplication* NewApplication()
	{
	return new CPuzzlesApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}

