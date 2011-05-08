
// INCLUDE FILES
#include "PuzzlesConfig.h"

#include <aknutils.h>
#include <coemain.h>
#include <eiklabel.h>
#include <aknlists.h>
#include <eikclbd.h>
#include <gulicon.h>
#include <aknsettingitemlist.h>
#include <aknnotewrappers.h>
#include <brctlinterface.h>

#include <string.h>

#include "Puzzles.hrh"
#include "Puzzles.pan"
#include "CGameView.h"
#include "CGameContainer.h"
#include "PuzzlesApplication.h"
#include "PuzzlesAppUi.h"
#include "PuzzlesAppView.h"

#include "Puzzles_0xa000ef77.rsg"
#include "puzzles_0xa000ef77.mbg"

extern "C" {
#include "puzzles.h"
}

#include "_puzzles.inc.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPuzzlesAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPuzzlesAppView* CPuzzlesAppView::NewL(const TRect& aRect, CPuzzlesAppUi *aAppUi) {
    CPuzzlesAppView* self = CPuzzlesAppView::NewLC(aRect, aAppUi);
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
// CPuzzlesAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPuzzlesAppView* CPuzzlesAppView::NewLC(const TRect& aRect, CPuzzlesAppUi *aAppUi) {
    CPuzzlesAppView* self = new (ELeave) CPuzzlesAppView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect, aAppUi);
    return self;
}

// -----------------------------------------------------------------------------
// CPuzzlesAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPuzzlesAppView::ConstructL(const TRect& aRect, CPuzzlesAppUi *aAppUi) {
    // Create a window for this application view
    CreateWindowL();
    InitComponentArrayL();
    
    iAppUi = aAppUi;
    iCurrentLayout = 0;
    iLayoutStack[iCurrentLayout] = ELayoutNone;
    
    ConstructGameContainerL();
    ConstructGameListL();
    
    // Set the windows size
    SetRect(aRect);
    
    RDebug::Printf("Version: %04x", iAppUi->GetPlatformVersionL());
    
    // Activate the window, which makes it ready to be drawn
    ActivateL();
}

void CPuzzlesAppView::ConstructGameContainerL() {
    iGameContainer = CGameContainer::NewL(*this, iAppUi);
    Components().AppendLC(iGameContainer);
    CleanupStack::Pop(iGameContainer);
    iGameContainer->MakeVisible(EFalse);
}

