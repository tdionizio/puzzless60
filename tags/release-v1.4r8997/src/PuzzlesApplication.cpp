
// INCLUDE FILES
#include "Puzzles.hrh"
#include "PuzzlesDocument.h"
#include "PuzzlesApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPuzzlesApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CPuzzlesApplication::CreateDocumentL()
	{
	// Create an Puzzles document, and return a pointer to it
	return CPuzzlesDocument::NewL(*this);
	}

// -----------------------------------------------------------------------------
// CPuzzlesApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CPuzzlesApplication::AppDllUid() const
	{
	// Return the UID for the Puzzles application
	return KUidPuzzlesApp;
	}

// End of File
