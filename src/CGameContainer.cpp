
// INCLUDE FILES
#include "PuzzlesConfig.h"

#include <aknutils.h>
#include <aknnotewrappers.h>
#include <coemain.h>
#include <eiklabel.h>
#ifdef ENABLE_TOUCH_UI
#include <aknbutton.h>
#include <aknstyluspopupmenu.h>
#endif

#include <string.h>

#include "Puzzles_0xa000ef77.rsg"
#include "Puzzles.hrh"

#include "CGameView.h"
#include "CGameContainer.h"
#include "PuzzlesAppUi.h"
#include "PuzzlesDrawingApi.h"

extern "C" {
#include "puzzles.h"
}

#include "_puzzles.inc.h"


static void show_error(char *error) {
    TBuf<100> bError;
    bError.Copy(TPtrC8((TUint8*)error));
    
    CAknErrorNote *dlg = new (ELeave) CAknErrorNote(ETrue);
    dlg->ExecuteLD(bError);
}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CGameContainer::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGameContainer* CGameContainer::NewL(const CCoeControl &aParent, CPuzzlesAppUi *aAppUi) {
    CGameContainer* self = CGameContainer::NewLC(aParent, aAppUi);
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
// CGameContainer::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGameContainer* CGameContainer::NewLC(const CCoeControl &aParent, CPuzzlesAppUi *aAppUi) {
    CGameContainer* self = new (ELeave) CGameContainer;
    CleanupStack::PushL(self);
    self->ConstructL(aParent, aAppUi);
    return self;
}

// -----------------------------------------------------------------------------
// CGameContainer::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CGameContainer::ConstructL(const CCoeControl &aParent, CPuzzlesAppUi *aAppUi) {
    // Create a window for this application view
    SetContainerWindowL(aParent);
    InitComponentArrayL();
    
    iAppUi = aAppUi;
    iTimer = CPeriodic::NewL(0);
    iME = NULL;
    iGame = NULL;
    iGameStarted = EFalse;
    
#ifdef ENABLE_TOUCH_UI
    iStylusMode = EStylusLeft;
    iTap = ETapNone;
    iTapTimer = CPeriodic::NewL(0);
#endif
    
    // Create view object
    iGameView = CGameView::NewL(this);
    Components().AppendLC(iGameView);
    CleanupStack::Pop(iGameView);
    
    iStatusView = new (ELeave) CEikLabel();
    Components().AppendLC(iStatusView);
    CleanupStack::Pop(iStatusView);
    iStatusView->SetContainerWindowL(*this);
    iStatusView->SetFont(iEikonEnv->LegendFont());
    iStatusView->SetTextL(KNullDesC);
    iStatusView->MakeVisible(EFalse);
    
    
#ifdef ENABLE_TOUCH_UI
    // Create Buttons
    CAknButton *btn;
    TBuf<1> letter;
    letter.SetLength(1);
    
    _LIT(KBtnMenu, "Menu");
    _LIT(KBtnUndo, "Undo Move");
    _LIT(KBtnRedo, "Redo Move");
    _LIT(KBtnStylus, "Alternate Stylus Mode");
    
    TUint labels[] = {
        '1', '2', '3', '4', '5',
        '6', '7', '8', '9', '0',
        'U', 'R', 'M', 'S'
    };
    const TDesC* tooltips[] = {
        &KNullDesC, &KNullDesC, &KNullDesC, &KNullDesC, &KNullDesC, 
        &KNullDesC, &KNullDesC, &KNullDesC, &KNullDesC, &KNullDesC, 
        &KBtnUndo, &KBtnRedo, &KBtnMenu, &KBtnStylus
    };
    for (int i = 0; i < EMaxButtons; ++i) {
        letter[0] = labels[i];
        iButtons[i] = btn = CAknButton::NewL(
            NULL, NULL, NULL, NULL,
            letter, *tooltips[i],
            KAknButtonTextInsideFrame,
            0
        );
        Components().AppendLC(btn);
        
        btn->SetContainerWindowL(*this);
        btn->SetObserver(this);
        btn->MakeVisible(EFalse);
        
#ifdef __WINSCW__
        // emulator shows white text on light blue background...
        btn->OverrideColorL(EColorButtonText, KRgbBlack.Value());
#endif
        
        CleanupStack::Pop(btn);
    }