void CPuzzlesAppView::ConstructGameListL() {
    iGameList = new (ELeave) CAknSingleLargeStyleListBox;
    Components().AppendLC(iGameList);
    CleanupStack::Pop(iGameList);
    
    iGameList->ConstructL(this, EAknListBoxSelectionList);
    iGameList->CreateScrollBarFrameL(ETrue);
    iGameList->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
    
    CArrayPtr<CGulIcon> *icons = new (ELeave) CArrayPtrFlat<CGulIcon>(gamecount);
    CleanupStack::PushL(icons);
    
    TMifPuzzles_0xa000ef77 bmps[] = { EMbmPuzzles_0xa000ef77Blackbox,
        EMbmPuzzles_0xa000ef77Bridges, EMbmPuzzles_0xa000ef77Cube,
        EMbmPuzzles_0xa000ef77Dominosa, EMbmPuzzles_0xa000ef77Fifteen,
        EMbmPuzzles_0xa000ef77Filling, EMbmPuzzles_0xa000ef77Flip,
        EMbmPuzzles_0xa000ef77Galaxies, EMbmPuzzles_0xa000ef77Guess,
        EMbmPuzzles_0xa000ef77Inertia, EMbmPuzzles_0xa000ef77Keen,
        EMbmPuzzles_0xa000ef77Lightup, EMbmPuzzles_0xa000ef77Loopy,
        EMbmPuzzles_0xa000ef77Magnets, EMbmPuzzles_0xa000ef77Map,
        EMbmPuzzles_0xa000ef77Mines, EMbmPuzzles_0xa000ef77Net,
        EMbmPuzzles_0xa000ef77Netslide, EMbmPuzzles_0xa000ef77Pattern,
        EMbmPuzzles_0xa000ef77Pegs, EMbmPuzzles_0xa000ef77Range,
        EMbmPuzzles_0xa000ef77Rect,
        EMbmPuzzles_0xa000ef77Samegame, EMbmPuzzles_0xa000ef77Signpost,
        EMbmPuzzles_0xa000ef77Singles,
        EMbmPuzzles_0xa000ef77Sixteen, EMbmPuzzles_0xa000ef77Slant,
        EMbmPuzzles_0xa000ef77Solo, EMbmPuzzles_0xa000ef77Tents,
        EMbmPuzzles_0xa000ef77Towers, EMbmPuzzles_0xa000ef77Twiddle,
        EMbmPuzzles_0xa000ef77Unequal, EMbmPuzzles_0xa000ef77Untangle,
        EMbmPuzzles_0xa000ef77LastElement };
    for (TUint i = 0; bmps[i] != EMbmPuzzles_0xa000ef77LastElement; ++i) {
        icons->AppendL(CGulIcon::NewL(AknIconUtils::CreateIconL(KMBM, bmps[i])));
    }
    
    iGameList->ItemDrawer()->ColumnData()->SetIconArray(icons);
    CleanupStack::Pop(icons);
    
    CDesCArrayFlat* ItemArray = new (ELeave) CDesCArrayFlat(gamecount);
    CleanupStack::PushL(ItemArray);
    
    _LIT(KNameFmt, "%d\t%S\t");
    TBuf<50> name;
    TBuf<50> namef;
    
    // add the item adding in here..
    for (int i = 0; i < gamecount; ++i) {
        name.Copy(TPtrC8((TUint8*)gamelist[i]->name));
        namef.Format(KNameFmt, i, &name);
        ItemArray->AppendL(namef);
    }
    
    CleanupStack::Pop(ItemArray);            //ItemArray
    iGameList->Model()->SetItemTextArray(ItemArray);
    iGameList->Model()->SetOwnershipType(ELbmOwnsItemArray);
    
    iGameList->SetListBoxObserver(this);
    iGameList->MakeVisible(EFalse);
}

void CPuzzlesAppView::ConstructHelpBrowserL() {
    TUint v = iAppUi->GetPlatformVersionL();
    
    TRect rect = TRect(
        (v < 0x0902 ? Position() : TPoint()),
        Size()
    );
    
    iHelpBr = CreateBrowserControlL(
        this,
        rect,
        TBrCtlDefs::ECapabilityDisplayScrollBar,
        TBrCtlDefs::ECommandIdBase
    );
    Components().AppendLC(iHelpBr);
    CleanupStack::Pop(iHelpBr);
    iHelpBr->ActivateL();
}


void CPuzzlesAppView::FreeHelpBrowser() {
    Components().Remove(iHelpBr);
    delete iHelpBr;
    iHelpBr = NULL;
}

// -----------------------------------------------------------------------------
// CPuzzlesAppView::CPuzzlesAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CPuzzlesAppView::CPuzzlesAppView() {
    // No implementation required
}

// -----------------------------------------------------------------------------
// CPuzzlesAppView::~CPuzzlesAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CPuzzlesAppView::~CPuzzlesAppView() {
    FreeHelpBrowser();
    FreeGameParameters();
}

void CPuzzlesAppView::Draw(const TRect& aRect) const {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();
    
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.SetBrushColor(KRgbWhite);
    gc.Clear(aRect);
}
// -----------------------------------------------------------------------------
// CPuzzlesAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CPuzzlesAppView::SizeChanged() {
    TRect rect = Rect();
    
    iGameContainer->SetRect(rect);
    iGameList->SetRect(rect);
    if (iHelpBr) {
        TUint v = iAppUi->GetPlatformVersionL();

        TRect rect = TRect(
            (v < 0x0902 ? Position() : TPoint()),
            Size()
        );
        
        iHelpBr->SetRect(rect);
    }
    if (iGameSettings) {
        iGameSettings->SetRect(rect);
        iGameSettings->ListBox()->SetRect(rect);
    }
}

