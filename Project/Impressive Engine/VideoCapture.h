/*/////////////////////////////////////////////////////////////////////////////////
///
///  Impressive Engine
///
/// Copyright (c) 2012-2013 Impressive Reality team
//
/// The license is
///
/// Permission is denied, to any person or company
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// Project leader : O.Shahbazi <sh_omid_m@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////*/
////-----------------------------------------------------------------------------
//// torque
//// copyright garagegames, llc 2011
////-----------------------------------------------------------------------------
//
//#ifndef _videocapture_h_
//#define _videocapture_h_
//
//#include "core/util/tsingleton.h"
//#include "core/util/tvector.h"
//#include "core/util/str.h"
//#include "gfx/gfxtexturehandle.h"
//#include "math/mpoint2.h"
//#include "platform/threads/threadsafedeque.h"
//
//
//class guicanvas;
//class videoframegrabber;
//class videoencoder;
//class gbitmap;
//
//
//typedef videoencoder* (*videoencoderfactoryfn)();
//
//
///// abstract frame grabber class
///// implementation and initalization depends on video device
//class videoframegrabber
//{
//   friend class videocapture;
//protected:
//   point2i mresolution; // the resolution used to capture the back buffer (scaling will be used)
//   vector<gbitmap*> mbitmaplist; //list of bitmaps created from backbuffer captures
//
//   /// sets the output frame resolution
//   void setoutresolution( const point2i& res ) { mresolution = res; }   
//
//   /// pushes a fresh bitmap into our list
//   void pushnewbitmap( gbitmap* bitmap ) { mbitmaplist.push_back(bitmap); }
//
//   /// returns one captured bitmap. returns null if there are no more bitmaps.
//   gbitmap* fetchbitmap();
//
//   /// texture event callback
//   void _ontextureevent(gfxtexcallbackcode code);
//   
//   /// captures the current backbuffer. if the last capture wasn't made into a bitmap, it will be overriden.   
//   virtual void capturebackbuffer() = 0;
//
//   /// starts converting the last backbuffer capture to a bitmap
//   /// depending on the videoframegrabber implementation, this may not produce a bitmap right away.
//   virtual void makebitmap() = 0;
//
//   /// releases internal textures
//   virtual void releasetextures() {};
//
//public:
//   videoframegrabber();
//   virtual ~videoframegrabber();
//};
//
//
///// video capture interface class
//class videocapture
//{   
//private:
//   struct encoderfactory {
//      const char* name;
//      videoencoderfactoryfn factory;      
//   };
//
//   /// list of encoder factory functions
//   static vector<encoderfactory> mencoderfactoryfnlist;
//
//   // the frame position of the latest backbuffer capture
//   f32 mcapturedframepos;
//
//   /// our current video encoder
//   videoencoder* mencoder;
//
//   /// our video frame grabber
//   videoframegrabber* mframegrabber;
//
//   /// the canvas we're recording from
//   guicanvas* mcanvas;
//
//   /// true if we're recording
//   bool misrecording;
//
//   /// time when we captured the previous frame
//   u32 mvideocapturestarttime;
//
//   /// frame to be captured next
//   f32 mnextframeposition;
//
//   /// the per-frame time (in milliseconds)
//   f32 mmsperframe;
//   
//   /// the framerate we'll use to record
//   f32 mframerate;
//
//   /// accumulated error when converting the per-frame-time to integer
//   /// this is used to dither the value and keep overall time advancing
//   /// correct
//   f32 mmsperframeerror;
//
//   /// name of the encoder we'll be using
//   string mencodername;
//
//   /// the video output resolution
//   point2i mresolution;
//
//   /// output filename
//   string mfilename;
//
//   /// tur if we're waiting for a canvas to bre created before capturing
//   bool mwaitingforcanvas;
//
//   /// vector with bitmaps to delete
//   vector< gbitmap* > mbitmapdeletelist;
//
//   /// initializes our encoder
//   bool initencoder( const char* name );   
//
//   /// deletes processed bitmaps
//   void deleteprocessedbitmaps();
//      
//public:
//   videocapture();
//      
//   /// start a video capture session
//   void begin( guicanvas* canvas );
//
//   /// captures a new frame
//   void capture();
//   
//   /// ends a video capture
//   void end();
//
//   /// sets the output filename
//   void setfilename( const char* filename ) { mfilename = filename; }
//
//   /// sets the encoder we'll use
//   void setencodername( const char* encoder ) { mencodername = encoder; }
//
//   /// sets the framerate
//   void setframerate( f32 fps ) { mframerate = fps; }
//
//   /// sets the video output resolution
//   void setresolution(const point2i& res) { mresolution = res; }
//
//   /// returns true if we're capturing
//   bool isrecording() { return misrecording; }
//
//   /// returns the number of milliseconds per frame
//   s32 getmsperframe();
//
//   /// sets the video farme grabber (cannot record without one).
//   void setframegrabber( videoframegrabber* grabber ) { mframegrabber = grabber; }
//
//   /// this will make the video capture begin capturing 
//   /// as soon as a guicanvas is created
//   void waitforcanvas() { mwaitingforcanvas = true; }
//   bool iswaitingforcanvas() { return mwaitingforcanvas; }
//
//   /// registers an encoder
//   static void registerencoder( const char* name, videoencoderfactoryfn factoryfn );    
//
//   // for managedsingleton.
//   static const char* getsingletonname() { return "videocapture"; }   
//};
//
//
//
///// abstract video encoder class
//class videoencoder
//{
//protected:
//   // video output file path
//   string mpath;
//
//   // video framerate
//   f32 mframerate;
//
//   // video resolution
//   point2i mresolution;
//
//   // list with bitmaps which are done encoding
//   threadsafedeque< gbitmap* > mprocessedbitmaps;
//public:
//   // stores an encoded bitmap to be dealt with later
//   void pushprocessedbitmap( gbitmap* bitmap );
//      
//public:
//   /// sets the file the encoder will write to
//   void setfile( const char* path );
//
//   /// sets the framerate (and fixes it if its invalid)
//   virtual void setframerate( f32* framerate ) { mframerate = *framerate; }
//
//   /// sets the output resolution (and fixes it if its invalid)
//   virtual void setresolution( point2i* resolution ) { mresolution = *resolution; }
//
//   /// begins accepting frames for encoding
//   virtual bool begin() = 0;
//
//   /// pushes a new frame into the video stream
//   virtual bool pushframe( gbitmap * bitmap ) = 0;
//
//   /// finishes the encoding and closes the video
//   virtual bool end() = 0;
//
//   /// returns an already encoded bitmap. video capture will get these and manage their deletion
//   gbitmap* getprocessedbitmap();
//};
//
///// returns the videocapture singleton.
//#define vidcap managedsingleton<videocapture>::instance()
//
////-----------------------------------------
///// video encoder registration macro
//#define register_video_encoder(classname, encodername)   \
//   videoencoder* encoderfactory##encodername() { return new classname(); } \
//   struct __videncreg##encodername { __videncreg##encodername() { videocapture::registerencoder( #encodername, &encoderfactory##encodername ); } }; \
//   static __videncreg##encodername _gencoderregistration;
//
//#endif // !_videocapture_h_
