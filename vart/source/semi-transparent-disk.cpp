/// \file semi-transparent-disk.cpp
/// \brief Implementation file for V-ART class "SemiTransparentDisk".

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/glu.h>
#endif
#include "vart/semi-transparent-disk.h"
#include <iostream>

using namespace std;

VART::SemiTransparentDisk::SemiTransparentDisk() : VART::GraphicObj() {
    radius = 1.5;
    material = VART::Material::PLASTIC_RED();
    material.SetPlasticColor(VART::Color(255, 255, 255, 100));
    ComputeBoundingBox();
}

VART::SemiTransparentDisk::SemiTransparentDisk(const VART::SemiTransparentDisk& semiTransparentDisk)
{
    this->operator=(semiTransparentDisk);
}

VART::SemiTransparentDisk& VART::SemiTransparentDisk::operator=(const VART::SemiTransparentDisk& semiTransparentDisk)
{
    this->GraphicObj::operator=(semiTransparentDisk);
    radius = semiTransparentDisk.radius;
    material = semiTransparentDisk.material;
    return *this;
}

VART::SceneNode * VART::SemiTransparentDisk::Copy()
{
    return new SemiTransparentDisk(*this);
}

VART::SemiTransparentDisk::SemiTransparentDisk( float fRad ) : VART::GraphicObj() {
    radius = fRad;
    ComputeBoundingBox();
}

void VART::SemiTransparentDisk::SetRadius( float r ) {
    radius = r;
    ComputeBoundingBox();
}

float VART::SemiTransparentDisk::GetRadius( void ) {
    return radius;
}

void VART::SemiTransparentDisk::ComputeBoundingBox() {
    bBox.SetSmallerX(-radius);
    bBox.SetSmallerY(-radius);
    bBox.SetSmallerZ(-radius);
    bBox.SetGreaterX(radius);
    bBox.SetGreaterY(radius);
    bBox.SetGreaterZ(radius);
    //oobBox=VART::OOBoundingBox(bBox);
}

bool VART::SemiTransparentDisk::DrawInstanceOGL() const {
#ifdef VART_OGL
    GLUquadricObj* qObj = gluNewQuadric();
    bool result = true;

    if (show)
    {
        if( material.GetTexture().HasData() )
        {
            gluQuadricTexture(qObj,GL_TRUE);
        }
        result = material.DrawOGL();
        gluQuadricDrawStyle(qObj, GLU_FILL);
        gluQuadricNormals(qObj, GLU_SMOOTH);
        gluDisk(qObj, 0.0, radius, 15, 1);
        gluDeleteQuadric(qObj);
    }
    if (bBox.visible)
        bBox.DrawInstanceOGL();
    if (recBBox.visible)
        recBBox.DrawInstanceOGL();
    return result;
#else
    return false;
#endif
}