void CPuzzlesAppView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane) {
    iGameContainer->DynInitMenuPaneL(aResourceId, aMenuPane);
    
    if (aResourceId == R_MENU) {
        if (iGameList->IsVisible()) {
            aMenuPane->SetItemDimmed(ECommandGameNew, ETrue);
            aMenuPane->SetItemDimmed(ECommandGameType, ETrue);
            aMenuPane->SetItemDimmed(ECommandGame, ETrue);
        }
    }
    
    if (aResourceId == R_MENU_HELP) {
        if (!iGameContainer->IsVisible() || !iGameContainer->HaveGame()) {
            aMenuPane->SetItemDimmed(EHelpGame, ETrue);
        }
    }
}

void CPuzzlesAppView::PushLayoutL(ELayout aLayout) {
    // check recursive layout
    for (int i = 0; i <= iCurrentLayout; ++i) {
        if (aLayout == iLayoutStack[i]) {
            Panic(EPuzzlesPanicInvalidLayout);
        }
    }
    
    LeaveLayoutL();
    EnterLayoutL(aLayout, ETrue);
    
    iLayoutStack[++iCurrentLayout] = aLayout;
}

void CPuzzlesAppView::EnterLayoutL(ELayout aLayout, TBool aCreate) {
    int currentLayout = GetCurrentLayout();
    
    switch (aLayout) {
        case ELayoutHelp:
            if (aCreate) {
                ConstructHelpBrowserL();
            } else {
                iHelpBr->MakeVisible(ETrue);
            }
            iAppUi->Cba()->SetCommandSetL(R_HELP_SOFTKEYS_BACK_CLOSE);
            break;
        case ELayoutGame:
            iGameContainer->MakeVisible(ETrue);
            iAppUi->Cba()->SetCommandSetL(R_AVKON_SOFTKEYS_OPTIONS_EXIT);
            break;
        case ELayoutGameList:
            if (iGameContainer && iGameContainer->GetGame() != NULL) {
                const game *currentGame = iGameContainer->GetGame();
                for (TInt i = 0; i < gamecount; ++i) {
                    if (currentGame == gamelist[i]) {
                        iGameList->SetCurrentItemIndex(i);
                    }
                }
            }
            
            iGameList->MakeVisible(ETrue);
            iGameList->ScrollBarFrame()->SetScrollBarVisibilityL(
                CEikScrollBarFrame::EOff, 
                CEikScrollBarFrame::EAuto
            );
            iGameList->UpdateScrollBarsL();
            if (iGameContainer->HaveGame()) {
                iAppUi->Cba()->SetCommandSetL(R_AVKON_SOFTKEYS_OPTIONS_BACK);
            } else {
                iAppUi->Cba()->SetCommandSetL(R_AVKON_SOFTKEYS_OPTIONS_EXIT);
            }
            break;
        case ELayoutGameParameters:
            if (aCreate) {
                ConstructGameParametersL();
            } else {
                iGameSettings->MakeVisible(ETrue);
            }
            iAppUi->Cba()->SetCommandSetL(R_AVKON_SOFTKEYS_OK_CANCEL);
            break;
    }
    
    if ((aLayout == ELayoutGame || aLayout == ELayoutGameParameters)
    && iGameContainer && iGameContainer->HaveGame()) {
        iGameContainer->UpdateGameTitle();
    } else {
        iAppUi->ChangePaneTextL(KNullDesC, KNullDesC);
    }
    
    iAppUi->Cba()->DrawDeferred();
    iAppUi->HandleStatusPaneSizeChange();
}

void CPuzzlesAppView::LeaveLayoutL() {
    switch (GetCurrentLayout()) {
        case ELayoutHelp:
            iHelpBr->MakeVisible(EFalse);
            break;
            
        case ELayoutGame:
            iGameContainer->MakeVisible(EFalse);
            break;
            
        case ELayoutGameList:
            iGameList->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOff);
            iGameList->MakeVisible(EFalse);
            break;
        
        case ELayoutGameParameters:
            iGameSettings->MakeVisible(EFalse);
            break;
    }
}

void CPuzzlesAppView::PopLayoutL() {
    LeaveLayoutL();
    
    switch (GetCurrentLayout()) {
        case ELayoutHelp:
            FreeHelpBrowser();
            break;
        
        case ELayoutGameParameters:
            FreeGameParameters();
            break;
    }
    
    --iCurrentLayout;
    
    EnterLayoutL(GetCurrentLayout(), EFalse);
}

