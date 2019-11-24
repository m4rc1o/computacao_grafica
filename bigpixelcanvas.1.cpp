#include "bigpixelcanvas.h"
#include <wx/dcclient.h>

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

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
    // Coordenadas do ponto inicial
    int x0 = p0.x;
    int y0 = p0.y;
    
    // Coordenadas do ponto final
    int x1 = p1.x;
    int y1 = p1.y;

    // Cálculo de dy e dx(As variações em y e x)
    int dy = y1 - y0;
    int dx = x1 - x0;

    int incrementoVert = 1;
    int incrementoHor = 1;
    
    if(dx < 0 && dy < 0){
        // Para permitir que linhas em quadrantes diametralmente opostos 
        // sejam calculadas da mesma forma
        int xTemp = x0;
        int yTemp = y0;
        x0 = x1;
        y0 = y1;
        x1 = xTemp;
        y1 = yTemp;
    }else if(dy < 0){
        incrementoVert = -1;
    }else if(dx < 0){
        incrementoHor = -1;
    }
    
    dx = abs(dx);
    dy = abs(dy);

    int x = x0;
    int y = y0;
    DrawPixel(x0, y0, dc);
    if(dy <= dx){
        int d = 2*dy - dx; 
        int deltaLeste = 2*dy;
        int deltaNordeste = 2*(dy - dx);
        while(x != x1){
            if(d > 0){
                d += deltaNordeste;
                y += incrementoVert;
            }else{
                d += deltaLeste;
            }
            x += incrementoHor;
            DrawPixel(x, y, dc);
        }
    }else{
        int d = dy - 2*dx;
        int deltaNordeste = 2*(dy - dx);
        int deltaNorte = -2*dx;
        while(y != y1){
            if(d <= 0){
                d += deltaLeste;
                x += incrementoHor;
            }else{
                d += deltaNordeste;
            }
            y += incrementoVert;
            DrawPixel(x, y, dc);
        }
    }
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
    int x = 0;
    int y = radius;
    int x1 = round(radius*cos(M_PI/4));
    int d = 1 - radius;
    DrawPixel(center.x, center.y + y, dc);
    DrawPixel(center.x, center.y - y, dc);
    DrawPixel(center.x - radius, center.y, dc);
    DrawPixel(center.x + radius, center.y, dc);
    int deltaLeste = 0;
    int deltaSudeste = 0;    
    while(x < x1){
        deltaLeste = (x<<1) + 3;
        deltaSudeste = ((x - y)<<1) + 5;
        if(d < 0){
            d += deltaLeste;
        }else{
            d += deltaSudeste;
            y--;
        }
        x++;
        DrawPixel(center.x + x, center.y + y, dc);
        DrawPixel(center.x + x, center.y - y, dc);
        DrawPixel(center.x - x, center.y + y, dc);
        DrawPixel(center.x - x, center.y - y, dc);
        DrawPixel(center.x + y, center.y + x, dc);
        DrawPixel(center.x + y, center.y - x, dc);
        DrawPixel(center.x - y, center.y - x, dc);
        DrawPixel(center.x - y, center.y + x, dc);
    }
}

void BigPixelCanvas::DesenharTriangulo2D(const Triang2D& triangulo) {
    wxClientDC dc(this);
    PrepareDC(dc);
    DesenharTriangulo2D(triangulo, dc);
}

