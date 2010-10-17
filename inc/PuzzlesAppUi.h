
#ifndef __PUZZLESAPPUI_h__
#define __PUZZLESAPPUI_h__

// INCLUDES
#include "PuzzlesConfig.h"
#include "CAppConfig.h"
#include <aknappui.h>
#ifdef ENABLE_TOUCH_UI
#include <akntitlepaneobserver.h>
#include <aknnavidecoratorobserver.h>
#endif

// FORWARD DECLARATIONS
class TTime;
class CPuzzlesAppUi;
class CPuzzlesAppView;
class CGameView;
class CGameContainer;
class CPeriodic;
class CAknTitlePane;
class CAknNavigationDecorator;
#ifdef ENABLE_TOUCH_UI
class CAknStylusPopUpMenu;
#endif

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
class CPuzzlesAppUi: public CAknAppUi
#ifdef ENABLE_TOUCH_UI
    , public MAknTitlePaneObserver
    , public MAknNaviDecoratorObserver
#endif
{
public:
    void ConstructL();
    CPuzzlesAppUi();
    virtual ~CPuzzlesAppUi();

public:
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
    void HandleCommandL(TInt aCommand);
    void HandleStatusPaneSizeChange();
#ifdef ENABLE_TOUCH_UI
    void HandleTitlePaneEventL(TInt aEventID);
    void HandleNaviDecoratorEventL(TInt aEventID);
#endif
    
private:
    CAppConfig iAppConfig;
    CPuzzlesAppView *iAppView;
    
    TInt iScreenMode;
    CAknTitlePane *iTitlePane;
    CAknNavigationDecorator *iNaviLabel;
#ifdef ENABLE_TOUCH_UI
    CAknStylusPopUpMenu* iPopupMenu;
#endif
    
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
    
    // Returns the platform version in hexadecimal form:
    // 0x0901, 0x0902, etc...
    // Used for help browser: incompatible placement of top left position
    // from version 0x901 and 0x0902+
    TUint GetPlatformVersionL();
};

#endif // __PUZZLESAPPUI_h__
// End of File
