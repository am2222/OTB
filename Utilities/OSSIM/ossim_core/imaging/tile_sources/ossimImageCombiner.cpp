//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageCombiner.cpp,v 1.70 2005/10/13 20:17:49 gpotts Exp $
#include <imaging/tile_sources/ossimImageCombiner.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ossimIrect.h>
#include <imaging/ossimImageData.h>
#include <base/common/ossimTrace.h>

using namespace std;

RTTI_DEF2(ossimImageCombiner, "ossimImageCombiner", ossimImageSource, ossimConnectableObjectListener)
static ossimTrace traceDebug ("ossimImageCombiner:debug");

ossimImageCombiner::ossimImageCombiner()
   :ossimImageSource(NULL,
                     0,
                     0,
                     false,
                     false),
    theLargestNumberOfInputBands(0),
    theInputToPassThrough(0),
    theHasDifferentInputs(false),
    theNormTile(NULL),

    theCurrentIndex(0)
{
	theComputeFullResBoundsFlag = true;
   // until something is set we will just set the blank tile
   // to a 1 band unsigned char type
   addListener((ossimConnectableObjectListener*)this);
}
ossimImageCombiner::ossimImageCombiner(ossimObject* owner,
                                       int numberOfInputs,
                                       int numberOfOutputs,
                                       bool inputListIsFixedFlag,
                                       bool outputListIsFixedFlag)
   :ossimImageSource(owner,
                     numberOfInputs,
                     numberOfOutputs,
                     inputListIsFixedFlag,
                     outputListIsFixedFlag),
    theLargestNumberOfInputBands(0),
    theInputToPassThrough(0),
    theHasDifferentInputs(false),
    theNormTile(NULL),
    theCurrentIndex(0)
{
   addListener((ossimConnectableObjectListener*)this);
   theComputeFullResBoundsFlag = true;
}

ossimImageCombiner::ossimImageCombiner(const std::vector<ossimImageSource*>& inputSources)
   :ossimImageSource(NULL,
                     inputSources.size(),
                     0,
                     false,
                     false),
    theLargestNumberOfInputBands(0),
    theInputToPassThrough(0),
    theHasDifferentInputs(false),
    theNormTile(NULL),
    theCurrentIndex(0)
{
	theComputeFullResBoundsFlag = true;
   for(ossim_uint32 index = 0; index < inputSources.size(); ++index)
   {
      connectMyInputTo(index, inputSources[index]);
   }
   addListener((ossimConnectableObjectListener*)this);
   initialize();
}

ossimImageCombiner::~ossimImageCombiner()
{
   removeListener((ossimConnectableObjectListener*)this);
}

void ossimImageCombiner::getDecimationFactor(ossim_uint32 resLevel,
                                             ossimDpt& result) const
{
   if(getInput(0))
   {
      ossimImageSourceInterface* temp = PTR_CAST(ossimImageSourceInterface, getInput(0));
      if(temp)
      {
         temp->getDecimationFactor(resLevel, result);
      }     
   }
   else
   {
      result.x = 1;
      result.y = 1;
   }
}

void ossimImageCombiner::getDecimationFactors(vector<ossimDpt>& decimations) const
{
   if(getInput(0))
   {
      ossimImageSourceInterface* temp = PTR_CAST(ossimImageSourceInterface, getInput(0));
      if(temp)
      {
         temp->getDecimationFactors(decimations);
      }     
   }
   else
   {
      decimations.push_back(ossimDpt(1,1));
   }
}

ossim_uint32 ossimImageCombiner::getNumberOfDecimationLevels()const
{
   if(getInput(0))
   {
      ossimImageSourceInterface* temp = PTR_CAST(ossimImageSourceInterface, getInput(0));
      if(temp)
      {
         return temp->getNumberOfDecimationLevels();
      }     
   }
   
   return 1;
}

ossimIrect ossimImageCombiner::getBoundingRect(ossim_uint32 resLevel)const
{
   static const char* MODULE = "ossimImageCombiner::getBoundingRect";
   ossimIrect result;
   if(theComputeFullResBoundsFlag)
   {
      precomputeBounds();
   }
   
   double scale = 1.0/std::pow(2.0, (double)resLevel);
   ossimDpt scalar(scale, scale);
   result.makeNan();
   
   ossim_uint32 inputIndex = 0;
   ossimImageSourceInterface* interface;
   for(inputIndex = 0;inputIndex < getNumberOfInputs(); ++inputIndex)
   {
      interface = PTR_CAST(ossimImageSourceInterface, getInput(inputIndex));
      if(interface)
      {
         ossimIrect rect  = theFullResBounds[inputIndex];
         if(result.hasNans())
         {
            result = rect;
         }
         else if(!rect.hasNans())
         {
            if(traceDebug())
            {
               CLOG << "rect " << inputIndex << " = " << result << endl;
            }
            rect = rect*scale;
            result = result.combine(rect);
         }
      }
   }
   if(traceDebug())
   {
      CLOG << "resulting bounding rect =  " << result << endl;
   }
   
   return result;
}