void CPuzzlesAppView::FreeGameParameters() {
    if (iGameSettings) {
        Components().Remove(iGameSettings);
        delete iGameSettings;
        iGameSettings = NULL;
    }
    
    if (iConfigValues) {
        int i;
        for (i = 0; i < iConfigLen; ++i) {
            if (iConfig[i].type == C_STRING) {
                iConfigValues[i].v.iStr->Close();
                delete iConfigValues[i].v.iStr;
            }
        }
        delete [] iConfigValues;
        iConfigValues = NULL;
    }
    
    if (iConfig) {
        free_cfg(iConfig);
        iConfig = NULL;
    }
}

void CPuzzlesAppView::ConstructGameParametersL() {
    iConfig = iGameContainer->GetGameConfig();
    iConfigLen = 0;
    
    int i;
    for (i = 0; iConfig[i].type != C_END; ++i) {
        ++iConfigLen;
    }
    
    iConfigValues = new (ELeave) TConfigValue[iConfigLen];
    
    for (i = 0; i < iConfigLen; ++i) {
        if (iConfig[i].type == C_STRING) {
            iConfigValues[i].v.iStr = NULL;
        }
    }
    
    iGameSettings = new (ELeave) CAknSettingItemList();
    CleanupStack::PushL(iGameSettings);
    iGameSettings->SetContainerWindowL(*this);
    iGameSettings->ConstructFromResourceL(R_ENTRY_SETTINGS_LIST);
    CleanupStack::Pop(iGameSettings);
    
    Components().AppendLC(iGameSettings);
    CleanupStack::Pop(iGameSettings);

    TBool isNumberedStyle = iGameSettings->IsNumberedStyle();
    CArrayPtr<CGulIcon>* icons = iGameSettings->ListBox()->ItemDrawer()->FormattedCellData()->IconArray();;
    
    
    _LIT(KEmpty, "Empty");
    TBuf<100> name;
    TPtrC8 name8;
    TPtrC8 sval8;
    RBuf *value;
    for (i = 0; i < iConfigLen; ++i) {
        name8.Set((TUint8*)iConfig[i].name, strlen(iConfig[i].name));
#ifndef NDEBUG
        if (name8.Length() > name.MaxLength()) {
            RDebug::Printf("%d: bad length!! %d > %d", i, name8.Length(), name.MaxLength());
        }
#endif
        name.Copy(name8);
        
        if (iConfig[i].type == C_STRING) {
            value = new (ELeave) RBuf;
            iConfigValues[i].v.iStr = value;
            value->Create(KMaxTextLength);
            
            sval8.Set((TUint8*)iConfig[i].sval, strlen(iConfig[i].sval));
#ifndef NDEBUG
            if (sval8.Length() > value->MaxLength()) {
                RDebug::Printf("%d: bad length!! %d > %d", i, sval8.Length(), value->MaxLength());
            }
#endif
            
            value->Copy(sval8);
            
            /*Text setting item*/
            CAknTextSettingItem* item = new (ELeave) CAknTextSettingItem(i, *value);
            CleanupStack::PushL(item);
            
            // The same resource id can be used for multiple text setting pages.
            item->ConstructL(isNumberedStyle, i, name, icons, R_TEXT_SETTING_PAGE, -1);
            item->SetEmptyItemTextL(KEmpty);
            iGameSettings->SettingItemArray()->AppendL(item);
            CleanupStack::Pop(item);
        }
        else if (iConfig[i].type == C_CHOICES) {
            iConfigValues[i].v.iInt = 0;
            
            /*Enumerated text setting item*/
            CAknEnumeratedTextPopupSettingItem* item = 
                new (ELeave) CAknEnumeratedTextPopupSettingItem(i, iConfigValues[i].v.iInt);
            CleanupStack::PushL(item);

            // The same resource id can be used for multiple enumerated text setting pages.
            item->ConstructL(isNumberedStyle, i, name, icons, R_ENUMERATEDTEXT_SETTING_PAGE, -1, 0, R_POPUP_SETTING_TEXTS);
            
            // Load texts dynamically.
            CArrayPtr<CAknEnumeratedText>* texts = item->EnumeratedTextArray();
            texts->ResetAndDestroy();
            CAknEnumeratedText* enumItem;
            TBuf<40> option;
            
            char *sval = iConfig[i].sval;
            char sep = sval[0];
            int j, k, n;
            for (n = 0, j = 1; sval[j-1]; ++n, ++j) {
                for (k = j; sval[k] != NULL && sval[k] != sep; ++k) ;
                
                TPtrC8 option8((TUint8*)&sval[j], k - j);
                
#ifndef NDEBUG
                if (option8.Length() > option.MaxLength()) {
                    RDebug::Printf("%d: bad length!! %d > %d", i, option8.Length(), option.MaxLength());
                }
#endif
                
                option.Copy(option8);
                
                HBufC *enumText = option.AllocLC();
                enumItem = new (ELeave) CAknEnumeratedText(n, enumText);
                CleanupStack::Pop(enumText);
                CleanupStack::PushL(enumItem);
                texts->AppendL(enumItem);
                CleanupStack::Pop(enumItem);
                j = k;
            }
            
            item->SetEmptyItemTextL(KEmpty);
            
            if (iConfig[i].ival >= 0 && iConfig[i].ival < item->NumberOfItems()) {
                iConfigValues[i].v.iInt = iConfig[i].ival;
                item->LoadL();
            }
            
            iGameSettings->SettingItemArray()->AppendL(item);
            CleanupStack::Pop(item);
        }
        else if (iConfig[i].type == C_BOOLEAN) {
            iConfigValues[i].v.iInt = iConfig[i].ival ? 1 : 0;
            
            /*Binary popup setting item*/
            CAknBinaryPopupSettingItem* item = new (ELeave) CAknBinaryPopupSettingItem(
                i, iConfigValues[i].v.iInt
            );
            CleanupStack::PushL(item);
            
            // The same resource id can be used for multiple binary setting pages.
            item->ConstructL(isNumberedStyle, i, name, icons, R_BINARY_SETTING_PAGE, -1, 0, R_BINARY_TEXTS);

            iGameSettings->SettingItemArray()->AppendL(item);
            CleanupStack::Pop(item);
        }
        else {
#ifndef NDEBUG
            RDebug::Printf("%d: invalid option type!! %d", i, iConfig[i].type);
#endif
        }
    }

    // Required when there is only one setting item.
    iGameSettings->SettingItemArray()->RecalculateVisibleIndicesL();
    iGameSettings->HandleChangeInItemArrayOrVisibilityL();     
    iGameSettings->ListBox()->UpdateScrollBarsL();

    iGameSettings->SetRect(Rect());
    iGameSettings->ListBox()->SetRect(Rect());
    iGameSettings->ActivateL();
    
    iGameSettings->SetObserver(this);
}