#endif
}

// -----------------------------------------------------------------------------
// CGameContainer::CGameContainer()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CGameContainer::CGameContainer() {
}

// -----------------------------------------------------------------------------
// CGameContainer::~CGameContainer()
// Destructor.
// -----------------------------------------------------------------------------
//
CGameContainer::~CGameContainer() {
    if (iColourMap) {
        delete [] iColourMap;
        iColourMap = NULL;
    }
    if (iME) {
        midend_free(iME);
        iME = NULL;
    }
    if (iTimer) {
        delete iTimer;
        iTimer = NULL;
    }
#ifdef ENABLE_TOUCH_UI
    if (iTapTimer) {
        delete iTapTimer;
        iTapTimer = NULL;
    }
#endif
}

void CGameContainer::Draw(const TRect& aRect) const {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();
    float bg[3];
    frontend_default_colour(NULL, bg);
    
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.SetBrushColor(TRgb(
        (TInt)(255 * bg[0]),
        (TInt)(255 * bg[1]),
        (TInt)(255 * bg[2])
    ));
    gc.DrawRect(aRect);
}
// -----------------------------------------------------------------------------
// CGameContainer::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CGameContainer::SizeChanged() {
    const TUint controlPad = 5;
    TRect screen = Rect();
    
#ifdef ENABLE_TOUCH_UI
    // buttons
    if (iButtons[0]->IsVisible()) {
        // 7 buttons per row, with spaces after the 2 buttons at the sides
        // button rect is squared
        
        if (screen.Width() > screen.Height()) {
            // landscape
            
            TUint h = screen.Height();
            TUint bh = (h - 10) / 7;
            TUint pad = (h - bh * 7) / 2;
            
            TRect btnRect(
                TPoint(screen.iBr.iX - bh * 2, screen.iBr.iY - bh),
                TSize(bh, bh)
            );
            iButtons[EButtonUndo]->SetRect(btnRect);
            
            btnRect.Move(bh, 0);
            iButtons[EButtonMenu]->SetRect(btnRect);
            
            btnRect.Move(0, - h + bh);
            iButtons[EButtonStylus]->SetRect(btnRect);
            
            btnRect.Move(- bh, 0);
            iButtons[EButtonRedo]->SetRect(btnRect);
            
            btnRect.Move(0, h - bh - bh - pad);
            
            for (int btn = EButton1; btn <= EButton0; ++btn) {
                if (btn == EButton6) {
                    btnRect.Move(bh, + bh * 5);
                }
                iButtons[btn]->SetRect(btnRect);
                btnRect.Move(0, -bh);
            }
            
            screen.iBr.iX += - bh * 2 - controlPad;
        } else {
            // portrait
            
            TUint w = screen.Width();
            TUint bw = (w - 10) / 7;
            TUint pad = (w - bw * 7) / 2;
            
            TRect btnRect(
                TPoint(screen.iTl.iX, screen.iBr.iY - bw * 2),
                TSize(bw, bw)
            );
            
            iButtons[EButtonUndo]->SetRect(btnRect);
            btnRect.Move(0, bw);
            iButtons[EButtonMenu]->SetRect(btnRect);
            btnRect.Move(w - bw, 0);
            iButtons[EButtonStylus]->SetRect(btnRect);
            btnRect.Move(0, - bw);
            iButtons[EButtonRedo]->SetRect(btnRect);
            btnRect.Move(- w + bw + bw + pad, 0);
            
            for (int btn = EButton1; btn <= EButton0; ++btn) {
                if (btn == EButton6) {
                    btnRect.Move(- bw * 5, bw);
                }
                iButtons[btn]->SetRect(btnRect);
                btnRect.Move(bw, 0);
            }
            
            screen.iBr.iY += - bw * 2 - controlPad;
        }
    }
