
#ifndef __PUZZLESDOCUMENT_h__
#define __PUZZLESDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CPuzzlesAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CPuzzlesDocument application class.
 * An instance of class CPuzzlesDocument is the Document part of the
 * AVKON application framework for the Puzzles example application.
 */
class CPuzzlesDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CPuzzlesDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CPuzzlesDocument.
	 */
	static CPuzzlesDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CPuzzlesDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CPuzzlesDocument.
	 */
	static CPuzzlesDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~CPuzzlesDocument
	 * Virtual Destructor.
	 */
	virtual ~CPuzzlesDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CPuzzlesAppUi object and return a pointer to it.
	 * The object returned is owned by the Uikon framework.
	 * @return Pointer to created instance of AppUi.
	 */
	CEikAppUi* CreateAppUiL();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CPuzzlesDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CPuzzlesDocument(CEikApplication& aApp);

	};

#endif // __PUZZLESDOCUMENT_h__
// End of File
