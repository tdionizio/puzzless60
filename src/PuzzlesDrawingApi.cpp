
#include "PuzzlesDrawingApi.h"
#include "CGameView.h"
#include "CGameContainer.h"

extern "C" {
#include "puzzles.h"
}

void CPuzzlesDrawingApi::DrawAPI_DrawText(void *handle, int x, int y, int fonttype,
    int fontsize, int align, int colour, char *text) {
    ((CGameView*) handle)->DrawText(x, y, fonttype, fontsize, align, colour, text);
}
void CPuzzlesDrawingApi::DrawAPI_DrawRect(void *handle, int x, int y, int w, int h, int colour) {
    ((CGameView*) handle)->DrawRect(x, y, w, h, colour);
}
void CPuzzlesDrawingApi::DrawAPI_DrawLine(void *handle, int x1, int y1, int x2, int y2, int colour) {
    ((CGameView*) handle)->DrawLine(x1, y1, x2, y2, colour);
}
void CPuzzlesDrawingApi::DrawAPI_DrawPolygon(void *handle, int *coords, int npoints,
    int fillcolour, int outlinecolour) {
    ((CGameView*) handle)->DrawPolygon(
        coords, npoints, fillcolour, outlinecolour);
}
void CPuzzlesDrawingApi::DrawAPI_DrawCircle(void *handle, int cx, int cy, int radius,
    int fillcolour, int outlinecolour) {
    ((CGameView*) handle)->DrawCircle(cx, cy, radius, fillcolour, outlinecolour);
}
void CPuzzlesDrawingApi::DrawAPI_Clip(void *handle, int x, int y, int w, int h) {
    ((CGameView*) handle)->Clip(x, y, w, h);
}
void CPuzzlesDrawingApi::DrawAPI_Unclip(void *handle) {
    ((CGameView*)handle)->Unclip();
}
void CPuzzlesDrawingApi::DrawAPI_StartDraw(void *handle) {
    ((CGameView*) handle)->StartDraw();
}
void CPuzzlesDrawingApi::DrawAPI_DrawUpdate(void *handle, int x, int y, int w, int h) {
    ((CGameView*) handle)->DrawUpdate(x, y, w, h);
}
void CPuzzlesDrawingApi::DrawAPI_EndDraw(void *handle) {
    ((CGameView*) handle)->EndDraw();
}
blitter *CPuzzlesDrawingApi::DrawAPI_BlitterNew(void *handle, int w, int h) {
    return ((CGameView*) handle)->BlitterNew(w, h);
}
void CPuzzlesDrawingApi::DrawAPI_BlitterFree(void *handle, blitter *bl) {
    ((CGameView*) handle)->BlitterFree(bl);
}
void CPuzzlesDrawingApi::DrawAPI_BlitterSave(void *handle, blitter *bl, int x, int y) {
    ((CGameView*) handle)->BlitterSave(bl, x, y);
}
void CPuzzlesDrawingApi::DrawAPI_BlitterLoad(void *handle, blitter *bl, int x, int y) {
    ((CGameView*) handle)->BlitterLoad(bl, x, y);
}
void CPuzzlesDrawingApi::DrawAPI_StatusBar(void *handle, char *text) {
    TPtrC8 pText((TUint8*)text);
    TBuf<100> buf;
    buf.Copy(pText);
    ((CGameView*) handle)->iGameContainer->SetStatusText(buf);
}


const drawing_api *CPuzzlesDrawingApi::Api() {
    static const struct drawing_api drawing_api = {
        DrawAPI_DrawText,
        DrawAPI_DrawRect,
        DrawAPI_DrawLine,
        DrawAPI_DrawPolygon,
        DrawAPI_DrawCircle,
        DrawAPI_DrawUpdate,
        DrawAPI_Clip,
        DrawAPI_Unclip,
        DrawAPI_StartDraw,
        DrawAPI_EndDraw,
        DrawAPI_StatusBar,
        DrawAPI_BlitterNew,
        DrawAPI_BlitterFree,
        DrawAPI_BlitterSave,
        DrawAPI_BlitterLoad,
    };
    return &drawing_api;
}