#endif
    
    // status text
    if (iStatusView->IsVisible()) {
        const TUint padding = 2;
        const CFont *statusFont = iStatusView->Font();
        TUint statusHeight = statusFont->FontMaxHeight() + padding * 2;
        
        TRect statusRect(
            TPoint(
                screen.iTl.iX + padding,
                screen.iBr.iY - statusHeight + 1
            ),
            screen.iBr
        );
        iStatusView->SetRect(statusRect);
        
        // update available area
        screen.iBr.iY += - statusHeight - controlPad;
    }
    
    // use the remaining visible area for the game view
    TRect gameRect(screen);
    iGameView->SetRect(gameRect);
    ResizeGame(EFalse);
}

void CGameContainer::UpdateToolbarVisibility() {
#ifdef ENABLE_TOUCH_UI
    int visible = iAppUi->GetScreenMode() == CPuzzlesAppUi::EScreenFull
        || iAppUi->GetScreenMode() == CPuzzlesAppUi::EScreenLargeBottom
        || (iGame && (iGame->flags & (REQUIRE_RBUTTON | REQUIRE_NUMPAD)));
    
    TBool bVisible = visible ? ETrue : EFalse;
    
    if (bVisible != iButtons[0]->IsVisible()) {
        for (int i = 0; i < EMaxButtons; ++i) {
            iButtons[i]->MakeVisible(bVisible);
        }
        SizeChanged();
    }
#endif
}
void CGameContainer::UpdateStatusVisibility() {
    TBool bVisible = midend_wants_statusbar(iME) ? ETrue : EFalse;
    
    if (bVisible != iStatusView->IsVisible()) {
        iStatusView->MakeVisible(bVisible);
        SizeChanged();
    }
}

#ifdef ENABLE_TOUCH_UI
void CGameContainer::ToggleStylusMode() {
    TBuf<1> state;
    state.SetLength(1);
    
    if (iStylusMode == EStylusLeft) {
        state[0] = '/';
        iStylusMode = EStylusRight;
    } else {
        state[0] = 'S';
        iStylusMode = EStylusLeft;
    }
    
    iButtons[EButtonStylus]->State()->SetTextL(state);
    iButtons[EButtonStylus]->DrawDeferred();
}
#endif

void CGameContainer::SetStatusText(const TDesC &aText) {
    iStatusView->SetTextL(aText);
    iStatusView->DrawDeferred();
}

void CGameContainer::HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType) {
#ifndef ENABLE_TOUCH_UI
    (void)aControl;
    (void)aEventType;
#else
    TUint key;
    
    if (aEventType != EEventStateChanged) {
        return;
    }
    
    // find button
    int i;
    for (i = 0; i < EMaxButtons; ++i) {
        if (aControl == iButtons[i]) {
            break;
        }
    }
    
    if (i == EMaxButtons) {
        return;
    }
    
    switch (i) {
        case EButtonMenu:
            iAppUi->ProcessCommandL(EAknSoftkeyOptions);
            // iEikonEnv->AppUiFactory()->MenuBar()->TryDisplayMenuBarL();
            return;
            
        case EButtonStylus:
            ToggleStylusMode();
            return;
            
        case EButtonUndo:
            key = '*';
            break;
            
        case EButtonRedo:
            key = '#';
            break;
            
        default:
            // numbered keys
            key = iButtons[i]->State()->Text()[0];
            break;
    }
    
    TKeyEvent event;
    event.iCode = key;
    event.iModifiers = 0;
    event.iRepeats = 0;
    event.iScanCode = 0;
    
    OfferKeyEventL(event, EEventKey);
#endif
}

#ifdef ENABLE_TOUCH_UI
void CGameContainer::BuildPresetPopupMenu(CAknStylusPopUpMenu *aPopupMenu) {
    int i;
    int n = midend_num_presets(iME);
    int which = midend_which_preset(iME);
    
    _LIT(KSelBegin, "[");
    _LIT(KSelEnd, "]");
    _LIT(KCustom, "Custom...");
    TBuf<40> name;
    
    for (i = 0; i < n; ++i) {
        char *cname;
        game_params *params;
        midend_fetch_preset(iME, i, &cname, &params);
        
        TPtrC8 name8((TUint8*)cname);
        name.Copy(name8);
        
        if (i == which) {
            name.Insert(0, KSelBegin);
            name.Append(KSelEnd);
        }
        
        aPopupMenu->AddMenuItemL(name, ECommandGameTypePreset + i);
    }
    
    if (iGame->can_configure) {
        name.Copy(KCustom);
        
        if (which == -1) {
            name.Insert(0, KSelBegin);
            name.Append(KSelEnd);
        }
        
        aPopupMenu->AddMenuItemL(name, ECommandGameTypeCustom);
    }
}
#endif

