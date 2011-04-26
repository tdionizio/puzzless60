
#ifndef __PUZZLESAPPLICATION_H__
#define __PUZZLESAPPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "Puzzles.hrh"

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidPuzzlesApp =
	{
	_UID3
	};

// CLASS DECLARATION

/**
 * CPuzzlesApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CPuzzlesApplication is the application part of the
 * AVKON application framework for the Puzzles example application.
 */
class CPuzzlesApplication : public CAknApplication
	{
public:
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID (KUidPuzzlesApp).
	 */
	TUid AppDllUid() const;

protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates CPuzzlesDocument document object. The returned
	 * pointer in not owned by the CPuzzlesApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	};

#endif // __PUZZLESAPPLICATION_H__
// End of File
