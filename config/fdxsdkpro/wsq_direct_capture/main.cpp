/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : SGFPLibTest main.cpp source code module
 * Copyright(c): 2009 SecuGen Corporation, All rights reserved
 * History :
 * date        person   comments
 * ======================================================
 * 11/4/2009   driley   Initial release
 *
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgfplib.h"
#include <sys/stat.h>
#include <chrono>
#include <thread>

LPSGFPM  sgfplib = NULL;

int RawToWSQTest() 
{
  int ret;
  DWORD width=258;
  DWORD height=336;
  DWORD depth = 8;
  DWORD ppi = 500;
  float r_bitrate = WSQ_BITRATE_5_TO_1; 
  BYTE* WsqRec = NULL;
  BYTE* ImageRec = NULL;
  DWORD WsqRecLen = 0;
  DWORD err;
  FILE *fp = NULL;
  struct stat file;
  char szFileName[100];

  printf("\n-------------------------------------\n");
  printf(  "SecuGen Device Capture to WSQ\n");
  printf(  "-------------------------------------\n");

  sprintf(szFileName,"raw2wsq_finger.raw");

  //Open the RAW file
  printf("Open file %s\n",szFileName);
  fp = fopen(szFileName,"rb");
  if (fp == NULL)
  {
    printf("ERROR: Cannot open file %s\n",szFileName);
    return (-1);
  }

  //Get the length of the file       
  stat(szFileName,&file);

  if (file.st_size <= 0)
  {
    printf("ERROR: %s file length is %ld\n",szFileName,file.st_size);
    return(-1);
  }
  else
    printf("File %s is %ld bytes long\n",szFileName,file.st_size);

  //Read the Image Record
  ImageRec = (BYTE*) malloc(file.st_size);
  fread(ImageRec,1,file.st_size,fp);

  //Close the image file
  fclose(fp);

  printf("Encode WSQ File\n");
  printf("Width:    %ld\n",width);
  printf("Height:   %ld\n",height);
  printf("Depth:    %ld\n",depth);
  printf("PPI:      %ld\n",ppi);
  printf("Bit Rate: %f\n",r_bitrate);
  //	virtual DWORD WINAPI  EncodeWSQ(BYTE ** wsqImageOut, DWORD *wsqImageOutSize, float wsqBitRate,  BYTE * rawImage, DWORD width, DWORD height, DWORD pixelDepth, DWORD ppi, char *commentText) = 0;

  if (ret = sgfplib->EncodeWSQ(&WsqRec, &WsqRecLen, r_bitrate,
                  ImageRec, width, height, depth, ppi, (char*)"SecuGen Comment"))
  {
     free(ImageRec);
     exit(ret);
  }

  printf("EncodeWSQ() ret: %d \n",ret);
  printf("WSQ Image size is: %ld \n",WsqRecLen);

  sprintf(szFileName,"raw2wsq_finger.wsq");
  printf("Write file %s\n",szFileName);
  fp = fopen(szFileName,"wb");
  fwrite(WsqRec,1,WsqRecLen,fp);
  fclose(fp);

  sgfplib->FreeWSQ(WsqRec);

  free(ImageRec);

}

int WSQToRawTest() 
{
  int ret;
  DWORD width;
  DWORD height;
  DWORD depth;
  DWORD ppi;
  DWORD lossyFlag;
  BYTE* WsqRec = NULL;
  BYTE* ImageRec = NULL;
  DWORD WsqRecLen;
  DWORD err;
  FILE *fp = NULL;
  struct stat file;
  char szFileName[100];

  printf("\n-------------------------------------\n");
  printf(  "SecuGen WSQ2RAW Test\n");
  printf(  "-------------------------------------\n");

  sprintf(szFileName,"wsq2raw_finger.wsq");

  //Open the WSQ file
  printf("Open file %s\n",szFileName);
  fp = fopen(szFileName,"rb");
  if (fp == NULL)
  {
    printf("ERROR: Cannot open file %s\n",szFileName);
    return (-1);
  }

  //Get the length of the file       
  stat(szFileName,&file);

  if (file.st_size <= 0)
  {
    printf("ERROR: %s file length is %ld\n",szFileName,file.st_size);
    return(-1);
  }
  else
    printf("File %s is %ld bytes long\n",szFileName,file.st_size);

  //Read the Image Record
  WsqRec = (BYTE*) malloc(file.st_size);
  fread(WsqRec,1,file.st_size,fp);
  WsqRecLen = file.st_size;


  //Close the RAW file
  fclose(fp);

  printf("Decode WSQ File\n");
  if (ret = sgfplib->DecodeWSQ(&ImageRec, &width, &height, &depth, &ppi,
             &lossyFlag, WsqRec, WsqRecLen))
  {
     free(ImageRec);
     exit(ret);
  }

  printf("ret:        %d\n",ret);
  printf("Width:      %ld\n",width);
  printf("Height:     %ld\n",height);
  printf("Depth:      %ld\n",depth);
  printf("PPI:        %ld\n",ppi);
  printf("Lossy Flag: %ld\n",lossyFlag);


  sprintf(szFileName,"wsq2raw_finger.raw");
  printf("Write file %s\n",szFileName);
  fp = fopen(szFileName,"wb");
  fwrite(ImageRec,1,width*height,fp);
  fclose(fp);

  sgfplib->FreeWSQ(ImageRec);

  free(WsqRec);

  return 0;

}

