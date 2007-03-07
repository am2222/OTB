//******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: This file contains the Application cache algorithm
//
//***********************************
// $Id: ossimAppFixedTileCache.h,v 1.6 2005/08/17 19:18:12 gpotts Exp $
#ifndef ossimAppFixedTileCache_HEADER
#define ossimAppFixedTileCache_HEADER
#include <map>
#include <list>
#include <iostream>
#include <base/common/ossimConstants.h>
#include <base/common/ossimRefPtr.h>
#include <base/data_types/ossimIrect.h>


class ossimFixedTileCache;
class ossimImageData;

class ossimAppFixedTileCache
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimAppFixedTileCache& rhs);
   static const ossim_uint32 DEFAULT_SIZE;
   typedef ossim_int32 ossimAppFixedCacheId;
   static ossimAppFixedTileCache *instance(ossim_uint32  maxSize   = 0);
   virtual ~ossimAppFixedTileCache();
   
   /*!
    * Will flush all cache registered
    */
   virtual void flush();
   virtual void flush(ossimAppFixedCacheId cacheId);
   virtual void deleteCache(ossimAppFixedCacheId cacheId);
   /*!
    * Will create a new Tile cache for this application if the tile size is 0,0 it will
    * use the default tile size.  Will
    * return 0 if not successful.
    */
   ossimAppFixedCacheId newTileCache(const ossimIrect& tileBoundaryRect,
                                     const ossimIpt& tileSize=ossimIpt(0,0));
   ossimAppFixedCacheId newTileCache();

   virtual void setRect(ossimAppFixedCacheId cacheId,
                        const ossimIrect& boundaryTileRect);
   
   ossimRefPtr<ossimImageData> getTile(ossimAppFixedCacheId cacheId,
                           const ossimIpt& origin);
   ossimRefPtr<ossimImageData> addTile(ossimAppFixedCacheId cacheId,
                                       ossimRefPtr<ossimImageData> data);
   
   ossimRefPtr<ossimImageData> removeTile(ossimAppFixedCacheId cacheId,
                                          const ossimIpt& origin);
   void deleteTile(ossimAppFixedCacheId cacheId,
                   const ossimIpt& origin);
   
   const ossimIpt& getTileSize(ossimAppFixedCacheId cacheId);
   
   virtual void setMaxCacheSize(ossim_uint32 cacheSize);
   
protected:
//    struct ossimAppFixedCacheTileInfo
//    {
//    public:
//       ossimAppFixedCacheTileInfo(ossimAppFixedCacheId appId,
//                                  const ossimIpt& origin)
//          :theAppCacheId(appId),
//           theOrigin(origin)
//          {}
//       ossimAppFixedCacheId  theAppCacheId;
//       ossimIpt       theOrigin;
      
//       bool operator ==(const ossimAppFixedCacheTileInfo &rhs)const
//          {
//             return (theAppCacheId == rhs.theAppCacheId &&
//                     theOrigin     == rhs.theOrigin);
//          } 
//    };
   
   ossimAppFixedTileCache();
   
   ossimFixedTileCache* getCache(ossimAppFixedCacheId cacheId);

   void shrinkGlobalCacheSize(ossim_int32 byteCount);
   void shrinkCacheSize(ossimAppFixedCacheId id,
                        ossim_int32 byteCount);
   void shrinkCacheSize(ossimFixedTileCache* cache,
                        ossim_int32 byteCount);
   void deleteAll();
   
   static ossimAppFixedTileCache *theInstance;
   
   /*!
    * Will hold the current unique Application id.
    */
   static ossimAppFixedCacheId    theUniqueAppIdCounter;
   ossimIpt                       theTileSize;
   ossim_uint32                   theMaxCacheSize;
   ossim_uint32                   theMaxGlobalCacheSize;
   ossim_uint32                   theCurrentCacheSize;

   std::map<ossimAppFixedCacheId, ossimFixedTileCache*> theAppCacheMap;

   /*!
    * Is used in an Least recently used algorithm
    */
//   std::list<ossimAppFixedCacheTileInfo>      theUsedQueue;
};

#endif