void CGameContainer::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane) {
    if (aResourceId == R_MENU) {
        if (!iME) {
            aMenuPane->SetItemDimmed(ECommandGameNew, ETrue);
            aMenuPane->SetItemDimmed(ECommandGameType, ETrue);
            aMenuPane->SetItemDimmed(ECommandGame, ETrue);
        }
        else if (!iGame->can_configure && midend_num_presets(iME) == 0) {
            aMenuPane->SetItemDimmed(ECommandGameType, ETrue);
        }
    }
    
    if (aResourceId == R_MENU_GAME) {
        if (!iME || !iGame->can_solve) {
            aMenuPane->SetItemDimmed(ECommandGameSolve, ETrue);
        }
    }
    
    if (aResourceId == R_MENU_GAME_TYPE) {
        int i;
        int n = midend_num_presets(iME);
        int which = midend_which_preset(iME);
        
        CEikMenuPaneItem::SData item;
        item.iCascadeId = 0;
        item.iFlags = EEikMenuItemCheckBox;
        item.iExtraText.Copy(KNullDesC);
        
        for (i = 0; i < n; ++i) {
            char *cname;
            game_params *params;
            midend_fetch_preset(iME, i, &cname, &params);
            
            TPtrC8 name8((TUint8*)cname);
            item.iText.Copy(name8);
            item.iCommandId = ECommandGameTypePreset + i;
            if (which == i) {
                item.iFlags |= EEikMenuItemSymbolOn;
            } else {
                item.iFlags &= ~EEikMenuItemSymbolOn;
            }
            aMenuPane->AddMenuItemL(item);
        }
        
        if (iGame->can_configure) {
            _LIT(KCustom, "Custom...");
            
            item.iText.Copy(KCustom);
            item.iCommandId = ECommandGameTypeCustom;
            if (which == -1) {
                item.iFlags |= EEikMenuItemSymbolOn;
            } else {
                item.iFlags &= ~EEikMenuItemSymbolOn;
            }
            aMenuPane->AddMenuItemL(item);
        }
    }
}

#ifdef ENABLE_TOUCH_UI
void CGameContainer::ProcessTap(TInt aButton, const TPoint &aScreenLocation, const TPoint &aGamePos) {
    midend_process_key(
        iME,
        aScreenLocation.iX - aGamePos.iX,
        aScreenLocation.iY - aGamePos.iY,
        aButton
    );
}

TInt CGameContainer::LongTapTick(TAny *aObject) {
    CGameContainer *self = (CGameContainer*)aObject;
    self->iTapTimer->Cancel();
    self->ToggleStylusMode();
    self->iTap = ETapLong;
    return 1;
}

void CGameContainer::HandleGamePointerEventL(
    const TPointerEvent &aPointerEvent,
    const TPoint &aGamePos
) {
    if (!iME) {
        return;
    }
    
    int btn;
    
    switch (aPointerEvent.iType) {
        case TPointerEvent::EButton1Down:
            iTap = ETapWait;
            iFirstTapPosition = aPointerEvent.iPosition;
            
            iTapTimer->Cancel();
            iTapTimer->Start(250000, 250000, TCallBack(LongTapTick, this));
            break;
            
        case TPointerEvent::EDrag:
            TBool canDrag = iTap == ETapDrag;
            
            if (!canDrag) {
                if (iTap == ETapWait) {
                    TInt x = iFirstTapPosition.iX - aPointerEvent.iPosition.iX;
                    TInt y = iFirstTapPosition.iY - aPointerEvent.iPosition.iY;
                    
                    if (Abs(x) + Abs(y) > 5) {
                        canDrag = ETrue;
                    }
                }
            }
            
            if (canDrag) {
                if (iTap == ETapWait) {
                    iTapTimer->Cancel();
                    
                    btn = (iStylusMode == EStylusLeft ? LEFT_BUTTON : RIGHT_BUTTON);
                    ProcessTap(btn, iFirstTapPosition, aGamePos);
                    iTap = ETapDrag;
                }
                
                if (iTap == ETapDrag) {
                    btn = (iStylusMode == EStylusLeft ? LEFT_DRAG : RIGHT_DRAG);
                    ProcessTap(btn, aPointerEvent.iPosition, aGamePos);
                }
            }
            break;
            
        case TPointerEvent::EButton1Up:
            if (iTap != ETapLong) {
                if (iTap == ETapWait) {
                    iTapTimer->Cancel();
                    
                    btn = (iStylusMode == EStylusLeft ? LEFT_BUTTON : RIGHT_BUTTON);
                    ProcessTap(btn, iFirstTapPosition, aGamePos);
                }
                
                btn = (iStylusMode == EStylusLeft ? LEFT_RELEASE : RIGHT_RELEASE);
                ProcessTap(btn, aPointerEvent.iPosition, aGamePos);
            }
            
            iTap = ETapNone;
            break;
            
        default:
            break;
    }
}
#endif

