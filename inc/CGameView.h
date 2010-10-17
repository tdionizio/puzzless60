
#ifndef __PUZZLESCGAMEVIEW_h__
#define __PUZZLESCGAMEVIEW_h__

// INCLUDES
#include "PuzzlesConfig.h"
#include <coecntrl.h>

#include "PuzzlesAppUi.h"

class CGameContainer;

// CLASS DECLARATION
class CGameView: public CCoeControl {
    friend class CPuzzlesDrawingApi;
    
public:
    // New methods

    static CGameView* NewL(CGameContainer *aGameContainer);
    static CGameView* NewLC(CGameContainer *aGameContainer);
    
private: // Functions from base classes
    void Draw(const TRect& aRect) const;
    
public: // Functions from base classes
#ifdef ENABLE_TOUCH_UI
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
#endif
    
private:
    // Constructors

    void ConstructL(CGameContainer *aGameContainer);
    CGameView();
    virtual ~CGameView();

private:
    CGameContainer *iGameContainer;
    
    TPoint iGamePos;
    TSize iGameSize;
    CFbsBitmap *iVirtualScreen;
    
    CBitmapContext* iBC;
    CFbsBitmapDevice* iBitmapDevice;
    int iLastFontType;
    int iLastFontSize;
    CFbsFont *iLastFont;
    TInt iFontID;
    TInt iFixedFontID;
    
    void SetupFont(int fonttype, int fontsize);
    void ReleaseFont();
    
public:
    bool SetGameSize(const TSize &aSize);
    
private:
    void DrawText(int x, int y, int fonttype, int fontsize, int align,
        int colour, char *text);
    void DrawRect(int x, int y, int w, int h, int colour);
    void DrawLine(int x1, int y1, int x2, int y2, int colour);
    void DrawThickLine(float thickness, float x1, float y1, float x2, float y2, int colour);
    void DrawPolygon(int *coords, int npoints, int fillcolour,
        int outlinecolour);
    void DrawCircle(int cx, int cy, int radius, int fillcolour,
        int outlinecolour);
    void Clip(int x, int y, int w, int h);
    void Unclip();
    void StartDraw();
    void DrawUpdate(int x, int y, int w, int h);
    void EndDraw();
    blitter *BlitterNew(int w, int h);
    void BlitterFree(blitter *bl);
    void BlitterSave(blitter *bl, int x, int y);
    void BlitterLoad(blitter *bl, int x, int y);
    void StatusBar(char *text);
    
};

#endif // __PUZZLESCGAMEVIEW_h__
// End of File
