
// INCLUDE FILES
#include "PuzzlesConfig.h"

#include <aknutils.h>
#include <coemain.h>
#include "CGameView.h"
#include "CGameContainer.h"
#include "PuzzlesAppUi.h"

#include <string.h>

extern "C" {
#include "puzzles.h"
}

#include "_puzzles.inc.h"

// ============================ MEMBER FUNCTIONS ===============================

struct blitter {
    CFbsBitmap *bitmap;
    CBitmapDevice *bd;
    CBitmapContext *gc;
};

// -----------------------------------------------------------------------------
// CGameView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGameView* CGameView::NewL(CGameContainer *aGameContainer) {
    CGameView* self = CGameView::NewLC(aGameContainer);
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
// CGameView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGameView* CGameView::NewLC(CGameContainer *aGameContainer) {
    CGameView* self = new (ELeave) CGameView;
    CleanupStack::PushL(self);
    self->ConstructL(aGameContainer);
    return self;
}

// -----------------------------------------------------------------------------
// CGameView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CGameView::ConstructL(CGameContainer *aGameContainer) {
    iGameContainer = aGameContainer;
    iLastFont = NULL;
    
    // Create a window for this application view
    SetContainerWindowL(*aGameContainer);
    
    EnableDragEvents();
    
    iGameSize.iWidth = 0;
    iGameSize.iHeight = 0;
    
    iVirtualScreen = new (ELeave) CFbsBitmap;
    User::LeaveIfError(iVirtualScreen->Create(TSize(0, 0), iCoeEnv->ScreenDevice()->DisplayMode()));
    iBitmapDevice = CFbsBitmapDevice::NewL(iVirtualScreen);
    User::LeaveIfError(iBitmapDevice->CreateBitmapContext(iBC));
    
    TFileName fFont;
    TFileName fFixedFont;
    
    fFont.Copy(KFont);
    fFixedFont.Copy(KFixedFont);
    
    CompleteWithAppPath(fFont);
    CompleteWithAppPath(fFixedFont);
    
    // load fonts
    User::LeaveIfError(iBitmapDevice->AddFile(fFont, iFontID));
    User::LeaveIfError(iBitmapDevice->AddFile(fFixedFont, iFixedFontID));
}

// -----------------------------------------------------------------------------
// CGameView::CGameView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CGameView::CGameView() {
    // No implementation required
}

// -----------------------------------------------------------------------------
// CGameView::~CGameView()
// Destructor.
// -----------------------------------------------------------------------------
//
CGameView::~CGameView() {
    if (iVirtualScreen) {
        ReleaseFont();
        
        if (iBitmapDevice) {
            iBitmapDevice->RemoveFile(iFixedFontID);
            iBitmapDevice->RemoveFile(iFontID);
        }
        
        delete iBC;
        delete iBitmapDevice;
        delete iVirtualScreen;
    }
}

bool CGameView::SetGameSize(const TSize& aSize) {
    TSize size = Rect().Size();
    iGamePos.iX = (size.iWidth - aSize.iWidth) >> 1;
    iGamePos.iY = (size.iHeight - aSize.iHeight) >> 1;
    
    if (iGameSize != aSize) {
        ReleaseFont();
        delete iBC;
        
        iGameSize = aSize;
        
        User::LeaveIfError(iVirtualScreen->Resize(aSize));
        User::LeaveIfError(iBitmapDevice->Resize(aSize));
        User::LeaveIfError(iBitmapDevice->CreateBitmapContext(iBC));
        
        iBC->SetBrushStyle(CGraphicsContext::ESolidBrush);
        iBC->SetBrushColor(KRgbWhite);
        iBC->Clear();
        
        return true;
    }
    
    return false;
}


// -----------------------------------------------------------------------------
// CGameView::HandlePointerEventL()
// Called by framework to handle pointer touch events.
// Note: although this method is compatible with earlier SDKs, 
// it will not be called in SDKs without Touch support.
// -----------------------------------------------------------------------------
//
#ifdef ENABLE_TOUCH_UI
void CGameView::HandlePointerEventL(const TPointerEvent& aPointerEvent) {
    iGameContainer->HandleGamePointerEventL(aPointerEvent, iGamePos);
}
#endif

