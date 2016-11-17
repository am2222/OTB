/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletImageFilter_h
#define __otbWaveletImageFilter_h

#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletTransform.h"
#include "otbWaveletsBandsListToWaveletsSynopsisImageFilter.h"

namespace otb {

/** \class WaveletImageFilter
 * \brief
 *
 * \ingroup
 * \sa
 */
template < class TInputImage,
           class TOutputImage,
           Wavelet::Wavelet TMotherWaveletOperator >
class WaveletImageFilter :
  public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef TInputImage                                                                InputImageType;
  typedef TOutputImage                                                               OutputImageType;

  itkStaticConstMacro( ImageDimension,            unsigned int,                      ::itk::GetImageDimension<InputImageType>::ImageDimension );
  itkStaticConstMacro( MotherWaveletOperator,     MotherWaveletOperatorEnumType,     TMotherWaveletOperator );
  itkStaticConstMacro( DirectionOfTransformation, DirectionOfTransformationEnumType, otb::Wavelet::FORWARD );

  typedef WaveletImageFilter<InputImageType,OutputImageType, TMotherWaveletOperator>  Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType>                    Superclass;
  typedef itk::SmartPointer<Self>                                                     Pointer;
  typedef itk::SmartPointer<const Self>                                               ConstPointer;

  typedef typename InputImageType::PixelType                                          InputPixelType;

  typedef otb::WaveletOperator< TMotherWaveletOperator, otb::Wavelet::FORWARD, InputPixelType, ImageDimension >
    WaveletOperatorType;
  typedef otb::WaveletFilterBank< InputImageType, InputImageType, WaveletOperatorType, otb::Wavelet::FORWARD >
    FilterBankType;
  typedef otb::WaveletTransform< InputImageType, InputImageType, FilterBankType, otb::Wavelet::FORWARD >
    WaveletTransformFilterType;
  typedef typename WaveletTransformFilterType::Pointer
    WaveletTransformFilterPointerType;

  typedef typename WaveletTransformFilterType::OutputImageListType
    WaveletTransformFilterOutputImageListType;
  typedef otb::WaveletsBandsListToWaveletsSynopsisImageFilter<WaveletTransformFilterOutputImageListType, OutputImageType>
    WaveletBandsListToWaveletsSynopsisImageFilterType;
  typedef typename WaveletBandsListToWaveletsSynopsisImageFilterType::Pointer
    WaveletBandsListToWaveletsSynopsisImageFilterPointerType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(WaveletImageFilter, ImageToImageFilter);

  itkGetMacro(NumberOfDecompositions,unsigned int);
  itkSetMacro(NumberOfDecompositions,unsigned int);

protected:
  WaveletImageFilter();
  virtual ~WaveletImageFilter();

  virtual void GenerateInputRequestedRegion();

  virtual void GenerateData();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  WaveletImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  WaveletTransformFilterPointerType m_WaveletTransform;
  WaveletBandsListToWaveletsSynopsisImageFilterPointerType m_WaveletBandsListToWaveletsSynopsis;
  unsigned int m_NumberOfDecompositions;
};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletImageFilter.txx"
#endif

#endif
