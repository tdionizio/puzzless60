
#ifndef __PUZZLESAPPUI_h__
#define __PUZZLESAPPUI_h__

// INCLUDES
#include "PuzzlesConfig.h"
#include "CAppConfig.h"
#include <aknappui.h>

// FORWARD DECLARATIONS
class TTime;
class CPuzzlesAppUi;
class CPuzzlesAppView;
class CGameView;
class CGameContainer;
class CPeriodic;
class CAknTitlePane;
class CAknNavigationDecorator;

typedef struct midend midend;
typedef struct game game;
typedef struct game_params game_params;
typedef struct drawing_api drawing_api;
typedef struct blitter blitter;
typedef struct config_item config_item;

// CLASS DECLARATION
/**
 * CPuzzlesAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class CPuzzlesAppUi: public CAknAppUi {
    friend class CPuzzlesAppView;
public:
    // Constructors and destructor

    void ConstructL();
    CPuzzlesAppUi();
    virtual ~CPuzzlesAppUi();

private:
    // Functions from base classes
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
    void HandleCommandL(TInt aCommand);
    void HandleStatusPaneSizeChange();
    
private:
    CAppConfig iAppConfig;
    
    TInt iScreenMode;
    CAknTitlePane *iTitlePane;
    CAknNavigationDecorator *iNaviLabel;
    
    CPuzzlesAppView *iAppView;
    
    TRect GetGameRect();
    
public:
    typedef enum {
        EScreenNormal,
        EScreenLargeTop,
        EScreenLargeBottom,
        EScreenFull,
        EScreenMax
    } TScreenMode;
    
    CAppConfig &AppConfig() { return iAppConfig; }
    
    void SetOrientationL(TAppUiOrientation aOrientation);
    
    TInt GetScreenMode() { return iScreenMode; }
    void SetScreenMode(TInt aScreenMode);
    
    void ChangePaneTextL(const TDesC& aTitle, const TDesC& aText);
    void ChangePaneIconL(const TDesC& aIconFile, TInt aIndex, TInt aMask);
    void ChangePaneIconDefaultL();
    
    // Returns the platform version in hexadecimal form:
    // 0x0901, 0x0902, etc...
    TUint GetPlatformVersionL();
    
private: // drawing API
};

#endif // __PUZZLESAPPUI_h__
// End of File
