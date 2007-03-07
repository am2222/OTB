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
// $Id: ossimStdOutProgress.cpp,v 1.5 2005/10/17 18:37:16 gpotts Exp $
#include "ossimStdOutProgress.h"

RTTI_DEF1(ossimStdOutProgress, "ossimStdOutProgress", ossimProcessListener);

ossimStdOutProgress theStdOutProgress;

ossimStdOutProgress::ossimStdOutProgress(long precision, bool flushStream)
   :
      ossimProcessListener(),
      thePrecision(precision),
      theFlushStreamFlag(flushStream)
{
}

void ossimStdOutProgress::processProgressEvent(ossimProcessProgressEvent& event)
{
   if (event.getOutputMessageFlag())
   {
      ossimString s;
      event.getMessage(s);
      if (!s.empty())
      {
		  ossimNotify(ossimNotifyLevel_NOTICE) << s.c_str() << std::endl;
      }
      return; // Don't output percentage on a message update.
   }

   
   double p = event.getPercentComplete();
   ossimNotify(ossimNotifyLevel_NOTICE)
	   << std::setiosflags(std::ios::fixed)
      << std::setprecision(thePrecision)
      << p << "%\r";
   
   if(theFlushStreamFlag)
   {
      (p != 100.0) ?
         ossimNotify(ossimNotifyLevel_NOTICE).flush() :
         ossimNotify(ossimNotifyLevel_NOTICE) << "\n";
   }
}

void ossimStdOutProgress::setFlushStreamFlag(bool flag)
{
   theFlushStreamFlag = flag;
}