ossim_uint32 ossimImageCombiner::getNumberOfInputBands() const
{
   return theLargestNumberOfInputBands;
}

ossimScalarType ossimImageCombiner::getOutputScalarType() const
{
   if(getInput(theInputToPassThrough))
   {
      ossimImageSourceInterface* temp = PTR_CAST(ossimImageSourceInterface, getInput(theInputToPassThrough));
      if(temp)
      {
         return temp->getOutputScalarType();
      }
   }

   return OSSIM_SCALAR_UNKNOWN;
}

ossim_uint32 ossimImageCombiner::getTileWidth()const
{
   if(getInput(theInputToPassThrough))
   {
      ossimImageSourceInterface* temp = PTR_CAST(ossimImageSourceInterface, getInput(theInputToPassThrough));
      if(temp)
      {
         return temp->getTileWidth();
      }     
   }
   
   return 1;
}

ossim_uint32 ossimImageCombiner::getTileHeight()const
{
   if(getInput(theInputToPassThrough))
   {
      ossimImageSourceInterface* temp = PTR_CAST(ossimImageSourceInterface, getInput(theInputToPassThrough));
      if(temp)
      {
         return temp->getTileHeight();
      }     
   }
   
   return 1;
}

double ossimImageCombiner::getNullPixelValue(ossim_uint32 band)const
{
   if(getInput(theInputToPassThrough))
   {
      ossimImageSourceInterface* temp = PTR_CAST(ossimImageSourceInterface, getInput(theInputToPassThrough));
      if(temp)
      {
         ossim_uint32 bands = temp->getNumberOfOutputBands();
         if(band < bands)
         {
            return temp->getNullPixelValue(band);
         }
         else
         {
            return temp->getNullPixelValue(bands-1);
         }
      }     
   }
   
   return 0.0;
}

double ossimImageCombiner::getMinPixelValue(ossim_uint32 band)const
{
   const char* MODULE = "ossimImageCombiner::getMinPixelValue";
   
   if(traceDebug())
   {
      CLOG <<"entered..." << endl;
   }
   if(!getNumberOfInputs())
   {
      return OSSIM_DBL_NAN;
   }
   double result = 1.0/DBL_EPSILON;
   
   for(ossim_uint32 index = 0; index < getNumberOfInputs();++index)
   {
      ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface, getInput(index));
      if(input)
      {
         ossim_uint32 bands = input->getNumberOfOutputBands();
         double temp = 0;
         if(band < bands)
         {
            temp = input->getMinPixelValue(band);
         }
         else
         {
            temp = input->getMinPixelValue(bands-1);
         }
         
         if(temp < result)
         {
            result = temp;
         }
      }
   }
   if(traceDebug())
   {
      CLOG << "min pixel returned is = " << result;
   }
   if(traceDebug())
   {
      CLOG <<"leaving..." << endl;
   }
   return result;
}

double ossimImageCombiner::getMaxPixelValue(ossim_uint32 band)const
{
   const char* MODULE = "ossimImageCombiner::getMaxPixelValue";
   if(traceDebug())
   {
      CLOG <<"entered..." << endl;
   }
   if(!getNumberOfInputs())
   {
      return OSSIM_DBL_NAN;
   }
   double result = -1.0/DBL_EPSILON;
   
   for(ossim_uint32 idx = 0; idx < getNumberOfInputs();++idx)
   {
      ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface, getInput(idx));
      if(input)
      {
         ossim_uint32 bands = input->getNumberOfOutputBands();
         double temp = 0;
         if(band < bands)
         {
            temp = input->getMaxPixelValue(band);
         }
         else
         {
            temp = input->getMaxPixelValue(bands-1);
         }
         if(temp > result)
         {
            result = temp;
         }
      }
   }

   if(traceDebug())
   {
      CLOG << "max pixel returned is = " << result;
   }
   if(traceDebug())
   {
      CLOG <<"leaving..." << endl;
   }
   
   return result;
}