void CGameView::SetupFont(int fonttype, int fontsize) {
    if (iLastFont && (fonttype != iLastFontType || fontsize != iLastFontSize)) {
        ReleaseFont();
    }
    
    if (!iLastFont) {
        _LIT(KVera, "Bitstream Vera Sans");
        _LIT(KVeraFixed, "Bitstream Vera Sans Mono");
        
        const TDesC *name;
        
        if (fonttype == FONT_FIXED) {
            name = &KVeraFixed;
        } else {
            name = &KVera;
        }
        
        TFontSpec fontSpec(*name, fontsize);
        fontSpec.iFontStyle.SetBitmapType(EAntiAliasedGlyphBitmap);
        iBitmapDevice->GetNearestFontInPixels(iLastFont, fontSpec);

        iBC->UseFont(iLastFont);

        iLastFontType = fonttype;
        iLastFontSize = fontsize;
    }
}
void CGameView::ReleaseFont() {
    if (iLastFont) {
        iBC->DiscardFont();
        iBitmapDevice->ReleaseFont(iLastFont);
        iLastFont = NULL;
    }
}

#define GOOD_XY(x, y) \
    ((x) >= 0 && (x) < iGameSize.iWidth && (y) >= 0 && (y) < iGameSize.iHeight)
#define CHECK_XY(x, y) \
    if (!GOOD_XY(x, y)) return;
#define CHECK_XYWH(x, y, w, h) \
    if (!GOOD_XY(x, y) || !GOOD_XY(x + w - 1, y + h - 1)) return;
#define CHECK2_XY(x1, y1, x2, y2) \
    if (!GOOD_XY(x1, y1) || !GOOD_XY(x2, y2)) return;

