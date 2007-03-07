//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimMatrix4x4.h,v 1.10 2005/09/06 16:23:41 gpotts Exp $

#ifndef ossimMatrix4x4_HEADER
#define ossimMatrix4x4_HEADER
#include <iostream>
#include "matrix/newmat.h"
#include "matrix/newmatio.h"
#include "ossimColumnVector4d.h"
#include "ossimColumnVector3d.h"

class OSSIMDLLEXPORT ossimMatrix4x4
{
 public:
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimMatrix4x4& data)
    {
      return out << data.getData();
    }
  ossimMatrix4x4();
  ossimMatrix4x4(const NEWMAT::Matrix& m);
  ossimMatrix4x4(double v00, double v01, double v02, double v03,
                 double v10, double v11, double v12, double v13,
                 double v20, double v21, double v22, double v23,
                 double v30, double v31, double v32, double v33);
  ossimMatrix4x4(const ossimMatrix4x4& rhs)
    :theData(4,4)
    {
      theData[0][0] = rhs.theData[0][0];
      theData[0][1] = rhs.theData[0][1];
      theData[0][2] = rhs.theData[0][2];
      theData[0][3] = rhs.theData[0][3];
      theData[1][0] = rhs.theData[1][0];
      theData[1][1] = rhs.theData[1][1];
      theData[1][2] = rhs.theData[1][2];
      theData[1][3] = rhs.theData[1][3];
      theData[2][0] = rhs.theData[2][0];
      theData[2][1] = rhs.theData[2][1];
      theData[2][2] = rhs.theData[2][2];
      theData[2][3] = rhs.theData[2][3];
      theData[3][0] = rhs.theData[3][0];
      theData[3][1] = rhs.theData[3][1];
      theData[3][2] = rhs.theData[3][2];
      theData[3][3] = rhs.theData[3][3];
    }
  ossimMatrix4x4 operator+ (const ossimMatrix4x4& rhs)const
    {
      return ossimMatrix4x4(theData[0][0] + rhs.theData[0][0], theData[0][1] + rhs.theData[0][1], theData[0][2] + rhs.theData[0][2], theData[0][3] + rhs.theData[0][3],
                            theData[1][0] + rhs.theData[1][0], theData[1][1] + rhs.theData[1][1], theData[1][2] + rhs.theData[1][2], theData[1][3] + rhs.theData[1][3],
                            theData[2][0] + rhs.theData[2][0], theData[2][1] + rhs.theData[2][1], theData[2][2] + rhs.theData[2][2], theData[2][3] + rhs.theData[2][3],
                            theData[3][0] + rhs.theData[3][0], theData[3][1] + rhs.theData[3][1], theData[3][2] + rhs.theData[3][2], theData[3][3] + rhs.theData[3][3]);
    }
  ossimMatrix4x4 operator- (const ossimMatrix4x4& rhs)const
    {
      return ossimMatrix4x4(theData[0][0] - rhs.theData[0][0], theData[0][1] - rhs.theData[0][1], theData[0][2] - rhs.theData[0][2], theData[0][3] - rhs.theData[0][3],
                            theData[1][0] - rhs.theData[1][0], theData[1][1] - rhs.theData[1][1], theData[1][2] - rhs.theData[1][2], theData[1][3] - rhs.theData[1][3],
                            theData[2][0] - rhs.theData[2][0], theData[2][1] - rhs.theData[2][1], theData[2][2] - rhs.theData[2][2], theData[2][3] - rhs.theData[2][3],
                            theData[3][0] - rhs.theData[3][0], theData[3][1] - rhs.theData[3][1], theData[3][2] - rhs.theData[3][2], theData[3][3] - rhs.theData[3][3]);
    }
  friend ossimColumnVector4d operator * (const ossimMatrix4x4& lhs, const ossimColumnVector4d& rhs)
      {
         return ossimColumnVector4d( (lhs.theData[0][0]*rhs[0] + lhs.theData[0][1]*rhs[1] + lhs.theData[0][2]*rhs[2] + lhs.theData[0][3]*rhs[3]),
                                     (lhs.theData[1][0]*rhs[0] + lhs.theData[1][1]*rhs[1] + lhs.theData[1][2]*rhs[2] + lhs.theData[1][3]*rhs[3]),
                                     (lhs.theData[2][0]*rhs[0] + lhs.theData[2][1]*rhs[1] + lhs.theData[2][2]*rhs[2] + lhs.theData[2][3]*rhs[3]),
                                     (lhs.theData[3][0]*rhs[0] + lhs.theData[3][1]*rhs[1] + lhs.theData[3][2]*rhs[2] + lhs.theData[3][3]*rhs[3]));
      
    }
   friend ossimColumnVector3d operator * (const ossimMatrix4x4& lhs, const ossimColumnVector3d& rhs)
      {
         // assume the 4 coordinate (homogeneous coord) of the 3d vector is 1
         return ossimColumnVector3d( (lhs.theData[0][0]*rhs[0] + lhs.theData[0][1]*rhs[1] + lhs.theData[0][2]*rhs[2] + lhs.theData[0][3]),
                                     (lhs.theData[1][0]*rhs[0] + lhs.theData[1][1]*rhs[1] + lhs.theData[1][2]*rhs[2] + lhs.theData[1][3]),
                                     (lhs.theData[2][0]*rhs[0] + lhs.theData[2][1]*rhs[1] + lhs.theData[2][2]*rhs[2] + lhs.theData[2][3]));         
      }
   ossimColumnVector4d rotateOnly(const ossimColumnVector4d& rhs)const
      {
         // multiply only by the 3x3 submatrix.
         return ossimColumnVector4d( (theData[0][0]*rhs[0] + theData[0][1]*rhs[1] + theData[0][2]*rhs[2]),
                                     (theData[1][0]*rhs[0] + theData[1][1]*rhs[1] + theData[1][2]*rhs[2]),
                                     (theData[2][0]*rhs[0] + theData[2][1]*rhs[1] + theData[2][2]*rhs[2]));         
      }
   ossimColumnVector3d rotateOnly(const ossimColumnVector3d& rhs)const
      {
         // assume the 4 coordinate (homogeneous coord) of the 3d vector is 1
         return ossimColumnVector3d( (theData[0][0]*rhs[0] + theData[0][1]*rhs[1] + theData[0][2]*rhs[2]),
                                     (theData[1][0]*rhs[0] + theData[1][1]*rhs[1] + theData[1][2]*rhs[2]),
                                     (theData[2][0]*rhs[0] + theData[2][1]*rhs[1] + theData[2][2]*rhs[2]));
      }
   ossimMatrix4x4 operator*(double scalar)const
      {
         return ossimMatrix4x4(theData[0][0]*scalar, theData[0][1]*scalar, theData[0][2]*scalar, theData[0][3]*scalar,
                               theData[1][0]*scalar, theData[1][1]*scalar, theData[1][2]*scalar, theData[1][3]*scalar,
                               theData[2][0]*scalar, theData[2][1]*scalar, theData[2][2]*scalar, theData[2][3]*scalar,
                               theData[0][0]*scalar, theData[0][1]*scalar, theData[0][2]*scalar, theData[0][3]*scalar);
         
      }
   ossimMatrix4x4 operator*(const ossimMatrix4x4& rhs)const
      {
         ossimMatrix4x4 m;
         
         // element 0,0 is first row time first column
         m.theData[0][0] = (theData[0][0]*rhs.theData[0][0] +
                         theData[0][1]*rhs.theData[1][0] +
                         theData[0][2]*rhs.theData[2][0] +
                         theData[0][3]*rhs.theData[3][0]);
         
         // element 0, 1 is first row second column
         m.theData[0][1] = (theData[0][0]*rhs.theData[0][1] +
                         theData[0][1]*rhs.theData[1][1] +
                         theData[0][2]*rhs.theData[2][1] +
                         theData[0][3]*rhs.theData[3][1]);
         
         // element 0, 2 is first row third column
         m.theData[0][2] = (theData[0][0]*rhs.theData[0][2] +
                         theData[0][1]*rhs.theData[1][2] +
                         theData[0][2]*rhs.theData[2][2] +
                         theData[0][3]*rhs.theData[3][2]);
         
         // element 0, 3 is first row fourth column
         m.theData[0][3] = (theData[0][0]*rhs.theData[0][3] +
                         theData[0][1]*rhs.theData[1][3] +
                         theData[0][2]*rhs.theData[2][3] +
                         theData[0][3]*rhs.theData[3][3]);
         
         // element 1, 0 is second row first column
         m.theData[1][0] = (theData[1][0]*rhs.theData[0][0] +
                         theData[1][1]*rhs.theData[1][0] +
                         theData[1][2]*rhs.theData[2][0] +
                         theData[1][3]*rhs.theData[3][0]);
         
         // element 1, 1 is second row second column
         m.theData[1][1] = (theData[1][0]*rhs.theData[0][1] +
                         theData[1][1]*rhs.theData[1][1] +
                         theData[1][2]*rhs.theData[2][1] +
                         theData[1][3]*rhs.theData[3][1]);
         
         // element 1, 2 is second row third column
         m.theData[1][2] = (theData[1][0]*rhs.theData[0][2] +
                         theData[1][1]*rhs.theData[1][2] +
                         theData[1][2]*rhs.theData[2][2] +
                         theData[1][3]*rhs.theData[3][2]);
         
         // element 1, 3 is second row fourth column
         m.theData[1][3] = (theData[1][0]*rhs.theData[0][3] +
                         theData[1][1]*rhs.theData[1][3] +
                         theData[1][2]*rhs.theData[2][3] +
                         theData[1][3]*rhs.theData[3][3]);
         
         // element 2, 0 is third row first column
         m.theData[2][0] = (theData[2][0]*rhs.theData[0][0] +
                         theData[2][1]*rhs.theData[1][0] +
                         theData[2][2]*rhs.theData[2][0] +
                         theData[2][3]*rhs.theData[3][0]);
         
         // element 2, 1 is third row second column
         m.theData[2][1] = (theData[2][0]*rhs.theData[0][1] +
                         theData[2][1]*rhs.theData[1][1] +
                         theData[2][2]*rhs.theData[2][1] +
                         theData[2][3]*rhs.theData[3][1]);
         
         // element 2, 2 is third row third column
         m.theData[2][2] = (theData[2][0]*rhs.theData[0][2] +
                         theData[2][1]*rhs.theData[1][2] +
                         theData[2][2]*rhs.theData[2][2] +
                         theData[2][3]*rhs.theData[3][2]);
         
         // element 2, 3 is third row fourth column
         m.theData[2][3] = (theData[2][0]*rhs.theData[0][3] +
                         theData[2][1]*rhs.theData[1][3] +
                         theData[2][2]*rhs.theData[2][3] +
                         theData[2][3]*rhs.theData[3][3]);
         
         // element 3, 0 is fourth row first column
         m.theData[3][0] = (theData[3][0]*rhs.theData[0][0] +
                         theData[3][1]*rhs.theData[1][0] +
                         theData[3][2]*rhs.theData[2][0] +
                         theData[3][3]*rhs.theData[3][0]);
         
         // element 3, 1 is fourth row second column
         m.theData[3][1] = (theData[3][0]*rhs.theData[0][1] +
                         theData[3][1]*rhs.theData[1][1] +
                         theData[3][2]*rhs.theData[2][1] +
                         theData[3][3]*rhs.theData[3][1]);
         
         // element 3, 2 is fourth row third column
         m.theData[3][2] = (theData[3][0]*rhs.theData[0][2] +
                         theData[3][1]*rhs.theData[1][2] +
                         theData[3][2]*rhs.theData[2][2] +
                         theData[3][3]*rhs.theData[3][2]);
         
         // element 3, 3 is fourth row fourth column
         m.theData[3][3] = (theData[3][0]*rhs.theData[0][3] +
                         theData[3][1]*rhs.theData[1][3] +
                         theData[3][2]*rhs.theData[2][3] +
                         theData[3][3]*rhs.theData[3][3]);
         
         return m;
      }

   /*!
    * uses the matrix package to compute the inverse
    */
   ossimMatrix4x4& i()
      {
         theData = theData.i();
         
         return *this;
      }
   
   /*!
    * Uses the matrix package to compute the transpose.
    */
   ossimMatrix4x4& t()
      {        
         theData = theData.t();

         return *this;
      }
   
   /*!
    * uses the matrix package to compute the determinant
    */
   double getDeterminant()const
      {
         return theData.Determinant();
      }
   
   /*!
    * Uses the matrix package to compute the eigenvalues for this
    * matrix
    */ 
   ossimColumnVector3d getEigenValues()const;
   
   static NEWMAT::Matrix createIdentity();
   static NEWMAT::Matrix createRotateOnly(const ossimMatrix4x4& aMatrix);
  
   static NEWMAT::Matrix createZero();
   static NEWMAT::Matrix createTranslationMatrix(double x,
                                                 double y,
                                                 double z);
   static NEWMAT::Matrix createRotationMatrix(double angleX,
                                              double angleY,
                                              double angleZ,
                                              ossimCoordSysOrientMode orientationMode=OSSIM_RIGHT_HANDED);
   static NEWMAT::Matrix createRotationXMatrix(double angle,
                                               ossimCoordSysOrientMode orientationMode=OSSIM_RIGHT_HANDED);
   static NEWMAT::Matrix createRotationYMatrix(double angle,
                                               ossimCoordSysOrientMode orientationMode=OSSIM_RIGHT_HANDED);
   static NEWMAT::Matrix createRotationZMatrix(double angle,
                                               ossimCoordSysOrientMode orientationMode=OSSIM_RIGHT_HANDED);
   
   static NEWMAT::Matrix createScaleMatrix(double X, double Y, double Z);
   
  const NEWMAT::Matrix& getData()const{return theData;}
  NEWMAT::Matrix& getData(){return theData;}

private:
  NEWMAT::Matrix theData; 
};

#endif
