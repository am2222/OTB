//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// ossimCacheTileSource.
//*******************************************************************
//  $Id: ossimCacheTileSource.h,v 1.27 2005/02/11 15:07:32 dburken Exp $
#ifndef ossimCacheTileSource_HEADER
#define ossimCacheTileSource_HEADER
#include <imaging/tile_sources/ossimImageSourceFilter.h>
#include <imaging/cache/ossimAppFixedTileCache.h>
#include <base/common/events/ossimProcessProgressEvent.h>

/** Cache Tile Source */
class OSSIMDLLEXPORT ossimCacheTileSource : public ossimImageSourceFilter
{
public:
   /**
    * Will construct a new Application cache
    */
   ossimCacheTileSource();
   ossimCacheTileSource(ossimImageSource* inputSource);
   virtual ~ossimCacheTileSource();

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   virtual void initialize();
   virtual void flush();
   virtual void setCachingEnabledFlag(bool value);
   virtual void setEventProgressFlag(bool value);

   void     changeTileSize(const ossimIpt& size);
   ossimIpt getTileSize();
   void     setTileSize(const ossimIpt& size);
   
   virtual ossim_uint32 getTileWidth() const;
   virtual ossim_uint32 getTileHeight() const;
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix=0);
   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0)const;

protected:

   void allocate();
   
   ossimAppFixedTileCache::ossimAppFixedCacheId theCacheId;

   ossimRefPtr<ossimImageData> theTile;
   ossimIpt                    theFixedTileSize;
   bool                        theCachingEnabled;
   bool                        theEventProgressFlag;
   ossim_uint32                theCacheRLevel;
   ossimIrect                  theBoundingRect;
   
   virtual void fireProgressEvent(double percentComplete);
   
   virtual ossimRefPtr<ossimImageData> fillTile(ossim_uint32 resLevel);

TYPE_DATA
};


#endif /* end of "#ifndef ossimCacheTileSource_HEADER" */