void CGameView::DrawText(
    int x, int y, 
    int fonttype, int fontsize,
    int align, int colour, 
    char *text
) {
    CHECK_XY(x, y);
    
    SetupFont(fonttype, fontsize);
    
    TBuf<32> buf;
    HBufC *hText = NULL;
    TPtrC8 pText((TUint8*)text);
    int w;
    
	if (pText.Length() > buf.MaxLength()) {
	    hText = HBufC::NewL(pText.Length() + 1);
	    hText->Des().Copy(pText);
	    
        w = iLastFont->TextWidthInPixels(hText->Des());
	} else {
	    buf.Copy(pText);
        w = iLastFont->TextWidthInPixels(buf);
	}
    
    if (align & ALIGN_VCENTRE) {
        y = y + iLastFont->AscentInPixels() / 2 + 1;
    }
    if (align & ALIGN_HCENTRE) {
        x = x - w / 2 + 1;
    }
    if (align & ALIGN_HRIGHT) {
        x = x - w;
    }
    
    iBC->SetPenColor(iGameContainer->MapColour(colour));
    
    if (hText != NULL) {
        iBC->DrawText(hText->Des(), TPoint(x, y));
        delete hText;
    } else {
        iBC->DrawText(buf, TPoint(x, y));
    }
}
void CGameView::DrawRect(int x, int y, int w, int h, int colour) {
    CHECK_XYWH(x, y, w, h);
    
    iBC->SetPenStyle(CGraphicsContext::ENullPen);
    iBC->SetBrushStyle(CGraphicsContext::ESolidBrush);
    iBC->SetBrushColor(iGameContainer->MapColour(colour));
    iBC->DrawRect(TRect(TPoint(x, y), TSize(w, h)));
}
void CGameView::DrawLine(int x1, int y1, int x2, int y2, int colour) {
    CHECK2_XY(x1, y1, x2, y2);
    
    iBC->SetPenStyle(CGraphicsContext::ESolidPen);
    iBC->SetPenColor(iGameContainer->MapColour(colour));
    iBC->DrawLine(TPoint(x1, y1), TPoint(x2, y2));
}
void CGameView::DrawThickLine(float thickness, float x1, float y1, float x2, float y2, int colour) {
    CHECK2_XY(x1, y1, x2, y2);
    
    int thick = (int)((thickness - 1) / 2 + 0.5) * 2 + 1;
    
    iBC->SetPenSize(TSize(thick, thick));
    iBC->SetPenStyle(CGraphicsContext::ESolidPen);
    iBC->SetPenColor(iGameContainer->MapColour(colour));
    iBC->DrawLine(TPoint(x1, y1), TPoint(x2, y2));
    
    iBC->SetPenSize(TSize(1, 1));
}
void CGameView::DrawPolygon(
    int *coords, int npoints, 
    int fillcolour, int outlinecolour
) {
    int i;
    for (i = 0; i < npoints; ++i) {
        if (!GOOD_XY(coords[i*2], coords[i*2+1])) {
            return;
        }
    }
    
    TPoint *points = new TPoint[npoints];
    
    for (i = 0; i < npoints; ++i) {
        points[i] = TPoint(coords[i*2], coords[i*2+1]);
    }

    iBC->SetPenStyle(CGraphicsContext::ESolidPen);
    iBC->SetPenColor(iGameContainer->MapColour(outlinecolour));
    iBC->SetBrushStyle(CGraphicsContext::ESolidBrush);
    iBC->SetBrushColor(iGameContainer->MapColour(fillcolour));
    iBC->DrawPolygon(points, (TInt)npoints, CGraphicsContext::EAlternate);
    
    delete [] points;
}
void CGameView::DrawCircle(
    int cx, int cy, int radius, 
    int fillcolour,
    int outlinecolour
) {
    CHECK2_XY(cx, cy, cx+radius-1, cy+radius-1);
    
    iBC->SetPenStyle(CGraphicsContext::ESolidPen);
    iBC->SetPenColor(iGameContainer->MapColour(outlinecolour));
    iBC->SetBrushStyle(CGraphicsContext::ESolidBrush);
    iBC->SetBrushColor(iGameContainer->MapColour(fillcolour));
    iBC->DrawEllipse(TRect(cx - radius, cy - radius, cx + radius + 1, cy + radius + 1));
}
void CGameView::Clip(int x, int y, int w, int h) {
    iBC->SetClippingRect(TRect(TPoint(x, y), TSize(w, h)));
}
void CGameView::Unclip() {
    iBC->CancelClippingRect();
}
void CGameView::StartDraw() {
    iBC->SetPenSize(TSize(1, 1));
}
void CGameView::DrawUpdate(int /* x */, int /* y */, int /* w */, int /* h */) {
    //DrawNow(TRect(TPoint(x, y) + iGamePos, TSize(w, h)));
}
void CGameView::EndDraw() {
    ReleaseFont();
    Unclip();
    
    DrawDeferred();
}
blitter *CGameView::BlitterNew(int w, int h) {
    blitter *bl = snew(blitter);
    bl->bitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError(bl->bitmap->Create(TSize(w, h), iVirtualScreen->DisplayMode()));
    bl->bd = CFbsBitmapDevice::NewL(bl->bitmap);
    bl->bd->CreateBitmapContext(bl->gc);
    return bl;
}
void CGameView::BlitterFree(blitter *bl) {
    delete bl->gc;
    delete bl->bd;
    delete bl->bitmap;
    sfree(bl);
}
void CGameView::BlitterSave(blitter *bl, int x, int y) {
    bl->gc->BitBlt(
        TPoint(0, 0),
        iVirtualScreen,
        TRect(TPoint(x, y), bl->bitmap->SizeInPixels())
    );
}
void CGameView::BlitterLoad(blitter *bl, int x, int y) {
    iBC->BitBlt(TPoint(x, y), bl->bitmap);
}

// -----------------------------------------------------------------------------
// CGameView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CGameView::Draw(const TRect& /* aRect */) const {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();
    
    if (iVirtualScreen) {
        TRect gameRect(TPoint(), iGameSize);
        
        gc.BitBlt(
            iGamePos,
            iVirtualScreen,
            gameRect
        );
        
        // draw a rectangle around the board area to make 
        // the limits visible to the user
        
        gc.SetPenSize(TSize(1, 1));
        gc.SetPenColor(KRgbBlack);
        gc.SetPenStyle(CGraphicsContext::ESolidPen);
        gc.SetBrushStyle(CGraphicsContext::ENullBrush);
        
        gameRect.Move(iGamePos);
        gameRect.Grow(1, 1);
        gc.DrawRect(gameRect);
    }
}

// End of File
