
// INCLUDE FILES
#include "PuzzlesConfig.h"

#include <avkon.hrh>
#include <eikspane.h>

#include <akntitle.h>
#include <aknnavi.h>
#include <aknnavide.h>
#include <aknnavilabel.h>
#include <aknlists.h>
#ifdef ENABLE_TOUCH_UI
#include <aknstyluspopupmenu.h>
#endif

#include <stringloader.h>
#include <hlplch.h>
#include <e32base.h>
#include <e32std.h>

#include <string.h>

#include <puzzles_0xa000ef77.rsg>

#include "puzzles_0xa000ef77.mbg"

#include "Puzzles.hrh"
#include "Puzzles.pan"
#include "PuzzlesAppUi.h"
#include "PuzzlesAppView.h"
#include "CGameView.h"
#include "CGameContainer.h"
#include "CAppConfig.h"

extern "C" {
#include "puzzles.h"
}

void CPuzzlesAppUi::ChangePaneTextL(const TDesC& aTitle, const TDesC& aText) {
    if (aTitle.Length() == 0) {
        iTitlePane->SetTextToDefaultL();
    } else {
        iTitlePane->SetTextL(aTitle);
    }
    
    CAknNaviLabel *label = (CAknNaviLabel*)iNaviLabel->DecoratedControl();
    label->SetTextL(aText);
    
    iTitlePane->DrawDeferred();
    label->DrawDeferred();
}

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CPuzzlesAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPuzzlesAppUi::ConstructL() {
    // Initialise app UI with standard value.
    BaseConstructL(CAknAppUi::EAknEnableSkin);
    
    iAppConfig.ConstructL();
    
    // create navigation pane controls
    CEikStatusPane* status_pane = StatusPane();
    iTitlePane = (CAknTitlePane *)
        status_pane->ControlL(TUid::Uid(EEikStatusPaneUidTitle));
#ifdef ENABLE_TOUCH_UI
    iTitlePane->SetTitlePaneObserver(this);
#endif
    
    CAknNavigationControlContainer *iNaviPane = (CAknNavigationControlContainer *)
        status_pane->ControlL(TUid::Uid(EEikStatusPaneUidNavi));
    if (iNaviPane) {
        iNaviLabel = iNaviPane->CreateNavigationLabelL();
        iNaviPane->PushL(*iNaviLabel);
#ifdef ENABLE_TOUCH_UI
        iNaviLabel->SetNaviDecoratorObserver(this);
#endif
    }
    
    iScreenMode = iAppConfig.GetScreenMode(
#ifdef ENABLE_TOUCH_UI
        EScreenLargeBottom
#else
        EScreenNormal
#endif
    );
    
    iAppView = CPuzzlesAppView::NewL(GetGameRect(), this);
    AddToStackL(iAppView);
    
    TBuf8<20> lastGame;
    iAppConfig.GetLastSelectedGame(lastGame);
    iAppView->PushLayoutL(CPuzzlesAppView::ELayoutGame);
    if (iAppView->GameContainer()->SelectGame(lastGame)) {
        iAppView->GameContainer()->LoadOrNewGame();
    } else {
        iAppView->PushLayoutL(CPuzzlesAppView::ELayoutGameList);
    }
    
    TInt orientation =
            iAppConfig.GetScreenOrientation(EAppUiOrientationAutomatic);
    
    switch (orientation) {
        case EAppUiOrientationPortrait:
        case EAppUiOrientationLandscape:
        case EAppUiOrientationAutomatic:
            SetOrientationL((TAppUiOrientation)orientation);
            break;
        default:
            break;
    }
}

// -----------------------------------------------------------------------------
// CPuzzlesAppUi::CPuzzlesAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CPuzzlesAppUi::CPuzzlesAppUi() {
    // No implementation required
}

// -----------------------------------------------------------------------------
// CPuzzlesAppUi::~CPuzzlesAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CPuzzlesAppUi::~CPuzzlesAppUi() {
    if (iNaviLabel) {
        delete iNaviLabel;
    }
    if (iAppView) {
        RemoveFromStack(iAppView);
        delete iAppView;
    }
#ifdef ENABLE_TOUCH_UI
    if (iPopupMenu) {
        delete iPopupMenu;
    }
#endif
}