void ossimImageCombiner::initialize()
{
   
   theLargestNumberOfInputBands = 0;
   theComputeFullResBoundsFlag = true;
   
   // now find the largest number of bands
   //
   ossim_uint32 size = getNumberOfInputs();
   ossimScalarType scalarType = OSSIM_SCALAR_UNKNOWN;
   ossim_uint32 scalarSizeInBytes = 0;
   theInputToPassThrough = 0;
   theHasDifferentInputs= false;
   if(size > 0)
   {
      for(ossim_uint32 idx = 0; idx < size; ++idx)
      {
         ossimImageSourceInterface* temp = PTR_CAST(ossimImageSourceInterface, getInput(idx));
         if(temp)
         {
            ossim_uint32 numberOfBands = temp->getNumberOfOutputBands();
            if(numberOfBands > theLargestNumberOfInputBands)
            {
               theLargestNumberOfInputBands = numberOfBands;
            }
            ossimScalarType current = temp->getOutputScalarType();
            if(current != scalarType)
            {
               if(scalarType == OSSIM_SCALAR_UNKNOWN)
               {
                  scalarType = current;
                  scalarSizeInBytes = ossimGetScalarSizeInBytes(scalarType);
                  theInputToPassThrough = idx;
               }
               else
               {
                  theHasDifferentInputs = true;
                  if(scalarSizeInBytes < ossimGetScalarSizeInBytes(current))
                  {
                     scalarType        = current;
                     scalarSizeInBytes = ossimGetScalarSizeInBytes(current);
                     theInputToPassThrough = idx;
                  }
               }
            }
         }
      }
   }
}

bool ossimImageCombiner::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool result = ossimImageSource::loadState(kwl, prefix);

   return result;
}

void ossimImageCombiner::updateRects()
{
	theComputeFullResBoundsFlag = true;
}

ossimRefPtr<ossimImageData> ossimImageCombiner::getNextTile(
   ossim_uint32 index,
   const ossimIpt& origin,
   ossim_uint32 resLevel)
{
   theCurrentIndex = (ossim_int32)index - 1;
   return getNextTile(origin, resLevel);
}

ossimRefPtr<ossimImageData> ossimImageCombiner::getNextTile(
   ossim_uint32 index,
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   theCurrentIndex = (ossim_int32)index-1;
   return getNextTile(tileRect, resLevel);
}

ossimRefPtr<ossimImageData> ossimImageCombiner::getNextTile(
   const ossimIpt& origin,
   ossim_uint32 resLevel)
{
   ossim_int32 w = getTileWidth();
   ossim_int32 h = getTileHeight();
   
   return getNextTile(ossimIrect(origin.x,
                                 origin.y,
                                 origin.x + w-1,
                                 origin.y + h-1),
                      resLevel);
}

ossimRefPtr<ossimImageData> ossimImageCombiner::getNextTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(theComputeFullResBoundsFlag)
   {
      precomputeBounds();
   }
   ossim_int32 size = getNumberOfInputs();

   
   if((theCurrentIndex+1) >= size) return NULL;
   double scale = 1.0/std::pow(2.0, (double)resLevel);
   ossimDpt scalar(scale, scale);

   ossimImageSourceInterface* temp = NULL;
   ossimRefPtr<ossimImageData> result;
   ossimDataObjectStatus status = OSSIM_NULL;
   do
   {
      ++theCurrentIndex;
      ossimIrect rect = theFullResBounds[theCurrentIndex];
      if(!rect.hasNans())
      {
         rect = rect * scalar;
         temp = PTR_CAST(ossimImageSourceInterface,
                         getInput(theCurrentIndex));
         if(rect.intersects(tileRect)&&temp)
         {
            
            result = temp->getTile(tileRect, resLevel);
            status = (result.valid() ?
                      result->getDataObjectStatus():OSSIM_NULL);
            if((status == OSSIM_NULL)||
               (status == OSSIM_EMPTY))
            {
               result = NULL;
            }
         }
         else
         {
            status = OSSIM_NULL;
            result = NULL;
         }
      }
      else
      {
         status = OSSIM_NULL;
         result = NULL;
      }

   }while(!result&&(theCurrentIndex<size));

   return result;
}


ossimRefPtr<ossimImageData> ossimImageCombiner::getNextNormTile(
   ossim_uint32 index,
   const ossimIpt& origin,
   ossim_uint32 resLevel)
{
   theCurrentIndex = (ossim_int32)index - 1;
   return getNextNormTile(origin, resLevel);
}

ossimRefPtr<ossimImageData> ossimImageCombiner::getNextNormTile(
   ossim_uint32 index,
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   theCurrentIndex = (ossim_int32)index-1;
   return getNextNormTile(tileRect, resLevel);
}

ossimRefPtr<ossimImageData> ossimImageCombiner::getNextNormTile(
   const ossimIpt& origin,
   ossim_uint32 resLevel)
{
   ossim_int32 w = getTileWidth();
   ossim_int32 h = getTileHeight();
   
   return getNextNormTile(ossimIrect(origin.x,
                                     origin.y,
                                     origin.x + w-1,
                                     origin.y + h-1),
                          resLevel);
}

