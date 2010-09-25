
#ifndef __PUZZLESCGAMECONTAINER_h__
#define __PUZZLESCGAMECONTAINER_h__

// INCLUDES
#include "PuzzlesConfig.h"
#include <coecntrl.h>

class CPuzzlesAppUi;
class CGameView;
class CEikLabel;
class CAknButton;
class CEikMenuPane;

typedef struct game game;
typedef struct game_params game_params;
typedef struct midend midend;
typedef struct config_item config_item;

// CLASS DECLARATION
class CGameContainer: public CCoeControl, public MCoeControlObserver {
    
public:
    // New methods
    static CGameContainer* NewL(const CCoeControl &aParent, CPuzzlesAppUi *aAppUi);
    static CGameContainer* NewLC(const CCoeControl &aParent, CPuzzlesAppUi *aAppUi);

    virtual ~CGameContainer();

private:
    // Constructors
    void ConstructL(const CCoeControl &aParent, CPuzzlesAppUi *aAppUi);
    CGameContainer();

    
public: // from CCoeControl
    void SizeChanged();
    void Draw(const TRect& aRect) const;
    
public:
    // from MCoeControlObserver
    virtual void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType);

private:
#ifdef ENABLE_TOUCH_UI
    enum {
        EButton1,
        EButton2,
        EButton3,
        EButton4,
        EButton5,
        EButton6,
        EButton7,
        EButton8,
        EButton9,
        EButton0,
        EButtonUndo,
        EButtonRedo,
        EButtonMenu,
        EButtonStylus,
        EMaxButtons
    };
    
    CAknButton* iButtons[EMaxButtons];
    
    typedef enum {
        EStylusLeft,
        EStylusRight,
    } TStylusMode;
    
    TInt iStylusMode;
#endif
    
    CPuzzlesAppUi *iAppUi;
    CGameView *iGameView;
    CEikLabel *iStatusView;
    
    TBool iGameStarted;
    const game *iGame;
    midend *iME;
    TRgb *iColourMap;
    
    void ResizeGame(TBool aForceRedraw);
    
    
    CPeriodic* iTimer;
    TTime iTimerTick;
    
    static TInt TimerTick(TAny *aObject);
    
public: // new methods
    
    TBool HaveGame() { return iGame ? ETrue : EFalse; }
    void SetGame(const game *aGame);
    TBool SelectGame(const TDesC8 &aGameName);
    const game *GetGame() const { return iGame; }
    void UpdateGameTitle();
    
    void SaveGame();
    TBool LoadGame();
    
    void LoadOrNewGame();
    void NewGame();
    void SetGamePreset(TInt aPresetIndex);
    char *SetGameConfig(config_item *aConfig);
    config_item *GetGameConfig();
    
    void RestartGame();
    void UndoMove();
    void RedoMove();
    void SolveGame();
    
    void UpdateToolbarVisibility();
    void UpdateStatusVisibility();
    
    TBool ToolbarVisible();
    TBool StatusViewVisible();
    
    void ToggleStylusMode();
    void SetStatusText(const TDesC &aText);
    
    TRgb MapColour(TInt aColourIndex) { return iColourMap[aColourIndex]; }
    
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
    TKeyResponse OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType);
    void HandleGamePointerEventL(const TPointerEvent &aPointerEvent, const TPoint &aGamePos);
    
    void StartTimer();
    void EndTimer();
};

#endif // __PUZZLESCGAMECONTAINER_h__
// End of File
