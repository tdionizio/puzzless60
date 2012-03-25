
#ifndef __PUZZLESAPPVIEW_h__
#define __PUZZLESAPPVIEW_h__

// INCLUDES
#include "PuzzlesConfig.h"
#include <coecntrl.h>
#include <eiklbo.h>


class CPuzzlesAppUi;
class CGameContainer;
class CAknSettingItemList;
class CAknSingleLargeStyleListBox;
class CBrCtlInterface;

// CLASS DECLARATION
class CPuzzlesAppView: public CCoeControl,
    public MEikListBoxObserver,
    public MCoeControlObserver
{
public:
    // New methods
    static CPuzzlesAppView* NewL(const TRect& aRect, CPuzzlesAppUi *aAppUi);
    static CPuzzlesAppView* NewLC(const TRect& aRect, CPuzzlesAppUi *aAppUi);

    virtual ~CPuzzlesAppView();

public: // overrides
    void SizeChanged();
    void Draw(const TRect& aRect) const;
    
    void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType);
    void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);
    TKeyResponse OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType);
    
private:
    // Constructors
    CPuzzlesAppView();
    void ConstructL(const TRect& aRect, CPuzzlesAppUi *aAppUi);
    void ConstructGameContainerL();
    void ConstructGameListL();

    void ConstructGameParametersL();
    void ConstructHelpBrowserL();
    
    void FreeGameParameters();
    void FreeHelpBrowser();
    
    void ValidateAndSaveGameSettings();

public: // new methods
    
    TBool HandleCommandL(TInt aCommand);
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
    
    CGameContainer* GameContainer() { return iGameContainer; }
    
public:
    enum ELayout {
        ELayoutNone,
        ELayoutGame,
        ELayoutGameList,
        ELayoutGameSettings,
        ELayoutHelp
    };
    
    void SelectGame();
    TBool ShowHelp(const char *topic);
    
    void ShowGameView();
    void ShowGameList();
    void ShowGameSettings(TInt aIndex);
    
private:
    int iCurrentLayout;
    
    ELayout iLayoutStack[5];
    void EnterLayoutL(ELayout aLayout, TBool aCreate);
    void LeaveLayoutL();
    ELayout GetCurrentLayout() { return iLayoutStack[iCurrentLayout]; }
    void PushLayoutL(ELayout aLayout);
    void PopLayoutL();
    
private:
    CPuzzlesAppUi *iAppUi;
    CEikColumnListBox *iGameList;
    CGameContainer *iGameContainer;
    CBrCtlInterface* iHelpBr;
    
    CGameProfile *iGameProfiles;
    CGameProfile *iCurrentGameProfile;
    
    enum {
        KMaxTextLength = 50
    };
    typedef struct {
        union {
            RBuf *iStr;
            TInt iInt;
            TBool iBool;
        } v;
    } TConfigValue;
    
    config_item *iConfig;
    TInt iConfigLen;
    TConfigValue *iConfigValues;
    TBuf<KMaxTextLength> iConfigName;
    TBool iConfigDelete;
    // profile being edited
    CGameProfile *iConfigProfile;
    
    CAknSettingItemList* iGameSettings;
};

#endif // __PUZZLESAPPVIEW_h__
// End of File
