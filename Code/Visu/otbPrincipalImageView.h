/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   4 avril 2005
  Version   :   
  Role      :   Classe permettant de g�rer l'affichage de la fenetre "Principal"
  $Id$

=========================================================================*/
// OTB-FA-00026-CS
#ifndef otbPrincipalImageView_h
#define otbPrincipalImageView_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "otbGLVectorImageViewBase.h"

namespace otb
{
  
/**
* \class PrincipalImageView
* \brief Class for viewing an image in entier resolution
* 
* See GLVectorImageViewBase.h for details...
  **/
template <class TPixel>
class ITK_EXPORT PrincipalImageView :  public GLVectorImageViewBase<TPixel>
                                
{
public:
    typedef PrincipalImageView                                  Self;
    typedef GLVectorImageViewBase<TPixel>                       Superclass;
    typedef itk::SmartPointer<Self>                             Pointer;
    typedef itk::SmartPointer<const Self>                       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(PrincipalImageView,GLVectorImageViewBase);

  
  typedef typename Superclass::ImageType                ImageType;
  typedef typename Superclass::ImagePointer             ImagePointer;
  typedef typename Superclass::ImageConstPointer        ImageConstPointer;
  typedef typename Superclass::RegionType               RegionType;
  typedef typename Superclass::SizeType                 SizeType;
  typedef typename Superclass::IndexType                IndexType;
  typedef typename Superclass::PixelType                PixelType;

  virtual int  handle(int event);

  /** Show the image (display the window) */
  virtual void Show(void)
  {
        this->BuildWithImageRegion();
        this->m_flDoubleWindow->show();
        this->m_flDoubleWindow->size(           this->GetViewImageRegion().GetSize()[0], 
  					        this->GetViewImageRegion().GetSize()[1]);
        this->m_flDoubleWindow->label( this->GetLabel() );
        this->show();
        this->update();
  }

protected:
  
  /*! Standard constructor */
  PrincipalImageView();
  /*! Standard destructor */
  virtual ~PrincipalImageView(void);
  

};

} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPrincipalImageView.txx"
#endif

#endif