ossimRefPtr<ossimImageData> ossimImageCombiner::getNextNormTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   ossim_int32 size = getNumberOfInputs();

   if(!theNormTile)
   {
      theNormTile = new ossimImageData(this,
                                       OSSIM_NORMALIZED_FLOAT);
      theNormTile->initialize();
   }
   if((theCurrentIndex+1) >= size) return NULL;

   ossimRefPtr<ossimImageData> result = getNextTile(tileRect, resLevel);

   if(result.valid())
   {
      theNormTile->setImageRectangleAndBands(result->getImageRectangle(),
                                             result->getNumberOfBands());
      result->copyTileToNormalizedBuffer((float*)theNormTile->getBuf());

      theNormTile->setDataObjectStatus(result->getDataObjectStatus());
      result = theNormTile;
   }
   
   return result;
}

ossim_uint32 ossimImageCombiner::getNumberOfOverlappingImages(const ossimIrect& rect,
                                                         ossim_uint32 resLevel)const
{
   if(theComputeFullResBoundsFlag)
   {
      precomputeBounds();
   }
   double scale = 1.0/std::pow(2.0, (double)resLevel);
   ossimDpt scalar(scale, scale);
   ossim_uint32 result = 0;
   ossim_uint32 maxIndex = getNumberOfInputs();
   for(ossim_uint32 inputIndex = 0; inputIndex < maxIndex; ++inputIndex)
   {
      if(!theFullResBounds[inputIndex].hasNans())
      {
         ossimIrect boundingRect = theFullResBounds[inputIndex] * scalar;
         if(rect.intersects(boundingRect))
         {
            ++result;
         }
      }
   }
   
   return result;
}

void ossimImageCombiner::getOverlappingImages(std::vector<ossim_uint32>& result,
					      const ossimIrect& rect,
                                              ossim_uint32 resLevel)const
{
   if(theComputeFullResBoundsFlag)
   {
      precomputeBounds();
   }
   double scale = 1.0/std::pow(2.0, (double)resLevel);
   ossimDpt scalar(scale, scale);
   
   ossim_uint32 inputIndex;
   ossimIrect boundingRect;
   for(inputIndex = 0; inputIndex < getNumberOfInputs(); ++inputIndex)
   {
      if(!theFullResBounds[inputIndex].hasNans())
      {
         boundingRect = theFullResBounds[inputIndex]*scalar;
         if(rect.intersects(boundingRect))
         {
            result.push_back(inputIndex);
         }
      }
   }
}

void ossimImageCombiner::connectInputEvent(ossimConnectionEvent& event)
{
   initialize();
}

void ossimImageCombiner::disconnectInputEvent(ossimConnectionEvent& event)
{ 
   initialize();
}

void ossimImageCombiner::propertyEvent(ossimPropertyEvent& event)
{
   initialize();
}

void ossimImageCombiner::refreshEvent(ossimRefreshEvent& event)
{
   initialize();
}

bool ossimImageCombiner::hasDifferentInputs()const
{
   return theHasDifferentInputs;
}

bool ossimImageCombiner::getImageGeometry(ossimKeywordlist& kwl,
                                          const char* prefix)
{
   if(getInput(0))
   {
      ossimImageSourceInterface* inter = PTR_CAST(ossimImageSourceInterface,
                                                  getInput(0));
      if(inter)
      {
         return inter->getImageGeometry(kwl, prefix);
      }
   }
   
   return false;
}

bool ossimImageCombiner::saveState(ossimKeywordlist& kwl,
                                   const char* prefix) const
{
   return ossimImageSource::saveState(kwl, prefix);
}

bool ossimImageCombiner::canConnectMyInputTo(ossim_int32 inputIndex,
                                             const ossimConnectableObject* object)const
{
   return (object&& PTR_CAST(ossimImageSourceInterface, object));
}

void ossimImageCombiner::precomputeBounds()const
{
   
   ossim_uint32 inputSize = getNumberOfInputs();

   if(inputSize)
   {
      ossimImageSourceInterface* tempInterface=0;
      if(theFullResBounds.size() != inputSize)
      {
         theFullResBounds.resize(inputSize);
      }
      for(ossim_uint32 inputIndex = 0; inputIndex < inputSize; ++inputIndex)
      {
         tempInterface = PTR_CAST(ossimImageSourceInterface, getInput(inputIndex));
         if(tempInterface)
         {
            theFullResBounds[inputIndex] = tempInterface->getBoundingRect();
         }
         else
         {
            theFullResBounds[inputIndex].makeNan();
         }
      }
      theComputeFullResBoundsFlag = false;
   }
   else
   {
      theFullResBounds.clear();
   }
}