// ---------------------------------------------------------------- main() ---
int main(int argc, char **argv)
{

  long err;
  char function[100];
  SGDeviceInfoParam deviceInfo;
  char kbBuffer[100];
  char kbWhichFinger[100];
  int fingerLength = 0;
  char *finger;
  FILE *fp = NULL;
  BYTE *imageBuffer1;
  float r_bitrate = WSQ_BITRATE_5_TO_1; 
  DWORD depth = 8;
  DWORD ppi = 500;
  BYTE* WsqRec = NULL;
  DWORD WsqRecLen = 0;
  char szFileName[100];

  printf("\n-------------------------------------\n");
  printf(  "SecuGen WSQ Direct\n");
  printf(  "-------------------------------------\n");

  ///////////////////////////////////////////////
  // Instantiate SGFPLib object
  strcpy(function,"CreateSGFPMObject()");
  printf("\nCall %s\n",function);
  err = CreateSGFPMObject(&sgfplib);
  if (!sgfplib)
  {
    printf("ERROR - Unable to instantiate FPM object.\n\n");
    return -1;
  }
  printf("%s returned: %ld\n",function,err);

  ///////////////////////////////////////////////
  // Init()
  strcpy(function,"Init(SG_DEV_FDU03)");
  printf("\nCall %s\n",function);
  // err = sgfplib->Init(SG_DEV_AUTO);
  err = sgfplib->Init(SG_DEV_FDU03);
  printf("%s returned: %ld\n",function,err);

  if (err != SGFDX_ERROR_NONE)
  {
     printf("ERROR - Unable to initialize device.\n\n");
     return -1;
  }

  ///////////////////////////////////////////////
  // OpenDevice()
  strcpy(function,"OpenDevice(USB_AUTO_DETECT)");
  printf("\nCall %s\n",function);
  err = sgfplib->OpenDevice(USB_AUTO_DETECT);
  // err = sgfplib->OpenDevice(0);
  printf("%s returned: %ld\n",function,err);

  if (err == SGFDX_ERROR_NONE)
  {

    /////////////////////////////////////////////////
    //// setLedOn(true)
    //printf("Press <Enter> to turn fingerprint sensor LEDs on >> ");
    //getc(stdin);
    //strcpy(function,"SetLedOn(true)");
    //printf("\nCall %s\n",function);
    //err = sgfplib->SetLedOn(true);
    //printf("%s returned: %ld\n",function,err);

    /////////////////////////////////////////////////
    //// setLedOn(false)
    //printf("Fingerprint Sensor LEDS should now be illuminated.\n\n");
    //printf("Press <Enter> to turn fingerprint sensor LEDs off >> ");
    //getc(stdin);
    //strcpy(function,"SetLedOn(true)");
    //printf("\nCall %s\n",function);
    //err = sgfplib->SetLedOn(false);
    //printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // getDeviceInfo()
    deviceInfo.DeviceID = 0;
    strcpy(function,"GetDeviceInfo()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetDeviceInfo(&deviceInfo);
    printf("%s returned: %ld\n",function,err);
    if (err == SGFDX_ERROR_NONE)
    {
      printf("\tdeviceInfo.DeviceID   : %ld\n", deviceInfo.DeviceID);
      printf("\tdeviceInfo.DeviceSN   : %s\n",  deviceInfo.DeviceSN);
      printf("\tdeviceInfo.ComPort    : %ld\n", deviceInfo.ComPort);
      printf("\tdeviceInfo.ComSpeed   : %ld\n", deviceInfo.ComSpeed);
      printf("\tdeviceInfo.ImageWidth : %ld\n", deviceInfo.ImageWidth);
      printf("\tdeviceInfo.ImageHeight: %ld\n", deviceInfo.ImageHeight);
      printf("\tdeviceInfo.Contrast   : %ld\n", deviceInfo.Contrast);
      printf("\tdeviceInfo.Brightness : %ld\n", deviceInfo.Brightness);
      printf("\tdeviceInfo.Gain       : %ld\n", deviceInfo.Gain);
      printf("\tdeviceInfo.ImageDPI   : %ld\n", deviceInfo.ImageDPI);
      printf("\tdeviceInfo.FWVersion  : %04X\n", (unsigned int) deviceInfo.FWVersion);
    }
    printf("\n");


    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    // printf("Fingerprint Sensor LEDS should now be off.\n");
    // printf("The next tests will require mutiple captures of the same finger.\n");
    // printf("Which finger would you like to test with? (e.g. left thumb) >> ");
    // fgets(kbWhichFinger,100,stdin); // se comento esto
    //Remove CR/NL (<ENTER>)
    
    fingerLength = strlen(kbWhichFinger);
    for (int i=0; i < strlen(kbWhichFinger); ++i)
    {
      if ((kbWhichFinger[i] == 0x0A) || (kbWhichFinger[i] == 0x0D)|| (kbWhichFinger[i] == 0x00))
      {
        fingerLength = i;
        break;
      }
    }
    if (fingerLength > 0)
    {
      finger = (char*) malloc(fingerLength + 1);
      strncpy(finger,kbWhichFinger,fingerLength);
      finger[fingerLength] = 0x00;
    }
    else
    {
      finger = (char*) malloc (7);
      strcpy(finger,"finger");
    }

    err = NULL;

    printf("Capture 1. Please place [%s]",finger);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    imageBuffer1 = (BYTE*) malloc(deviceInfo.ImageHeight*deviceInfo.ImageWidth);
    strcpy(function,"GetImage()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetImage(imageBuffer1);
    printf("%s returned: %ld\n",function,err);    

    while (err != 0) {
      ///////////////////////////////////////////////
      // getImage() - 1st Capture
      printf("Capture 1. Please place [%s] ",finger);
      std::this_thread::sleep_for(std::chrono::milliseconds(1500));
      // getc(stdin); // se comento 
      imageBuffer1 = (BYTE*) malloc(deviceInfo.ImageHeight*deviceInfo.ImageWidth);
      strcpy(function,"GetImage()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetImage(imageBuffer1);
      printf("%s returned: %ld\n",function,err);
    }



    if (err == SGFDX_ERROR_NONE)
    {
      sprintf(kbBuffer,"%s.raw",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (imageBuffer1 , sizeof (BYTE) , deviceInfo.ImageWidth*deviceInfo.ImageHeight , fp);
      fclose(fp);
    }

    ///////////////////////////////////////////////
    // closeDevice()TEMPLATE_FORMAT_ISO19794
    printf("\nCall CloseDevice()\n");
    err = sgfplib->CloseDevice();
    printf("CloseDevice returned : [%ld]\n",err);

    printf("Encode WSQ File\n");
    printf("Width:    %ld\n",deviceInfo.ImageWidth);
    printf("Height:   %ld\n",deviceInfo.ImageHeight);
    printf("Depth:    %ld\n",depth);
    printf("PPI:      %ld\n",ppi);
    printf("Bit Rate: %f\n",r_bitrate);
    
    if (err = sgfplib->EncodeWSQ(&WsqRec, &WsqRecLen, r_bitrate,
                    imageBuffer1, deviceInfo.ImageWidth, deviceInfo.ImageHeight, depth, ppi, (char*)"SecuGen Comment"))
    {
      ;
    }
    else
    {
      /* code */
      printf("EncodeWSQ() ret: %ld \n",err);
      printf("WSQ Image size is: %ld \n",WsqRecLen);

      sprintf(szFileName,"%s.wsq",finger);
      printf("Write file %s\n",szFileName);
      fp = fopen(szFileName,"wb");
      fwrite(WsqRec,1,WsqRecLen,fp);
      fclose(fp);

      sgfplib->FreeWSQ(WsqRec);
    }
    
    free(imageBuffer1);



  }  

    ///////////////////////////////////////////////
    // Destroy SGFPLib object
    strcpy(function,"DestroySGFPMObject()");
    printf("\nCall %s\n",function);
    err = DestroySGFPMObject(sgfplib);
    printf("%s returned: %ld\n",function,err);

  return 0;
}