void CPuzzlesAppUi::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane) {
    if (aResourceId == R_MENU) {
        // cascaded menu for settings for now
        //aMenuPane->SetItemDimmed(ECommandSettings, ETrue);
    }
    
    if (aResourceId == R_MENU_SCREEN) {
        TUint mode = ECommandScreenNormal;
        
        switch (iScreenMode) {
            case EScreenFull: mode = ECommandScreenFull; break;
            case EScreenLargeTop: mode = ECommandScreenLargeTop; break;
            case EScreenLargeBottom: mode = ECommandScreenLargeBottom; break;
            case EScreenNormal: mode = ECommandScreenNormal; break;
        }
        
        aMenuPane->SetItemButtonState(mode, EEikMenuItemSymbolOn);
    }
    if (aResourceId == R_MENU_ORIENTATION) {
        TUint mode = ECommandOrientationAutomatic;
        
        switch (Orientation()) {
            case EAppUiOrientationPortrait: mode = ECommandOrientationPortrait; break;
            case EAppUiOrientationLandscape: mode = ECommandOrientationLandscape; break;
            case EAppUiOrientationAutomatic: mode = ECommandOrientationAutomatic; break;
            default: break;
        }
        
        aMenuPane->SetItemButtonState(mode, EEikMenuItemSymbolOn);
    }
    
    iAppView->DynInitMenuPaneL(aResourceId, aMenuPane);
}

#ifdef ENABLE_TOUCH_UI
void CPuzzlesAppUi::HandleTitlePaneEventL(TInt aEventID) {
    if (aEventID == EAknTitlePaneTapped) {
        if (iAppView->GetCurrentLayout() == CPuzzlesAppView::ELayoutGame) {
            iAppView->PushLayoutL(CPuzzlesAppView::ELayoutGameList);
        }
    }
}
void CPuzzlesAppUi::HandleNaviDecoratorEventL(TInt aEventID) {
    if (aEventID == EAknNaviDecoratorEventNaviLabel) {
        if (!iAppView->GameContainer()->HaveGame()) {
            return;
        }
        
        if (iPopupMenu != NULL) {
            delete iPopupMenu;
        }
        
        TInt height = iNaviLabel->Size().iHeight;
        TPoint topLeft = iNaviLabel->PositionRelativeToScreen();
        TPoint bottomLeft = topLeft + TPoint(0, height);
        
        iPopupMenu = CAknStylusPopUpMenu::NewL(this, bottomLeft);
        iAppView->GameContainer()->BuildPresetPopupMenu(iPopupMenu);
        iPopupMenu->ShowMenu();
    }
}
#endif

// -----------------------------------------------------------------------------
// CPuzzlesAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CPuzzlesAppUi::HandleCommandL(TInt aCommand) {
    int preset = 0;
    if (aCommand >= ECommandGameTypePreset && aCommand < ECommandGameTypeCustom) {
        preset = aCommand - ECommandGameTypePreset;
        aCommand = ECommandGameTypePreset;
    }
    
    switch (aCommand) {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            iAppView->GameContainer()->SaveGame();
            
            Exit();
            break;
			
        case ECommandGameTypePreset: 
            iAppView->GameContainer()->SetGamePreset(preset);
            break;
            
        case ECommandGameTypeCustom: {
            iAppView->PushLayoutL(CPuzzlesAppView::ELayoutGameParameters);
            break;
        }
            
        case ECommandGameNew:
            iAppView->GameContainer()->NewGame();
            break;
            
        case ECommandGameRestart:
            iAppView->GameContainer()->RestartGame();
            break;
            
        case ECommandGameUndo:
            iAppView->GameContainer()->UndoMove();
            break;
            
        case ECommandGameRedo:
            iAppView->GameContainer()->RedoMove();
            break;
            
        case ECommandGameSolve:
            iAppView->GameContainer()->SolveGame();
            break;
            
        case ECommandScreenNormal:
            SetScreenMode(EScreenNormal);
            break;
            
        case ECommandScreenLargeTop:
            SetScreenMode(EScreenLargeTop);
            break;
            
        case ECommandScreenLargeBottom:
            SetScreenMode(EScreenLargeBottom);
            break;
            
        case ECommandScreenFull:
            SetScreenMode(EScreenFull);
            break;
            
        case ECommandOrientationAutomatic:
            SetOrientationL(EAppUiOrientationAutomatic);
            break;
            
        case ECommandOrientationPortrait:
            SetOrientationL(EAppUiOrientationPortrait);
            break;
            
        case ECommandOrientationLandscape:
            SetOrientationL(EAppUiOrientationLandscape);
            break;
            
        case KErrCancel:
            // touch ui: popup menu cancelled!
            break;
            
        default:
            if (!iAppView->HandleCommandL(aCommand)) {
                Panic(EPuzzlesPanicCommandHandler);
            }
            break;
    }
}

