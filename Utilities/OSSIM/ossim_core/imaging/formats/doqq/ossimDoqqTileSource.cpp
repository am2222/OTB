//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimDoqqTileSource.cpp,v 1.16 2005/11/08 18:25:52 gpotts Exp $
#include <imaging/formats/doqq/ossimDoqqTileSource.h>
#include <support_data/doqq/ossimDoqq.h>
#include <imaging/formats/general_raster/ossimGeneralRasterInfo.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/common/ossimKeywordNames.h>

RTTI_DEF1(ossimDoqqTileSource,
          "ossimDoqqTileSource",
          ossimGeneralRasterTileSource);

ossimDoqqTileSource::ossimDoqqTileSource()
   :theHeaderInformation(0)
{
}

ossimDoqqTileSource::~ossimDoqqTileSource()
{
   theHeaderInformation = 0;
}

bool ossimDoqqTileSource::open(const ossimFilename& filename)
{
   bool result = false;
   
   theHeaderInformation = new ossimDoqq(filename);
   if(theHeaderInformation->errorStatus() == ossimDoqq::OSSIM_ERROR)
   {
      theHeaderInformation = 0;
      
      return result;
   }
   else
   {
     // Set the Acquisition Date
     theAcqDate = theHeaderInformation->theAcqYear;
     theAcqDate += "-";
     theAcqDate += theHeaderInformation->theAcqMonth;
     theAcqDate += "-";
     theAcqDate += theHeaderInformation->theAcqDay;

     vector<ossimFilename> f;
     f.push_back(filename);
     ossimGeneralRasterInfo genRasterInfo(f,
					  OSSIM_UCHAR,
					  OSSIM_BIP,
					  theHeaderInformation->theRgb,
					  theHeaderInformation->theLine,
					  theHeaderInformation->theSample,
					  theHeaderInformation->theHeaderSize,
					  ossimGeneralRasterInfo::NONE,
					  0);
     
     result = ossimGeneralRasterTileSource::open(genRasterInfo);
   }

   return result;
}

bool ossimDoqqTileSource::getImageGeometry(ossimKeywordlist& kwl,
                                           const char* prefix)
{
   if (theGeometryKwl.getSize())
   {
      kwl = theGeometryKwl;
      return true;
   }
   
   if(theHeaderInformation.valid())
   {
      ossimString proj  = theHeaderInformation->theProjection.trim().upcase();
      ossimString datum = theHeaderInformation->theDatum.trim().upcase();

      if(proj == ossimString("UTM") || proj == ossimString("1"))
      {
         kwl.add(prefix,
                 ossimKeywordNames::TYPE_KW,
                 "ossimUtmProjection",
                 true);
         kwl.add(prefix,
                 ossimKeywordNames::ZONE_KW,
                 theHeaderInformation->theUtmZone,
                 true);
      }
      else
      {
         return false;
      }

      if(datum == ossimString("NAR") || datum == ossimString("4"))
      {
         kwl.add(prefix,
                 ossimKeywordNames::DATUM_KW,
                 "NAR-C", // North American Conus 1983
                 true);
      }
      else if(datum == ossimString("NAS") || datum == ossimString("1"))
      {
         kwl.add(prefix,
                 ossimKeywordNames::DATUM_KW,
                 "NAS-C", // North American Conus 1927
                 true);
      }
      else if(datum == ossimString("2"))
      {
         kwl.add(prefix,
                 ossimKeywordNames::DATUM_KW,
                 "WGD", // WGS 72
                 true);
      }
      else if(datum == ossimString("3"))
      {
         kwl.add(prefix,
                 ossimKeywordNames::DATUM_KW,
                 "WGE", // WGS 84
                 true);
      }
      else
      {
         return false;
      }

      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_X_KW,
              theHeaderInformation->theGsd.x,
              true);

      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
              theHeaderInformation->theGsd.y,
              true);
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_EASTING_KW,
//            theHeaderInformation->theUE + (theHeaderInformation->theGsd/2.0),
	      theHeaderInformation->theEasting,
              true);

      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_NORTHING_KW,
//            theHeaderInformation->theUN - (theHeaderInformation->theGsd/2.0),
	      theHeaderInformation->theNorthing,
              true);

      // Capture this for next time.
      setImageGeometry(kwl);
      
      return true;
   }
   return false;
}

bool ossimDoqqTileSource::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   if(!ossimImageHandler::loadState(kwl, prefix))
   {
      return false;
   }

   if(lookup)
   {
      theHeaderInformation = new ossimDoqq(ossimFilename(lookup));
      if(theHeaderInformation->errorStatus() != ossimDoqq::OSSIM_OK)
      {
         theHeaderInformation = 0;
         return false;
      }
      else
      {
         ossimFilename filename(lookup);
         vector<ossimFilename> f;
         f.push_back(filename);
         ossimGeneralRasterInfo genRasterInfo(f,
                                              OSSIM_UCHAR,
                                              OSSIM_BIP,
                                              theHeaderInformation->theRgb,
                                              theHeaderInformation->theLine,
                                              theHeaderInformation->theSample,
                                              theHeaderInformation->theHeaderSize,
                                              ossimGeneralRasterInfo::NONE,
                                              0);
         return open(genRasterInfo);
      }
   }
   else
   {
      return false;
   }
   
   return true;
}

ossimString ossimDoqqTileSource::getShortName()const
{
   return ossimString("doqq");
}

ossimString ossimDoqqTileSource::getLongName()const
{
   return ossimString("doqq handler");
}

bool ossimDoqqTileSource::open(const ossimGeneralRasterInfo& info)
{
   return ossimGeneralRasterTileSource::open(info);
}

ossimString ossimDoqqTileSource::acqdate() const
{
   return theAcqDate;
}