void BigPixelCanvas::DesenharTriangulo2D(const Triang2D& triangulo, wxDC& dc) {
    // Aqui vai o código para desenhar um triângulo 2D
    Ponto p0 = triangulo.P1();
    Ponto p1 = triangulo.P2();
    Ponto p2 = triangulo.P3();

    int deltaYP0P1 = abs(p1.mY - p0.mY);
    int deltaYP0P2 = abs(p2.mY - p0.mY);
    int deltaYP1P2 = abs(p2.mY - p1.mY);

    //Determinando a aresta longa
    Ponto vInfArestaLonga;
    Ponto vSupArestaLonga;
    Ponto vQNaoEdaArestaLonga;
    if(deltaYP0P1 > deltaYP0P2){
        if(deltaYP0P1 > deltaYP1P2){
            //POP1 é a aresta longa
            if(p0.mY < p1.mY){
                vInfArestaLonga = p0;
                vSupArestaLonga = p1;
            }else{
                vInfArestaLonga = p1;
                vInfArestaLonga = p0;
            }
            vQNaoEdaArestaLonga = p2;
        }else{
            //P1P2 é a aresta longa
            if(p1.mY < p2.mY){
                vInfArestaLonga = p1;
                vSupArestaLonga = p2;
            }else{
                vInfArestaLonga = p2;
                vSupArestaLonga = p1;
            }         
            vQNaoEdaArestaLonga = p0;
        }
    }else{
        if(deltaYP0P2 > deltaYP1P2){
            //POP2 é a aresta longa
            if(p0.mY < p2.mY){
                vInfArestaLonga = p0;
                vSupArestaLonga = p2;
            }else{
                vInfArestaLonga = p2;
                vSupArestaLonga = p0;
            }
            vQNaoEdaArestaLonga = p1;
        }else{
            //P1P2 é a aresta longa
            if(p1.mY < p2.mY){
                vInfArestaLonga = p1;
                vSupArestaLonga = p2;
            }else{
                vInfArestaLonga = p2;
                vSupArestaLonga = p1;
            }
            vQNaoEdaArestaLonga = p0;
        }
    }

    
    int yMin = vInfArestaLonga.mY;
    int yMax = vQNaoEdaArestaLonga.mY;
    int y = yMin;

    float xEsq = float(vInfArestaLonga.mX);
    float xDir = xEsq;
    float dxDir = float(vSupArestaLonga.mX - vInfArestaLonga.mX)/(vSupArestaLonga.mY - vInfArestaLonga.mY);
    float dxEsq = float(vQNaoEdaArestaLonga.mX - vInfArestaLonga.mX)/(vQNaoEdaArestaLonga.mY - vInfArestaLonga.mY);
    bool estaAEsquerda = true;
    if(vQNaoEdaArestaLonga.mX - vInfArestaLonga.mX < 0){
        // O vértice que não faz parte da aresta longa está à esquerda desta
        dxDir = float(vSupArestaLonga.mX - vInfArestaLonga.mX)/(vSupArestaLonga.mY - vInfArestaLonga.mY);
        dxEsq = float(vQNaoEdaArestaLonga.mX - vInfArestaLonga.mX)/(vQNaoEdaArestaLonga.mY - vInfArestaLonga.mY);
    }else{
        // A aresta longa está à esquerda
        dxDir = float(vQNaoEdaArestaLonga.mX - vInfArestaLonga.mX)/(vQNaoEdaArestaLonga.mY - vInfArestaLonga.mY);
        dxEsq = float(vSupArestaLonga.mX - vInfArestaLonga.mX)/(vSupArestaLonga.mY - vInfArestaLonga.mY);
        estaAEsquerda = false;
    }
    while(y < yMax){
        for(int x = xEsq; x < xDir; ++x){
            DrawPixel(x, y, dc);
        }
        xEsq += dxEsq;
        xDir += dxDir;
        ++y;
    }

    yMin = vQNaoEdaArestaLonga.mY;
    yMax = vSupArestaLonga.mY;

    if(estaAEsquerda){
        dxEsq = float(vSupArestaLonga.mX - vQNaoEdaArestaLonga.mX)/(vSupArestaLonga.mY - vQNaoEdaArestaLonga.mY);
    }else{
        dxDir = float(vSupArestaLonga.mX - vQNaoEdaArestaLonga.mX)/(vSupArestaLonga.mY - vQNaoEdaArestaLonga.mY);
    }
    while(y < yMax){
        for(int x = xEsq; x < xDir; ++x){
            DrawPixel(x, y, dc);
        }
        xEsq += dxEsq;
        xDir += dxDir;
        ++y;
    }
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

