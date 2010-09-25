
// INCLUDE FILES
#include "PuzzlesAppUi.h"
#include "PuzzlesDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPuzzlesDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPuzzlesDocument* CPuzzlesDocument::NewL(CEikApplication& aApp)
	{
	CPuzzlesDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CPuzzlesDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPuzzlesDocument* CPuzzlesDocument::NewLC(CEikApplication& aApp)
	{
	CPuzzlesDocument* self = new (ELeave) CPuzzlesDocument(aApp);

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CPuzzlesDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPuzzlesDocument::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CPuzzlesDocument::CPuzzlesDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CPuzzlesDocument::CPuzzlesDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CPuzzlesDocument::~CPuzzlesDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CPuzzlesDocument::~CPuzzlesDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CPuzzlesDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CPuzzlesDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new (ELeave) CPuzzlesAppUi;
	}

// End of File
