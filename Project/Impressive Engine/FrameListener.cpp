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

#include "FrameListener.h"
#include "Engine.h"
#include "ScenesHolder.h"

#include "ViewPortsManager.h"
#include "RenderControl.h"
//#include "AVIRecorder.h"
//#include "StdAfxTheora.h"

#ifdef DEBUG
#include "HID.h"
#endif


BEGIN_NAMESPACE
FrameListener *Singleton<FrameListener>::m_Instance = NULL;
//th_info encoderInfo;

//FrameListener::FrameListener(void)
//{
//}


//FrameListener::~FrameListener(void)
//{
//}


void FrameListener::Initialize(void)
{
#ifdef DEBUG
	Ogre::Profiler &profiler = Ogre::Profiler::getSingleton();
	profiler.setTimer(Engine::GetInstance().GetGraphic()->getTimer());
	profiler.setDisplayMode(Ogre::Profiler::DISPLAY_MILLISECONDS);
	profiler.setUpdateDisplayFrequency(6);

	Engine::GetInstance().GetGraphic()->addFrameListener(this);
#endif

#ifdef EDIT_MODE
	//m_Avi = NULL;
	recording = false;
#endif

	//ScenesHolder::GetInstance().GetCurrentScene()->GetSceneManager()->addRenderQueueListener(this);
}

