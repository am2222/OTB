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
#ifndef otbSharkKMeansMachineLearningModel_h
#define otbSharkKMeansMachineLearningModel_h



#include "itkLightObject.h"
#include "otbMachineLearningModel.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif

#include "otb_shark.h"
#include "shark/Models/Clustering/HardClusteringModel.h"
#include "shark/Models/Clustering/SoftClusteringModel.h"
#include "shark/Models/Clustering/Centroids.h"
#include "shark/Models/Clustering/ClusteringModel.h"
#include "shark/Algorithms/KMeans.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

using namespace shark;

/** \class SharkKMeansMachineLearningModel
 *  \brief Shark version of Random Forests algorithm
 *
 *  This is a specialization of MachineLearningModel class allowing to
 *  use Shark implementation of the Random Forests algorithm.
 *
 *  It is noteworthy that training step is parallel.
 *
 *  For more information, see
 *  http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/kmeans.html
 *
 *  \ingroup OTBUnsupervised
 */
namespace otb
{
template<class TInputValue, class TTargetValue>
class ITK_EXPORT SharkKMeansMachineLearningModel : public MachineLearningModel<TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef SharkKMeansMachineLearningModel Self;
  typedef MachineLearningModel<TInputValue, TTargetValue> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::InputValueType InputValueType;
  typedef typename Superclass::InputSampleType InputSampleType;
  typedef typename Superclass::InputListSampleType InputListSampleType;
  typedef typename Superclass::TargetValueType TargetValueType;
  typedef typename Superclass::TargetSampleType TargetSampleType;
  typedef typename Superclass::TargetListSampleType TargetListSampleType;
  typedef typename Superclass::ConfidenceValueType ConfidenceValueType;
  typedef typename Superclass::ConfidenceSampleType ConfidenceSampleType;
  typedef typename Superclass::ConfidenceListSampleType ConfidenceListSampleType;

  typedef HardClusteringModel<RealVector> ClusteringModelType;
  typedef ClusteringModelType::OutputType ClusteringOutputType;

  /** Run-time type information (and related methods). */
  itkNewMacro( Self );
  itkTypeMacro( SharkKMeansMachineLearningModel, MachineLearningModel );

  /** Train the machine learning model */
  virtual void Train() ITK_OVERRIDE;

  /** Save the model to file */
  virtual void Save(const std::string &filename, const std::string &name = "") ITK_OVERRIDE;

  /** Load the model from file */
  virtual void Load(const std::string &filename, const std::string &name = "") ITK_OVERRIDE;

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  virtual bool CanReadFile(const std::string &) ITK_OVERRIDE;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  virtual bool CanWriteFile(const std::string &) ITK_OVERRIDE;
  //@}

  /** Get the maximum number of iteration for the kMeans algorithm.*/
  itkGetMacro( MaximumNumberOfIterations, unsigned
          int );
  /** Set the maximum number of iteration for the kMeans algorithm.*/
  itkSetMacro( MaximumNumberOfIterations, unsigned
          int );

  /** Get the number of class for the kMeans algorithm.*/
  itkGetMacro( K, unsigned
          int );
  /** Set the number of class for the kMeans algorithm.*/
  itkSetMacro( K, unsigned
          int );

  /** If true, normalized input data sample list */
  itkGetMacro( Normalized, bool );
  itkSetMacro( Normalized, bool );

protected:
  /** Constructor */
  SharkKMeansMachineLearningModel();

  /** Destructor */
  virtual ~SharkKMeansMachineLearningModel();

  /** Predict values using the model */
  virtual TargetSampleType
  DoPredict(const InputSampleType &input, ConfidenceValueType *quality = ITK_NULLPTR) const ITK_OVERRIDE;


  virtual void DoPredictBatch(const InputListSampleType *, const unsigned int &startIndex, const unsigned int &size,
                              TargetListSampleType *, ConfidenceListSampleType * = ITK_NULLPTR) const ITK_OVERRIDE;

  template<typename DataType>
  DataType NormalizeData(const DataType &data) const;

  /** PrintSelf method */
  void PrintSelf(std::ostream &os, itk::Indent indent) const;

private:
  SharkKMeansMachineLearningModel(const Self &); //purposely not implemented
  void operator=(const Self &); //purposely not implemented

  // Parameters set by the user
  bool m_Normalized;
  unsigned int m_K;
  unsigned int m_MaximumNumberOfIterations;


  /** Centroids results form kMeans */
  Centroids centroids;


  /** shark Model could be SoftClusteringModel or HardClusteringModel */
  boost::shared_ptr<ClusteringModelType> m_ClusteringModel;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#include "otbSharkKMeansMachineLearningModel.txx"

#endif

#endif