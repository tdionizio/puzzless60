
#ifndef __PUZZLESDRAWINGAPI_h__
#define __PUZZLESDRAWINGAPI_h__

// forward declarations
typedef struct drawing_api drawing_api;
typedef struct blitter blitter;


class CPuzzlesDrawingApi {
public:
    static const drawing_api *Api();

private:
    CPuzzlesDrawingApi() {}
    
    static void DrawAPI_DrawText(void *handle, int x, int y, int fonttype,
        int fontsize, int align, int colour, char *text);
    static void DrawAPI_DrawRect(void *handle, int x, int y, int w, int h, int colour);
    static void DrawAPI_DrawLine(void *handle, int x1, int y1, int x2, int y2,
        int colour);
    static void DrawAPI_DrawPolygon(void *handle, int *coords, int npoints,
        int fillcolour, int outlinecolour);
    static void DrawAPI_DrawCircle(void *handle, int cx, int cy, int radius,
        int fillcolour, int outlinecolour);
    static void DrawAPI_Clip(void *handle, int x, int y, int w, int h);
    static void DrawAPI_Unclip(void *handle);
    static void DrawAPI_StartDraw(void *handle);
    static void DrawAPI_DrawUpdate(void *handle, int x, int y, int w, int h);
    static void DrawAPI_EndDraw(void *handle);
    static blitter *DrawAPI_BlitterNew(void *handle, int w, int h);
    static void DrawAPI_BlitterFree(void *handle, blitter *bl);
    static void DrawAPI_BlitterSave(void *handle, blitter *bl, int x, int y);
    static void DrawAPI_BlitterLoad(void *handle, blitter *bl, int x, int y);
    static void DrawAPI_StatusBar(void *handle, char *text);
};

#endif // __PUZZLESDRAWINGAPI_h__