void CPuzzlesAppUi::SetOrientationL(TAppUiOrientation aOrientation) {
    iAppConfig.SetScreenOrientation(aOrientation);
    CAknAppUi::SetOrientationL(aOrientation);
}

void CPuzzlesAppUi::SetScreenMode(TInt aScreenMode) {
    if (iScreenMode != aScreenMode) {
        iScreenMode = aScreenMode;
        iAppConfig.SetScreenMode(iScreenMode);
        
        iAppView->GameContainer()->UpdateToolbarVisibility();
        HandleStatusPaneSizeChange();
    }
}

// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CPuzzlesAppUi::HandleStatusPaneSizeChange() {
    iAppView->SetRect(GetGameRect());
    iAppView->DrawDeferred();
}

TRect CPuzzlesAppUi::GetGameRect() {
    TRect appR(ApplicationRect());
    TRect gameR(ClientRect());
    
    CGameContainer *game = iAppView ? iAppView->GameContainer() : NULL;
    TInt screenMode = game && game->IsVisible() ? iScreenMode : EScreenNormal;
    
    switch (screenMode) {
        case EScreenFull:
            return appR;

        case EScreenLargeTop:
            gameR.iTl = appR.iTl;
            return gameR;

        case EScreenLargeBottom:
            gameR.iBr = appR.iBr;
            return gameR;

        default:
            return gameR;
    }
}

#include <hal.h>

// Used to position the help browser in the correctly.
// Update with other Machine Uids as needed.
TUint CPuzzlesAppUi::GetPlatformVersionL() {
    static TInt version = 0;
    
    if (version) {
        return version;
    }
    
#if defined(__WINSCW__) && !defined(__S60_50__)
    return 0x0901;
#endif
    
    TInt uid;
    HAL::Get(HAL::EMachineUid, uid);
    
    switch (uid) {
        case 0x200005F8: //  Nokia 3250
        case 0x20000602: //  Nokia 5500 Sport
        case 0x20002495: //  Nokia E50
        case 0x20001856: //  Nokia E60
        case 0x20001858: //  Nokia E61
        case 0x20002D7F: //  Nokia E61i
        case 0x20001859: //  Nokia E62
        case 0x20000604: //  Nokia E65
        case 0x20001857: //  Nokia E70
        case 0x200005FF: //  Nokia N71
        case 0x200005FB: //  Nokia N73
        case 0x200005FE: //  Nokia N75
        case 0x20000601: //  Nokia N77
        case 0x200005F9: //  Nokia N80
        case 0x200005FC: //  Nokia N91 & N91 8GB
        case 0x200005FA: //  Nokia N92
        case 0x20000600: //  Nokia N93
        case 0x20000605: //  Nokia N93i
        case 0x20008610: //  Samsung i400 [4] [5]            
            version = 0x0901;
            break;
            
        default:
            // assume at least 0x0902!
            version = 0x0902;
            break;
    }
    
#if 0
#ifdef __S60_50__
    
    // minimum expected version
    version = 0x0904;
    
#else
    // Connect to the file server session
    RFs fsSession;
    User::LeaveIfError(fsSession.Connect());
    CleanupClosePushL(fsSession);
    
    TFindFile fileFinder(fsSession);
    CDir* result;
    
    _LIT(KS60ProductIDFile, "Series60v*.sis");
    _LIT(KROMInstallDir, "z:\\system\\install\\");
    
    TInt error = fileFinder.FindWildByDir(
        KS60ProductIDFile,
        KROMInstallDir,
        result
    );
    User::LeaveIfError(error);
    
    CleanupStack::PushL(result);
    
    // Sort the file names so that the newest platforms are first
    User::LeaveIfError(result->Sort(ESortByName | EDescending));
    
    // Parse the version numbers from the file name (e.g. Series60v3_1.sis)
    TUint aMajor = (*result)[0].iName[9] - '0';
    TUint aMinor = (*result)[0].iName[11] - '0';
    CleanupStack::PopAndDestroy(result);
    
    CleanupStack::PopAndDestroy(&fsSession);
    
    version = aMajor << 8 | aMinor;
#endif
#endif
    
    return version;
}
// End of File