#ifdef EDIT_MODE
void FrameListener::initilizeRecorder(void)
{
	if (!recording)
	{
		ViewPortsManager::GetInstance().GetEditRenderControl()->GetCamera()->_GetCamera()->addListener(this);

		m_RT = Ogre::TextureManager::getSingleton().createManual("RecordRT", 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 1104, 524, 0 /* 0 mipmaps */, 
			Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET, 0, false, 0 /*rootData->window->getFSAA()*/);

		Ogre::RenderTarget *target = m_RT->getBuffer()->getRenderTarget();
		Ogre::Viewport *view = target->addViewport(ViewPortsManager::GetInstance().GetEditRenderControl()->GetCamera()->_GetCamera());

		//m_Avi = new CAVIGenerator();


		//// set 15fps
		//m_Avi->SetRate(24);
	
		//// give info about bitmap
		//////m_Avi->SetBitmapHeader(lpbih);
		//SetBitmapHeader();

		//// set filename, extension ".avi" is appended if necessary
		//m_Avi->SetFileName((Engine::PATH_DATA + "a.avi").c_str());



		//res = m_Avi->InitEngine();


		//m_RT->getBuffer()->getRenderTarget()->update();
		//Ogre::Image img;
		//m_RT->convertToImage(img);
		//img.save("C:\\1.jpg");

		
		//m_Avi->AddFrame((BYTE*)img.getData());


		///*
  //      ####################################################################################
  //      ##
  //      ##  fill ycbcr with bitmap
  //      ##
  //      ####################################################################################
  //      */


		//th_img_plane ycbcr[3];

  //          ycbcr[0].width  =   640;
  //          ycbcr[0].height =   480;
  //          ycbcr[0].stride =   640;
  //          //ycbcr[0].data   =   (unsigned char*)Y_luma;
  //          ycbcr[1].width  =   320;
  //          ycbcr[1].height =   240;
  //          ycbcr[1].stride =   320;
  //          //ycbcr[1].data   =   (unsigned char*)Cb_chroma;
  //          ycbcr[2].width  =   320;
  //          ycbcr[2].height =   240;
  //          ycbcr[2].stride =   320;
  //          //ycbcr[2].data   =   (unsigned char*)Cr_chroma;
  //      /*
  //      ####################################################################################
  //      ##
  //      ##  Call th_info_init() to initialise a th_info structure, then set up you output 
  //      ##  parameters by assigning the appropriate members in that
  //      ##
  //      ####################################################################################
  //      */
  //          th_info_init(&encoderInfo);
  //          encoderInfo.frame_width  =   1048;
  //          encoderInfo.frame_height =   544;
  //          encoderInfo.pic_width    =   1048;
  //          encoderInfo.pic_height   =   544;
  //          encoderInfo.pic_x        =   0;
  //          encoderInfo.pic_y        =   0;
  //          encoderInfo.fps_numerator            =   1;
  //          encoderInfo.fps_denominator          =   1;
  //          encoderInfo.aspect_numerator         =   1;
  //          encoderInfo.aspect_denominator       =   1;
  //          encoderInfo.colorspace               =   TH_CS_UNSPECIFIED;
  //          encoderInfo.target_bitrate           =   500;
  //          encoderInfo.quality                  =   50;
  //          encoderInfo.keyframe_granule_shift   =   0;
  //          encoderInfo.pixel_fmt                =   TH_PF_420;
  //      /*
  //      ####################################################################################
  //      ##
  //      ##  Use that structure in a call to th_encode_alloc() to get an encoder context
  //      ##
  //      ####################################################################################
  //      */
		//th_enc_ctx *td = th_encode_alloc(&encoderInfo);
  //      if(td==NULL)
  //          {
  //              printf("-E- negative return code initializing encoder!");
  //              th_info_clear(&encoderInfo);
  //              return;
  //          }
  //      /*
  //      ####################################################################################
  //      ##
  //      ##  Initialise a blank th_comment structure using th_comment_init
  //      ##
  //      ####################################################################################
  //      */
		//th_comment tc;
  //      th_comment_init(&tc);
  //      /*
  //      ####################################################################################
  //      ##
  //      ##  Initialise an ogg stream, with ogg_stream_init()
  //      ##
  //      ####################################################################################
  //      */
  //      //ogg_stream_init(&to,rand()); 
  //      //    if (th_encode_flushheader(td,&tc,&op)<=0)
  //      //    {
  //      //        printf("-E- Internal Theora Library Error!");
  //      //        return false;
  //          //}

  //          /*
  //      ####################################################################################
  //      ##
  //      ##  Call th_encode_flushheader with the the encoder context, the blank comment 
  //      ##  structure and an ogg_packet.
  //      ##
  //      ####################################################################################
  //      */
  //      //if (th_encode_flushheader(td, &tc,&op)<=0)
  //      //    {
  //      //        printf("-E- Internal Theora Library Error!");
  //      //        return false;
  //      //    }
  //      /* Theora is a one-frame-in,one-frame-out system; submit a frame
  //           for compression and pull out the packet */
  //        if(th_encode_ycbcr_in(td, ycbcr)) {
  //          fprintf(stderr, "-E- Error: could not encode frame\n");
  //          return;
  //        }
  //      //if(!th_encode_packetout(td, 1, &op)) {
  //      //    fprintf(stderr, "-E- [theora_write_frame] Error: could not read packets\n");
  //      //    return;
  //      //  }
  //      /*
  //      ####################################################################################
  //      ##
  //      ##  Send the resulting packet to the ogg stream with ogg_stream_acketin()
  //      ##
  //      ####################################################################################
  //      */
  //      //ogg_page og;
  //      //    ogg_stream_acketin(&to,&op);
  //      //    if (ogg_stream_ageout(&to,&og)!=1)
  //      //    {
  //      //        printf("-E- Internal Ogg library Error!");
  //      //        return false;
  //      //    }
  //      /*
  //      ####################################################################################
  //      ##
  //      ##  Until th_encode_flushheader returns 0 (or an error code)
  //      ##  Now, repeatedly call ogg_stream_ageout(), every time writing the page.header and then page.body 
  //      ##  to an output file, until it returns 0. 
  //      ##  Now call ogg_stream_flush and write the resulting page to the file.
  //      ####################################################################################
  //      */
  //      //int ret;
  //      /*for(;;)
  //          {
  //              ret=th_encode_flushheader(td,&tc,&op);
  //              if(ret<0)
  //              {
  //                  printf("-E- Internal Theora library error.");
  //                  if (td!=NULL)
  //                  {
  //                      th_encode_free(td);
  //                      td=NULL;
  //                  }
  //                  th_comment_clear(&tc);
  //                  th_info_clear(&ti);
  //                  return false;
  //              }
  //              else if(!ret)
  //              {   fwrite (og.header, 1 , sizeof(og.header) , fout );
  //                  fwrite (og.body, 1 , sizeof(og.body) , fout );
  //                  break;
  //              }
  //              ogg_stream_acketin(&to,&op);
  //          }
  //      int result;
  //      for(;;){
  //              int result = ogg_stream_flush(&to,&og);
  //              if(result<0){
  //                  printf("-E- Internal Ogg library error.");
  //                  return false;
  //              }

  //              if(result==0)break;
  //              fwrite(og.body,1,sizeof(og.body),fout);
  //          }*/










	}
	
}


void FrameListener::cameraPostRenderScene(Ogre::Camera *Camera)
{
	if (recording)
	{
		size_t numMips =  1;
		//size_t dataSize = Ogre::Image::calculateSize(numMips,
		//	m_RT->getNumFaces(), m_RT->getWidth(), m_RT->getHeight(), m_RT->getDepth(), m_RT->getFormat());
		size_t dataSize = Ogre::Image::calculateSize(numMips, m_RT->getNumFaces(), m_RT->getWidth(), m_RT->getHeight(), m_RT->getDepth(), m_RT->getFormat());

		void* pixData = OGRE_MALLOC(dataSize, Ogre::MEMCATEGORY_GENERAL);
		// if there are multiple faces and mipmaps we must pack them into the data
		// faces, then mips
		void* currentPixData = pixData;

		Ogre::PixelBox pixBox(m_RT->getWidth(), m_RT->getHeight(), m_RT->getDepth(), m_RT->getFormat(), currentPixData);
		m_RT->getBuffer(0, 0)->blitToMemory(pixBox);

		//currentPixData = (void*)((char*)currentPixData + mipDataSize);


		//m_RT->convertToImage(img);
		
		//m_Avi->AddFrame((unsigned char*)currentPixData, dataSize);
	}
}
#endif

