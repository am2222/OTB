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

#include "otbImage.h"
#include "otbLineSegmentDetector.h"
#include "otbDrawLineSpatialObjectListFilter.h"
#include "otbLineSpatialObjectList.h"
#include "itkLineIterator.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbLineSegmentDetector( int argc, char * argv[] )
{
  const   char * infname  = argv[1];
  const   char * outfname  = argv[2];
  
  typedef double                                                    InputPixelType;
  const   unsigned int                                              Dimension = 2;
  
  /** Typedefs */
  typedef otb::Image< InputPixelType,  Dimension >                  InputImageType;
  typedef InputImageType::IndexType                                 IndexType;
  typedef otb::ImageFileReader<InputImageType>                      ReaderType;
  typedef otb::ImageFileWriter<InputImageType>                      WriterType;
  typedef otb::DrawLineSpatialObjectListFilter< InputImageType ,InputImageType >         DrawLineListType;
  typedef otb::LineSegmentDetector<InputImageType , InputPixelType> lsdFilterType;
  typedef itk::LineIterator<InputImageType>                          LineIteratorFilter;


  /** Instanciation of smart pointer*/
  lsdFilterType::Pointer  lsdFilter = lsdFilterType::New();
  DrawLineListType::Pointer drawLineFilter =   DrawLineListType::New();
  ReaderType::Pointer reader = ReaderType::New();


  /** */
  //typedef otb::LineSpatialObjectList            LinesListType;
  //typedef LinesListType::LineType               LineType;
  //LinesListType::Pointer list = LinesListType::New();

  //LineType::PointListType             pointList;
  //LineType::LinePointType             pointBegin , pointEnd;
  //  IndexType                           IndexBegin , IndexEnd;

  /***/

  reader->SetFileName(infname);
  reader->GenerateOutputInformation();
  lsdFilter->SetInput(reader->GetOutput());

  drawLineFilter->SetInput(reader->GetOutput());
  drawLineFilter->SetInputLineSpatialObjectList(lsdFilter->GetOutput());

  
//   LinesListType::const_iterator it    = lsdFilter->GetOutput()->begin();
//   LinesListType::const_iterator itEnd  = lsdFilter->GetOutput()->end();
  
//   while(it != itEnd)
//     {
//       LineType::PointListType & pointsList = (*it)->GetPoints();
//       LineType::PointListType::const_iterator itPoints = pointsList.begin();
      
//       float x = (*itPoints).GetPosition()[0];
//       float y = (*itPoints).GetPosition()[1];
//       IndexBegin[0] = static_cast<int>(x) ; IndexBegin[1] = static_cast<int>(y);
      
//       itPoints++;

//       float x1 = (*itPoints).GetPosition()[0];
//       float y1 = (*itPoints).GetPosition()[1];
//       IndexEnd[0]= static_cast<int>(x1) ; IndexEnd[1] = static_cast<int>(y1);

//       LineIteratorFilter   itLine(reader->GetOutput(),IndexBegin,  IndexEnd);
//       itLine.GoToBegin();
//       while(!itLine.IsAtEnd())
// 	{
// 	  if(reader->GetOutput()->GetRequestedRegion().IsInside(itLine.GetIndex()))
// 	    itLine.Set(255.);
// 	  ++itLine;
// 	}
//       ++it;
//     }

 

  /** Write The Output Image*/
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(drawLineFilter->GetOutput());
  writer->Update();

  std::cout << " lsdFilter Ouput Size" << lsdFilter->GetOutput()->size() <<std::endl;
  
  
  return EXIT_SUCCESS;
}

