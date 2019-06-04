#include "bigpixelcanvas.h"
#include <wx/dcclient.h>

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

BEGIN_EVENT_TABLE(BigPixelCanvas, wxPanel)
    EVT_PAINT    (BigPixelCanvas::OnPaint)
    EVT_LEFT_UP  (BigPixelCanvas::OnClick)
END_EVENT_TABLE()


inline wxColour operator-(const wxColour& c1, const wxColour& c2) {
    unsigned char red = c1.Red() - c2.Red();
    unsigned char green = c1.Green() - c2.Green();
    unsigned char blue = c1.Blue() - c2.Blue();
    return wxColour(red, green, blue);
}

inline wxColour operator*(const wxColour& c, float n) {
    unsigned char red = c.Red() * n;
    unsigned char green = c.Green() * n;
    unsigned char blue = c.Blue() * n;
    return wxColour(red, green, blue);
}

inline wxColour operator+(const wxColour& c1, const wxColour& c2) {
    unsigned char red = c1.Red() + c2.Red();
    unsigned char green = c1.Green() + c2.Green();
    unsigned char blue = c1.Blue() + c2.Blue();
    return wxColour(red, green, blue);
}

BigPixelCanvas::BigPixelCanvas(wxFrame *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
      mPixelSize(5),
      mUsandoComandos(false),
      mBackgroundMode(wxSOLID),
      mColourForeground(*wxGREEN),
      mColourBackground(*wxWHITE),
      mPen(*wxBLACK_PEN)
{
    mOwnerPtr = parent;
    m_clip = false;
}

void BigPixelCanvas::DrawPixel(int x, int y, wxDC& dc)
{
    x *= mPixelSize;
    y *= mPixelSize;

    int halfPixelSize = mPixelSize / 2;
    int xStart = x - halfPixelSize;
    int xEnd = x + halfPixelSize;
    int yStart = y - halfPixelSize;
    int yEnd = y + halfPixelSize;
    for (int x = xStart; x <= xEnd; ++x)
        for (int y = yStart; y <= yEnd; ++y)
            dc.DrawPoint(x,y);
}

void BigPixelCanvas::DrawPixel(int x, int y, double z, wxDC& dc)
{
    if (mZBuffer.IsVisible(y, x, z)) {
        x *= mPixelSize;
        y *= mPixelSize;
        int halfPixelSize = mPixelSize / 2;
        int xStart = x - halfPixelSize;
        int xEnd = x + halfPixelSize;
        int yStart = y - halfPixelSize;
        int yEnd = y + halfPixelSize;
        for (int x = xStart; x <= xEnd; ++x)
            for (int y = yStart; y <= yEnd; ++y)
                dc.DrawPoint(x,y);
    }
}

void BigPixelCanvas::DrawLine(wxPoint p0, wxPoint p1)
{
    wxClientDC dc(this);
    PrepareDC(dc);
    p0 = ConvertDeviceToLogical(p0);
    p1 = ConvertDeviceToLogical(p1);
    DrawLine(p0, p1, dc);
}

void BigPixelCanvas::DrawLine(const wxPoint& p0, const wxPoint& p1, wxDC& dc)
{
    // Aqui o codigo para desenhar uma linha.
    // Para desenhar um pixel, use "DrawPixel(x, y, dc)".
    #warning BigPixelCanvas::DrawLine não foi implementado (necessário para a rasterização de segmentos de reta).
}

void BigPixelCanvas::DrawCircle(wxPoint center, int radius)
{
    wxClientDC dc(this);
    PrepareDC(dc);
    center = ConvertDeviceToLogical(center);
    DrawCircle(center, radius/mPixelSize, dc);
}

void BigPixelCanvas::DrawCircle(const wxPoint& center, int radius, wxDC& dc)
{
    // Aqui o codigo para desenhar um circulo.
    // Para desenhar um pixel, use "DrawPixel(x, y, dc)".
    #warning BigPixelCanvas::DrawCircle não foi implementado (necessário para rasterização de círculos).
}

void BigPixelCanvas::DesenharTriangulo2D(const Triang2D& triangulo) {
    wxClientDC dc(this);
    PrepareDC(dc);
    DesenharTriangulo2D(triangulo, dc);
}

void BigPixelCanvas::DesenharTriangulo2D(const Triang2D& triangulo, wxDC& dc) {
    Interv2D intervalo;
    while (triangulo.AtualizarIntervaloHorizontal(&intervalo))
        if (intervalo.Valido())
            DesenharIntervaloHorizontal(intervalo, dc);
}

void BigPixelCanvas::DesenharTriangulo3D(const Triang3D& triangulo, wxDC& dc)
{
    Interv3D intervalo;
    while (triangulo.AtualizarIntervaloHorizontal(&intervalo))
        if (intervalo.Valido())
            DesenharIntervaloHorizontal(intervalo, dc);
}

void BigPixelCanvas::DesenharIntervaloHorizontal(const Interv2D& intervalo, wxDC& dc)
{
    int x = intervalo.mXMin;
    while (x < intervalo.mXMax) {
        DrawPixel(x, intervalo.mY, dc);
        ++x;
    }
}

void BigPixelCanvas::DesenharIntervaloHorizontal(const Interv3D& intervalo, wxDC& dc)
{
    // Colocar aqui o código para desenhar um intervalo horizontal 3D. Necessário
    // para a implementação do z-buffer.
    // Desenhar um intervalo 3D é como desenhar um intervalo 2D, usando z-buffer.
    #warning BigPixelCanvas::DesenharIntervaloHorizontal não foi implementado (necessário para a rasterização do z-buffer).
}

void BigPixelCanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC pdc(this);
    wxDC &dc = pdc;

    PrepareDC(dc);

    mOwnerPtr->PrepareDC(dc);
    dc.SetBackgroundMode( mBackgroundMode );
    if ( mBackgroundBrush.Ok() )
        dc.SetBackground( mBackgroundBrush );
    if ( mColourForeground.Ok() )
        dc.SetTextForeground( mColourForeground );
    if ( mColourBackground.Ok() )
        dc.SetTextBackground( mColourBackground );

    dc.Clear();
    if (mUsandoComandos)
        InterpretarComandos();
}