TKeyResponse CGameContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType) {
    if (iME == NULL) {
        return EKeyWasNotConsumed;
    }
    
    // PEN key maps to SELECT2
    if (aType == EEventKeyDown 
        && aKeyEvent.iCode == 0 
        && (aKeyEvent.iModifiers & EModifierShift) != 0
    ) {
        midend_process_key(
            iME, 0, 0, CURSOR_SELECT2
        );
        
        return EKeyWasConsumed;
    }

    if (aType != EEventKey) {
        return EKeyWasNotConsumed;
    }
    
    int key;
    TUint code = aKeyEvent.iCode;
    
    switch (code) {
        case EKeyLeftArrow: key = CURSOR_LEFT; break;
        case EKeyRightArrow: key = CURSOR_RIGHT; break;
        case EKeyUpArrow: key = CURSOR_UP; break;
        case EKeyDownArrow: key = CURSOR_DOWN; break;
        
        case EKeyOK:
        case EKeyEnter: key = CURSOR_SELECT; break;
        
        case EKeyBackspace: key = CURSOR_SELECT2; break;
        
        case '*': key = '\x1A'; break;
        case '#': key = '\x12'; break;
        
        default:
            if ((code >= '0' && code <= '9')
                || (code >= 'a' && code <= 'z')
                || (code >= 'A' && code <= 'Z')) {
                
                key = (int)code;
            } else {
                return EKeyWasNotConsumed;
            }
    }
    
    midend_process_key(
        iME, 0, 0, key
    );
    
    return EKeyWasConsumed;
}
TBool CGameContainer::SelectGame(const TDesC8 &aGameName) {
    for (int i = 0; i < gamecount; ++i) {
        if (aGameName.Compare(TPtrC8((TUint8*)gamelist[i]->name)) == 0) {
            SetGame(gamelist[i]);
            return ETrue;
        }
    }
    
    return EFalse;
}

void CGameContainer::UpdateGameTitle() {
    char *cpreset_name;
    int preset_index;
    
    // force to load preset list from game
    midend_num_presets(iME);
    // check selected preset
    preset_index = midend_which_preset(iME);
    
    if (preset_index >= 0) {
        game_params *params;
        midend_fetch_preset(iME, preset_index, &cpreset_name, &params);
    } else {
        cpreset_name = "custom";
    }
    
    int game_name_len = strlen(iGame->name);
    int preset_name_len = strlen(cpreset_name);
    
    TBuf<30> title;
    TBuf<30> preset;
    
    if (game_name_len > title.MaxLength()) {
        game_name_len = title.MaxLength();
    }
    if (preset_name_len > preset.MaxLength()) {
        preset_name_len = preset.MaxLength();
    }

    title.Copy(TPtrC8((TUint8 *)iGame->name, game_name_len));
    preset.Copy(TPtrC8((TUint8 *)cpreset_name, preset_name_len));
    iAppUi->ChangePaneTextL(title, preset);
}