void CPuzzlesAppView::SelectGame() {
    iGameContainer->SaveGame();
    TInt sel = iGameList->CurrentItemIndex();
    const game *thegame = gamelist[sel];
    iGameContainer->SetGame(thegame);
    iGameContainer->LoadOrNewGame();
    PopLayoutL();
    iAppUi->AppConfig().SetLastSelectedGame(thegame->name);
}

TBool CPuzzlesAppView::ShowHelp(const char *topic) {
    _LIT(KPrefix, "file://");
    _LIT(KExt, ".html");
    
    TInt i;
    const char *t;
    TPath helpFile;
    
    PushLayoutL(ELayoutHelp);
    
    helpFile.Zero();
    helpFile.Append(KResources);
    
    for (t = topic; *t; ++t) {
        TChar c = *t;
        if (c >= 'a' && c <= 'z') {
            helpFile.Append(c);
        }
        if (c >= 'A' && c <= 'Z') {
            helpFile.Append(c + (TChar)('a' - 'A'));
        }
    }
    
    // helpFile.Append(btopic);
    helpFile.Append(KExt);
    
    CompleteWithAppPath(helpFile);
    
    // convert file paths
    for (i = 0; i < helpFile.Length(); ++i) {
        if (helpFile[i] == '\\') {
            helpFile[i] = '/';
        }
    }
    
    helpFile.Insert(0, KPrefix);
    
    iHelpBr->LoadUrlL(helpFile);
    
    return ETrue;
}