void BigPixelCanvas::InterpretarComandos()
{
    ifstream arquivo("comandos.txt");
    wxClientDC dc(this);
    PrepareDC(dc);
    string comando;
    while (arquivo >> comando)
    {
        if (comando == "linha")
        {
            int p0x, p0y, p1x, p1y;
            arquivo >> p0x >> p0y >> p1x >> p1y;
            DrawLine(wxPoint(p0x, p0y), wxPoint(p1x, p1y), dc);
        }
        else if (comando == "cor")
        {
            int r, g, b;
            arquivo >> r >> g >> b;
            mPen.SetColour(r, g, b);
            dc.SetPen(mPen);
        }
        else if (comando == "triangulo3d")
        {
            int x, y, z;
            arquivo >> x >> y >> z;
            P3D p1(x,y,z);
            arquivo >> x >> y >> z;
            P3D p2(x,y,z);
            arquivo >> x >> y >> z;
            P3D p3(x,y,z);
            Triang3D tri(p1, p2, p3);
            DesenharTriangulo3D(tri, dc);
        }
    }
}

void BigPixelCanvas::OnClick(wxMouseEvent &event)
{
    wxPostEvent(mOwnerPtr, event);
}

void BigPixelCanvas::PrepareDC(wxDC& dc)
{
    int height, width;
    GetClientSize(&width, &height);
    dc.SetLogicalOrigin(-width/2, height/2);
    dc.SetAxisOrientation(true, true);
    dc.SetMapMode(wxMM_TEXT);
    dc.SetPen(mPen);
    mZBuffer.AlterarCapacidade(static_cast<unsigned int>(height/mPixelSize),
                               static_cast<unsigned int>(width/mPixelSize));
}

wxPoint BigPixelCanvas::ConvertDeviceToLogical(const wxPoint& p)
{
    wxClientDC dc(this);
    PrepareDC(dc);
    wxPoint result;
    result.x = dc.DeviceToLogicalX(p.x) / mPixelSize;
    result.y = dc.DeviceToLogicalY(p.y) / mPixelSize;
    return result;
}

