#include "com_cabatuan_yuv2rgb_MainActivity.h"
#include <android/log.h>
#include <android/bitmap.h>
#include <stdlib.h>

#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

#define  LOG_TAG    "YUV2RGB"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)




/// 17 fps
void NV21toYUV( const cv::Mat &src, cv::Mat &rgba ){

 int height = 2 * src.rows / 3;
 int uvRowIndex;
 uchar colorV, colorU; 

 for (int j = 0, uvRowIndex = 0; j < height - 1; j+=2, uvRowIndex++) { // for all rows   
      
      const uchar* current = src.ptr<const uchar>(j);   // current NV21 row
      const uchar* next    = src.ptr<const uchar>(j+1); // next NV21 row
       
      const uchar* uv_row  = src.ptr<const uchar>(height + uvRowIndex); // uv row
      
      cv::Vec4b *rgba_row   = rgba.ptr<cv::Vec4b>(j);     // current RGBA row 
      cv::Vec4b *next_rgba  = rgba.ptr<cv::Vec4b>(j+1);   // next RGBA row
    
  
      for (int i = 0; i < src.cols - 1; i+=2) {
      
          // Get V & U
             colorV = uv_row[i];
             colorU = uv_row[i+1];
          
             //LOGI("colorV = %d, colorU = %d", colorV, colorU);
             
             //This is the problem:
            
          // Assign to a 2 x 2 block
             rgba_row[i]    = cv::Vec4b( current[i], colorU, colorV, 255);
             rgba_row[i+1]  = cv::Vec4b( current[i+1], colorU, colorV, 255);
             next_rgba[i]   = cv::Vec4b( next[i], colorU, colorV, 255);
		     next_rgba[i+1] = cv::Vec4b( next[i+1], colorU, colorV, 255);
		     
		      
       }
       
      
 }
   
   /// Sanity check.. OKAY
 
}





 


/*
 * Class:     com_cabatuan_yuv2rgb_MainActivity
 * Method:    process
 * Signature: (Landroid/graphics/Bitmap;[B)V
 */
JNIEXPORT void JNICALL Java_com_cabatuan_yuv2rgb_MainActivity_process
  (JNIEnv *pEnv, jobject clazz, jobject pTarget, jbyteArray pSource){

   AndroidBitmapInfo bitmapInfo;
   uint32_t* bitmapContent; // Links to Bitmap content

   if(AndroidBitmap_getInfo(pEnv, pTarget, &bitmapInfo) < 0) abort();
   if(bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) abort();
   if(AndroidBitmap_lockPixels(pEnv, pTarget, (void**)&bitmapContent) < 0) abort();

   /// Access source array data... OK
   jbyte* source = (jbyte*)pEnv->GetPrimitiveArrayCritical(pSource, 0);
   if (source == NULL) abort();

   Mat src(bitmapInfo.height + bitmapInfo.height/2, bitmapInfo.width, CV_8UC1, (unsigned char *)source);
   Mat mbgra(bitmapInfo.height, bitmapInfo.width, CV_8UC4, (unsigned char *)bitmapContent);

/***********************************************************************************************/

    NV21toYUV(src, mbgra);
 
    /// Sanity check
    //cvtColor(src.rowRange(0, bitmapInfo.height)-16, mbgra, CV_GRAY2BGRA);

/************************************************************************************************/ 
   
   /// Release Java byte buffer and unlock backing bitmap
   pEnv-> ReleasePrimitiveArrayCritical(pSource,source,0);
   if (AndroidBitmap_unlockPixels(pEnv, pTarget) < 0) abort();

}