TBool CPuzzlesAppView::HandleCommandL(TInt aCommand) {
    switch (aCommand) {
        case EHelpIntroduction:     return ShowHelp("intro");
        case EHelpCommonFeatures:   return ShowHelp("common");
        case EHelpOverview:         return ShowHelp("index");
        
        case EHelpGame: {
            if (iGameContainer && iGameContainer->HaveGame()) {
                return ShowHelp(iGameContainer->GetGame()->name);
            }
            return ETrue;
        }
    }
    
    switch (aCommand) {
        case ECommandSelectGame:
            switch (GetCurrentLayout()) {
                case ELayoutGameList:
                    SelectGame();
                    return ETrue;
                    
                case ELayoutGame:
                    PushLayoutL(ELayoutGameList);
                    return ETrue;
            }
            break;
    
        case EAknSoftkeyClose:
            switch (GetCurrentLayout()) {
                case ELayoutHelp:
                    PopLayoutL();
                    return ETrue;
            }
            break;
            
        case EAknSoftkeyBack:
            switch (GetCurrentLayout()) {
                case ELayoutHelp:
                    if (iHelpBr->NavigationAvailable(TBrCtlDefs::ENavigationBack)) {
                        iHelpBr->HandleCommandL(TBrCtlDefs::ECommandIdBase + TBrCtlDefs::ECommandBack);
                    }
                    return ETrue;
                    
                case ELayoutGameList:
                    PopLayoutL();
                    return ETrue;
            }
            break;
            
        case EAknSoftkeyCancel:
            switch (GetCurrentLayout()) {
                case ELayoutGameParameters:
                    PopLayoutL();
                    return ETrue;
            }
            break;
            
        case EAknSoftkeyOk:
            switch (GetCurrentLayout()) {
                case ELayoutGameParameters:
                    iGameSettings->StoreSettingsL();
                
                    TBuf8<50> str8;
                    for (int i = 0; i < iConfigLen; ++i) {
                        switch (iConfig[i].type) {
                            case C_STRING:
                                sfree(iConfig[i].sval);
                                str8.Copy(*iConfigValues[i].v.iStr);
                                iConfig[i].sval = dupstr((char*)str8.PtrZ());
                                break;
                                
                            case C_BOOLEAN:
                            case C_CHOICES:
                                iConfig[i].ival = iConfigValues[i].v.iInt;
                                break;
                        }
                    }
                    
                    char *error = iGameContainer->SetGameConfig(iConfig);
                    if (error != NULL) {
                        TBuf<100> bError;
                        bError.Copy(TPtrC8((TUint8*)error));
                        
                        CAknErrorNote *dlg = new (ELeave) CAknErrorNote(ETrue);
                        dlg->ExecuteLD(bError);
                    } else {
                        PopLayoutL();
                    }
                    return ETrue;
            }
            break;
    }
    
    return EFalse;
}

void CPuzzlesAppView::HandleControlEventL(CCoeControl* aControl, TCoeEvent aEventType) {
    if (aControl == iGameSettings && aEventType == EEventStateChanged) {
        iGameSettings->DrawDeferred();
    }
}

void CPuzzlesAppView::HandleListBoxEventL(CEikListBox* /*aListBox*/, TListBoxEvent aEventType) {
    if (aEventType == EEventEnterKeyPressed || aEventType == EEventItemDoubleClicked) {
        SelectGame();
    }
}

TKeyResponse CPuzzlesAppView::OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType) {
    if (iGameContainer->IsVisible()) {
        return iGameContainer->OfferKeyEventL(aKeyEvent, aType);
    }
    
    if (iGameList->IsVisible()) {
        return iGameList->OfferKeyEventL(aKeyEvent, aType);
    }
    
    if (iHelpBr) {
        return iHelpBr->OfferKeyEventL(aKeyEvent, aType);
    }
    
    if (iGameSettings && iGameSettings->IsVisible()) {
        return iGameSettings->OfferKeyEventL(aKeyEvent, aType);
    }
    
    return EKeyWasNotConsumed;
}

// End of File
