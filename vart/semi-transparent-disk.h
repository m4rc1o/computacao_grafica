/// \file semi-transparent-disk.h
/// \brief Header file for V-ART class "SemiTransparentDisk".

#ifndef VART_SEMITRANSPARENT_H
#define VART_SEMITRANSPARENT_H

#include "vart/graphicobj.h"
#include "vart/material.h"

namespace VART {
/// \class SemiTransparentDisk semiTransparentDisk.h
/// \brief A semiTransparentDisk.
///
/// SemiTransparentDisks are created on the xy plane and with center at the origin.
    class SemiTransparentDisk : public GraphicObj {
        public:
            SemiTransparentDisk();
            /// \brief Creates a new copy of an semiTransparentDisk.
            SemiTransparentDisk(const VART::SemiTransparentDisk& semiTransparentDisk);
            
            SemiTransparentDisk( float fRad );
            
            /// Copies all SemiTransparentDisk data to another semiTransparentDisk.
            VART::SemiTransparentDisk & operator=(const VART::SemiTransparentDisk & semiTransparentDisk);
            
            /// \brief Returns a copy of an SemiTransparentDisk. Every derived class must reimplements 
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();
            
            void SetRadius( float r );
            /// Material assignment.
            void SetMaterial(const Material& mat) { material = mat; }
            /// Returns of copy of the cylinder's material.
            Material GetMaterial() const { return material; }
            float GetRadius();
            /// \brief Draws the semiTransparentDisk using OpenGL engine.
            /// \return false if V-ART has not been compiled with OpenGL support.
            bool DrawInstanceOGL() const;
            virtual void ComputeBoundingBox();
        private:
            Material material;
            float radius;
    }; // end class declaration
} // end namespace
#endif