void CGameContainer::SetGame(const game *aGame) {
    if (iME) {
        midend_free(iME);
        iME = NULL;
        delete [] iColourMap;
        iColourMap = NULL;
    }
    
    iGame = aGame;
    iGameStarted = EFalse;
    
    if (aGame) {
        
        iME = midend_new(
            (frontend*)this,
            iGame,
            CPuzzlesDrawingApi::Api(),
            iGameView
        );
        
        int ncolours;
        float *cl = midend_colours(iME, &ncolours);
        iColourMap = new (ELeave) TRgb[ncolours];
        int i;
        for (i = 0; i < ncolours; ++i) {
            iColourMap[i] = TRgb(
                (TInt)(255 * cl[i * 3 + 0]),
                (TInt)(255 * cl[i * 3 + 1]),
                (TInt)(255 * cl[i * 3 + 2])
            );
        }
        sfree(cl);
        
        SetStatusText(KNullDesC);
        UpdateStatusVisibility();
        UpdateToolbarVisibility();
    }
}

void CGameContainer::NewGame() {
    midend_new_game(iME);
    UpdateGameTitle();
    iGameStarted = ETrue;
    ResizeGame(ETrue);
}

void CGameContainer::RestartGame() {
    if (iME) {
        midend_restart_game(iME);
    }
}

void CGameContainer::UndoMove() {
    if (iME) {
        midend_process_key(iME, 0, 0, '\x1A');
    }
}

void CGameContainer::RedoMove() {
    if (iME) {
        midend_process_key(iME, 0, 0, '\x12');
    }
}
void CGameContainer::SolveGame() {
    char *msg = midend_solve(iME);
    if (msg) {
        show_error(msg);
    }
}

void CGameContainer::SetGamePreset(TInt aPresetIndex) {
    if (aPresetIndex < midend_num_presets(iME)) {
        char *cname;
        game_params *params;
        midend_fetch_preset(iME, aPresetIndex, &cname, &params);
        midend_set_params(iME, params);
        NewGame();
    }
}

char *CGameContainer::SetGameConfig(config_item *aConfig) {
    char *error = midend_set_config(iME, CFG_SETTINGS, aConfig);
    if (error == NULL) {
        NewGame();
    }
    return error;
}

config_item *CGameContainer::GetGameConfig() {
    char *wintitle;
    config_item *cfg = midend_get_config(iME, CFG_SETTINGS, &wintitle);
    sfree(wintitle);
    return cfg;
}

void CGameContainer::ResizeGame(TBool aForceRedraw) {
    if (iME && iGameStarted) {
        TSize gameSize = iGameView->Size();
        
        // adjust to include a border around the game
        gameSize -= TSize(2, 2);
        
        midend_size(iME, &gameSize.iWidth, &gameSize.iHeight, 1);
        if (iGameView->SetGameSize(gameSize) || aForceRedraw) {
            midend_redraw(iME);
        }
    }
}


void CGameContainer::StartTimer() {
    const TInt interval = 20000;
    iTimer->Cancel();
    iTimer->Start(interval, interval, TCallBack(TimerTick, this));
    iTimerTick.UniversalTime();
}
void CGameContainer::EndTimer() {
    iTimer->Cancel();
}

TInt CGameContainer::TimerTick(TAny *aObject) {
    CGameContainer *self = (CGameContainer*) aObject;
    TTime lastTick = self->iTimerTick;
    self->iTimerTick.UniversalTime();
    TTimeIntervalMicroSeconds diff = self->iTimerTick.MicroSecondsFrom(lastTick);
    float elapsed = (float)diff.Int64() / 1000000;
    midend_timer(self->iME, elapsed);
    return 1;
}

void CGameContainer::LoadOrNewGame() {
    if (!LoadGame()) {
        NewGame();
    }
}
void CGameContainer::SaveGame() {
    if (iGame != NULL) {
        iAppUi->AppConfig().SaveGame(iME, iGame);
    }
}
TBool CGameContainer::LoadGame() {
    char *errmsg = iAppUi->AppConfig().LoadGame(iME, iGame);
    if (errmsg) {
        if (errmsg[0]) {
            show_error(errmsg);
        }
        return EFalse;
    }
    UpdateGameTitle();
    iGameStarted = ETrue;
    ResizeGame(ETrue);
    return ETrue;
}



// End of File
