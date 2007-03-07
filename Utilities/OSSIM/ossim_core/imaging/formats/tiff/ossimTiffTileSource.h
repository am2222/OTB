//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//          Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimTiffTileSource.
// ossimTiffTileSource  is derived from ImageHandler which is derived from
// TileSource.
//*******************************************************************
//  $Id: ossimTiffTileSource.h,v 1.37 2005/11/08 21:50:21 dburken Exp $

#ifndef ossimTiffTileSource_HEADER
#define ossimTiffTileSource_HEADER

#include <tiffio.h>

#include <imaging/formats/ossimImageHandler.h>
#include <base/data_types/ossimIrect.h>

class ossimImageData;
class ossimTiffOverviewTileSource;

class OSSIMDLLEXPORT ossimTiffTileSource : public ossimImageHandler
{
public:

   enum ReadMethod
   {
      UNKNOWN,
      READ_RGBA_U8_TILE,
      READ_RGBA_U8_STRIP,
      READ_RGBA_U8A_STRIP,
      READ_SCAN_LINE,
      READ_TILE
   };

   ossimTiffTileSource();
	
   virtual ~ossimTiffTileSource();

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;

   /*!
    *  Returns true if the image_file can be opened and is a valid tiff file.
    */
  virtual bool open(const ossimFilename& image_file);
  virtual void close();

   /*!
    *  Returns a pointer to a tile given an origin representing the upper left
    *  corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
   
   /*!
     *  Returns the number of bands in the image.
     *  Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfInputBands() const;
   virtual ossim_uint32 getNumberOfOutputBands () const;
   
   /*!
     *  Returns the number of lines in the image.
     *  Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 reduced_res_level = 0) const;

   /*!
    *  Returns the number of samples in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 reduced_res_level = 0) const;

   /*!
    *  Returns the number of image file directories in the tiff image.
    */
   virtual ossim_uint32 getNumberOfDirectories() const;
   
   /*!
    * Returns the number of reduced resolution data sets (rrds).
    * Notes:
    *
    * - The full res image is counted as a data set so an image with no
    *   reduced resolution data set will have a count of one.
    * - This method counts R0 as a res set even if it does not have one.
    *   This was done deliberately so as to not screw up code down the
    *   line.
    */
   virtual ossim_uint32 getNumberOfDecimationLevels() const;
   
   /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool isOpen()const;

   /*!
    * Returns the output pixel type of the tile source.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /*!
    * Returns the width of the output tile.
    */
   virtual ossim_uint32 getTileWidth() const;
   
   /*!
    * Returns the height of the output tile.
    */
   virtual ossim_uint32 getTileHeight() const;

   /*!
    *  Returns true if the first directory of the tiff image did not have
    *  the reduced resolution file type set.
    */
   bool hasR0() const;
  
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   
   virtual bool isValidRLevel(ossim_uint32 reduced_res_level) const;
   
   /*!
    * Returns the highest available reduced resolution data sets (rrds).
    * Note:  zero based
    */
   ossim_uint32 getHighestReducedResSet() const;

   /*!
    * @return The tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileWidth which
    * returns the output tile width, which can be different than the
    * internal image tile width on disk.
    */
   virtual ossim_uint32 getImageTileWidth() const;

   /*!
    * @return The tile width of the image or 0 if the image is not tiled.
    * Note: this is not the same as the ossimImageSource::getTileHeight which
    * returns the output tile width which can be different than the internal
    * image tile width on disk.
    */
   virtual ossim_uint32 getImageTileHeight() const;

   void setApplyColorPaletteFlag(bool flag);
   bool getApplyColorPaletteFlag()const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   bool isColorMapped() const;
   
   virtual std::ostream& print(std::ostream& os) const;

protected:
   /*!
    *  Returns true if no errors initializing object.
    *
    *  Notes:
    *  - Callers of this method must ensure "theTiffPtr" data member
    *    is initialized.
    *  - This method was added to consolidate object initialization code
    *    between constructor and public open method.
    */
   bool open();
   
private:

   /*!
    *  Adjust point to even tile boundary.  Assumes 0,0 origin.
    *  Shifts in the upper left direction.
    */
   void adjustToStartOfTile(ossimIpt& pt) const;

   /*!
    *  If the tiff source has R0 then this returns the current tiff directory
    *  that the tiff pointer is pointing to; else, it returns the current
    *  directory + 1.
    */
   ossim_uint32 getCurrentTiffRLevel() const;
   
   ossimString getReadMethod(ossim_uint32 directory) const;

   bool allocateBuffer();
   
   bool loadTile(const ossimIrect& tile_rect,
                 const ossimIrect& clip_rect);
   
   bool loadFromRgbaU8Tile(const ossimIrect& tile_rect,
                           const ossimIrect& clip_rect);
   
   bool loadFromRgbaU8Strip(const ossimIrect& tile_rect,
                            const ossimIrect& clip_rect);

   bool loadFromRgbaU8aStrip(const ossimIrect& tile_rect,
                             const ossimIrect& clip_rect);
   
   bool loadFromScanLine(const ossimIrect& clip_rect);

   bool loadFromTile(const ossimIrect& clip_rect);

   void setReadMethod();
   virtual void initializeBuffers();

   /**
    * Change tiff directory and sets theCurrentDirectory.
    *
    * @return true on success, false on error.
    */
   bool setTiffDirectory(ossim_uint32 directory);

   void populateLut();
   
   ossimRefPtr<ossimImageData> theTile;
   
   ossim_uint8*         theBuffer;
   ossim_uint32         theBufferSize;
   ossimIrect           theBufferRect;
   ossim_uint32         theBufferRLevel;
   TIFF*                theTiffPtr;

   ossim_uint16         theSamplesPerPixel;
   ossim_uint16         theBitsPerSample;
   ossim_uint16         theSampleFormatUnit;
   double               theMaxSampleValue;
   double               theMinSampleValue;
   ossim_uint32         theNumberOfDirectories;
   ossim_uint32         theCurrentDirectory;
   bool                 theR0isFullRes;
   ossim_uint32         theBytesPerPixel;
   ossimScalarType      theScalarType;
   bool                 theApplyColorPaletteFlag;

   // Things we need from each directory as they can be different.
   vector<ossim_uint32> theImageWidth;
   vector<ossim_uint32> theImageLength;
   vector<ReadMethod>   theReadMethod;
   vector<ossim_uint16> thePlanarConfig;
   vector<ossim_uint16> thePhotometric;
   vector<ossim_uint32> theRowsPerStrip;
   vector<ossim_uint32> theImageTileWidth;
   vector<ossim_uint32> theImageTileLength;
   

TYPE_DATA
};

#endif