bool FrameListener::frameStarted(const Ogre::FrameEvent &evt)
{
#ifdef DEBUG
	Ogre::Profiler &profiler = Ogre::Profiler::getSingleton();
	if (profiler.getEnabled())
	{
		profiler.setOverlayDimensions(250, 150);
		profiler.setOverlayPosition(250, 10);

		OgreProfileBegin("Engine Process");
	}
#endif

	return true;
}


//bool EditorListener::frameRenderingQueued(const Ogre::FrameEvent &evt)
//{
//	return true;
//}


bool FrameListener::frameEnded(const Ogre::FrameEvent &evt)
{
#ifdef EDIT_MODE
//	if (recording)
//	{
//		m_RT->getBuffer()->getRenderTarget()->update();
//		Ogre::Image img;
//		m_RT->convertToImage(img);
//		img.save("C:\\1.jpg");
//		
//		unsigned char * bytes;
//		//posix_memalign((void **)&bytes, 16, 1920 * 540 * 4);
//
//		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = m_RT->getBuffer(10, 100);
//
//		pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
//
//		const Ogre::PixelBox *pixelBox=new Ogre::PixelBox(1920,540,1,Ogre::PF_A8R8G8B8,bytes);
//
//		pixelBuffer->blitToMemory(*pixelBox);
//
//		size_t srcPitch = pixelBox->rowPitch;
//		size_t srcHeight = pixelBox->getHeight() ;
//		size_t dstBpp = Ogre::PixelUtil::getNumElemBytes(pixelBox->format);
//		srcPitch*= dstBpp;
//		Ogre::uint8 *srcData = static_cast<Ogre::uint8*>(pixelBox->data);
//
//
//
//
//		LPBITMAPINFOHEADER lpbih = m_Avi->GetBitmapHeader();
//		BYTE* bmBits = new BYTE[lpbih->biSizeImage];
//
//		Ogre::PixelBox *b = new Ogre::PixelBox(20, 20, 1, Ogre::PF_R8G8B8, bmBits);
//
//		ViewPortsManager::GetInstance().GetEditRenderControl()->GetRenderWindow()->swapBuffers();
//		ViewPortsManager::GetInstance().GetEditRenderControl()->GetRenderWindow()->copyContentsToMemory(*b);
//	
//		m_Avi->AddFrame(img.getData());
//	}
//
#endif
	
#ifdef DEBUG
	if (Ogre::Profiler::getSingleton().getEnabled())
	{
		OgreProfileEnd("Engine Main");
	}
#endif

	return true;
}


//void FrameListener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String &invocation, bool &skipThisInvocation)
//{
	//skipThisInvocation = false;

	//if (queueGroupId == Ogre::RENDER_QUEUE_9)
	//{
	//	Engine::GetInstance().GetGraphic()->getRenderSystem()->clearFrameBuffer(Ogre::FBT_DEPTH);
	//}
//}

#ifdef EDIT_MODE
void FrameListener::SetBitmapHeader(void)
{
	//CRect rect; 
	//pView->GetClientRect(&rect); 
	//CSize size(rect.Width(),rect.Height()); 

	///////////////////////////////////////////////////
	//// changing size of image so dimension are multiple of 4
	//size.cx=(size.cx/4)*4;
	//size.cy=(size.cy/4)*4;

	//// initialize m_bih
	//memset(&m_bih,0, sizeof(BITMAPINFOHEADER));
	//// filling bitmap info structure.
	//m_bih.biSize=sizeof(BITMAPINFOHEADER);
	//m_bih.biWidth=size.cx;
	//m_bih.biHeight=size.cy;
	//m_bih.biPlanes=1;
	//m_bih.biBitCount=24;
	//m_bih.biSizeImage=((m_bih.biWidth*m_bih.biBitCount+31)/32 * 4)*m_bih.biHeight; 
	//m_bih.biCompression=BI_RGB;		//BI_RGB means BRG in reality

	BITMAPINFOHEADER m_bih;
	wxSize size = ViewPortsManager::GetInstance().GetEditRenderControl()->GetSize();

	// changing size of image so dimension are multiple of 4
	size.x = (size.x / 4) * 4;
	size.y = (size.y / 4) * 4;

	// initialize m_bih
	memset(&m_bih, 0, sizeof(BITMAPINFOHEADER));
	// filling bitmap info structure.
	m_bih.biSize = sizeof(BITMAPINFOHEADER);
	m_bih.biWidth = size.x;
	m_bih.biHeight = size.y;
	m_bih.biPlanes = 1;
	m_bih.biBitCount = 24;
	m_bih.biSizeImage = ((m_bih.biWidth*m_bih.biBitCount + 31 ) / 32 * 4) * m_bih.biHeight; 
	m_bih.biCompression = BI_RGB;		//BI_RGB means BRG in reality

	//m_Avi->SetBitmapHeader(&m_bih);
}
END_NAMESPACE
#endif