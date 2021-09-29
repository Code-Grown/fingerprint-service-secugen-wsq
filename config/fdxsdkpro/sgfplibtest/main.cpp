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

LPSGFPM  sgfplib = NULL;

int  getPIVQuality(int quality)
{
    if (quality <= 20)
        return 20;
    if (quality <= 40)
        return 40;
    if (quality <= 60)
        return 60;
    if (quality <= 80)
        return 80;

    return 100;
}

// ---------------------------------------------------------------- main() ---
int main(int argc, char **argv)
{

  long err;
  DWORD templateSize, templateSizeMax;
  DWORD quality;
  char function[100];
  char kbBuffer[100];
  char kbWhichFinger[100];
  int fingerLength = 0;
  char *finger;
  BYTE *imageBuffer1;
  BYTE *imageBuffer2;
  BYTE *imageBuffer3;
  BYTE *minutiaeBuffer1;
  BYTE *minutiaeBuffer2;
  BYTE *ansiMinutiaeBuffer1;
  BYTE *ansiMinutiaeBuffer2;
  BYTE *isoMinutiaeBuffer1;
  BYTE *isoMinutiaeBuffer2;
  BYTE *isoCompactMinutiaeBuffer1;
  BYTE *isoCompactMinutiaeBuffer2;
  FILE *fp = NULL;
  SGDeviceInfoParam deviceInfo;
  DWORD score;
  SGFingerInfo fingerInfo;
  BOOL matched;
  DWORD nfiq;
  DWORD numOfMinutiae;

  for (int i=0; i < 100; ++i)
     kbWhichFinger[i] = 0x00;

  printf("\n-------------------------------------\n");
  printf(  "SecuGen PRO SGFPLIB Test\n");
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
  strcpy(function,"Init(SG_DEV_AUTO)");
  printf("\nCall %s\n",function);
  err = sgfplib->Init(SG_DEV_AUTO);
  printf("%s returned: %ld\n",function,err);

  if (err != SGFDX_ERROR_NONE)
  {
     printf("ERROR - Unable to initialize device.\n\n");
     return -1;
  }


  ///////////////////////////////////////////////
  // OpenDevice()
  strcpy(function,"OpenDevice(0)");
  printf("\nCall %s\n",function);
  err = sgfplib->OpenDevice(0);
  printf("%s returned: %ld\n",function,err);

  if (err == SGFDX_ERROR_NONE)
  {
    ///////////////////////////////////////////////
    // setLedOn(true)
    printf("Press <Enter> to turn fingerprint sensor LEDs on >> ");
    getc(stdin);
    strcpy(function,"SetLedOn(true)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetLedOn(true);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // setLedOn(false)
    printf("Fingerprint Sensor LEDS should now be illuminated.\n\n");
    printf("Press <Enter> to turn fingerprint sensor LEDs off >> ");
    getc(stdin);
    strcpy(function,"SetLedOn(true)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetLedOn(false);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // setBrightness()
    strcpy(function,"SetBrightness()");
    printf("\nCall %s\n",function);
    err = sgfplib->SetBrightness(30);
    printf("%s returned: %ld\n",function,err);

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
    printf("Fingerprint Sensor LEDS should now be off.\n");
    printf("The next tests will require mutiple captures of the same finger.\n");
    printf("Which finger would you like to test with? (e.g. left thumb) >> ");
    fgets(kbWhichFinger,100,stdin);
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

    ///////////////////////////////////////////////
    // getImage() - 1st Capture
    printf("Capture 1. Please place [%s] on sensor and press <ENTER> ",finger);
    getc(stdin);
    imageBuffer1 = (BYTE*) malloc(deviceInfo.ImageHeight*deviceInfo.ImageWidth);
    strcpy(function,"GetImage()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetImage(imageBuffer1);
    printf("%s returned: %ld\n",function,err);
    if (err == SGFDX_ERROR_NONE)
    {
      sprintf(kbBuffer,"%s1.raw",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (imageBuffer1 , sizeof (BYTE) , deviceInfo.ImageWidth*deviceInfo.ImageHeight , fp);
      fclose(fp);
    }

    ///////////////////////////////////////////////
    // getImageQuality()
    quality = 0;
    strcpy(function,"GetImageQuality()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetImageQuality(deviceInfo.ImageWidth, deviceInfo.ImageHeight, imageBuffer1, &quality);
    printf("%s returned: %ld\n",function,err);
    printf("Image quality : [%ld]\n",quality);

    ///////////////////////////////////////////////
    // ComputeNFIQ()
    strcpy(function,"ComputeNFIQ()");
    printf("\nCall %s\n",function);
    nfiq = sgfplib->ComputeNFIQ(imageBuffer1, deviceInfo.ImageWidth, deviceInfo.ImageHeight);
    printf("NFIQ : [%ld]\n",nfiq);

    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_SG400)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_SG400)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_SG400);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // getMaxTemplateSize()
    strcpy(function,"GetMaxTemplateSize()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMaxTemplateSize(&templateSizeMax);
    printf("%s returned: %ld\n",function,err);
    printf("Max Template Size : [%ld]\n",templateSizeMax);

    ///////////////////////////////////////////////
    // getMinutiae()
    strcpy(function,"CreateTemplate()");
    printf("\nCall %s\n",function);
    minutiaeBuffer1 = (BYTE*) malloc(templateSizeMax);
    fingerInfo.FingerNumber = SG_FINGPOS_UK;
    fingerInfo.ViewNumber = 0;
    fingerInfo.ImpressionType = SG_IMPTYPE_LP;
    fingerInfo.ImageQuality = getPIVQuality(quality); //0 to 100
    err = sgfplib->CreateTemplate(&fingerInfo, imageBuffer1, minutiaeBuffer1);
    printf("CreateTemplate returned : [%ld]\n",err);
    if (err == SGFDX_ERROR_NONE)
    {
      ///////////////////////////////////////////////
      // getTemplateSize()
      strcpy(function,"GetTemplateSize()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetTemplateSize(minutiaeBuffer1, &templateSize);
      printf("%s returned: %ld\n",function,err);
      printf("Template Size : [%ld]\n",templateSize);
      sprintf(kbBuffer,"%s1.min",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (minutiaeBuffer1 , sizeof (BYTE) , templateSize , fp);
      fclose(fp);

      strcpy(function,"GetNumOfMinutiae()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetNumOfMinutiae(TEMPLATE_FORMAT_SG400, minutiaeBuffer1, &numOfMinutiae);
      printf("%s returned: %ld\n",function,err);
      printf("Minutiae Count : [%ld]\n",numOfMinutiae);
    }


    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_ANSI378)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_ANSI378)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ANSI378);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // getMaxTemplateSize()
    strcpy(function,"GetMaxTemplateSize()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMaxTemplateSize(&templateSizeMax);
    printf("%s returned: %ld\n",function,err);
    printf("Max Template Size : [%ld]\n",templateSizeMax);

    ///////////////////////////////////////////////
    // getMinutiae()
    strcpy(function,"CreateTemplate()");
    printf("\nCall %s\n",function);
    ansiMinutiaeBuffer1 = (BYTE*) malloc(templateSizeMax);
    fingerInfo.FingerNumber = SG_FINGPOS_UK;
    fingerInfo.ViewNumber = 0;
    fingerInfo.ImpressionType = SG_IMPTYPE_LP;
    fingerInfo.ImageQuality = getPIVQuality(quality); //0 to 100
    err = sgfplib->CreateTemplate(&fingerInfo, imageBuffer1, ansiMinutiaeBuffer1);
    printf("CreateTemplate returned : [%ld]\n",err);
    if (err == SGFDX_ERROR_NONE)
    {
      ///////////////////////////////////////////////
      // getTemplateSize()
      strcpy(function,"GetTemplateSize()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetTemplateSize(ansiMinutiaeBuffer1, &templateSize);
      printf("%s returned: %ld\n",function,err);
      printf("Template Size : [%ld]\n",templateSize);
      sprintf(kbBuffer,"%s1.ansi378",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (ansiMinutiaeBuffer1 , sizeof (BYTE) , templateSize , fp);
      fclose(fp);

      strcpy(function,"GetNumOfMinutiae()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetNumOfMinutiae(TEMPLATE_FORMAT_ANSI378, ansiMinutiaeBuffer1, &numOfMinutiae);
      printf("%s returned: %ld\n",function,err);
      printf("Minutiae Count : [%ld]\n",numOfMinutiae);

    }


    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_ISO19794)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_ISO19794)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ISO19794);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // getMaxTemplateSize()
    strcpy(function,"GetMaxTemplateSize()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMaxTemplateSize(&templateSizeMax);
    printf("%s returned: %ld\n",function,err);
    printf("Max Template Size : [%ld]\n",templateSizeMax);

    ///////////////////////////////////////////////
    // getMinutiae()
    strcpy(function,"CreateTemplate()");
    printf("\nCall %s\n",function);
    isoMinutiaeBuffer1 = (BYTE*) malloc(templateSizeMax);
    fingerInfo.FingerNumber = SG_FINGPOS_UK;
    fingerInfo.ViewNumber = 0;
    fingerInfo.ImpressionType = SG_IMPTYPE_LP;
    fingerInfo.ImageQuality = getPIVQuality(quality); //0 to 100
    err = sgfplib->CreateTemplate(&fingerInfo, imageBuffer1, isoMinutiaeBuffer1);
    printf("CreateTemplate returned : [%ld]\n",err);
    if (err == SGFDX_ERROR_NONE)
    {
      ///////////////////////////////////////////////
      // getTemplateSize()
      strcpy(function,"GetTemplateSize()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetTemplateSize(isoMinutiaeBuffer1, &templateSize);
      printf("%s returned: %ld\n",function,err);
      printf("Template Size : [%ld]\n",templateSize);
      sprintf(kbBuffer,"%s1.iso",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (isoMinutiaeBuffer1 , sizeof (BYTE) , templateSize , fp);
      fclose(fp);

      strcpy(function,"GetNumOfMinutiae()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetNumOfMinutiae(TEMPLATE_FORMAT_ISO19794, isoMinutiaeBuffer1, &numOfMinutiae);
      printf("%s returned: %ld\n",function,err);
      printf("Minutiae Count : [%ld]\n",numOfMinutiae);

    }


    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_ISO19794_COMPACT)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_ISO19794_COMPACT)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ISO19794_COMPACT);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // getMaxTemplateSize()
    strcpy(function,"GetMaxTemplateSize()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMaxTemplateSize(&templateSizeMax);
    printf("%s returned: %ld\n",function,err);
    printf("Max Template Size : [%ld]\n",templateSizeMax);

    ///////////////////////////////////////////////
    // getMinutiae()
    strcpy(function,"CreateTemplate()");
    printf("\nCall %s\n",function);
    isoCompactMinutiaeBuffer1 = (BYTE*) malloc(templateSizeMax);
    fingerInfo.FingerNumber = SG_FINGPOS_UK;
    fingerInfo.ViewNumber = 0;
    fingerInfo.ImpressionType = SG_IMPTYPE_LP;
    fingerInfo.ImageQuality = getPIVQuality(quality); //0 to 100
    err = sgfplib->CreateTemplate(&fingerInfo, imageBuffer1, isoCompactMinutiaeBuffer1);
    printf("CreateTemplate returned : [%ld]\n",err);
    if (err == SGFDX_ERROR_NONE)
    {
      ///////////////////////////////////////////////
      // getTemplateSize()
      strcpy(function,"GetTemplateSize()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetTemplateSize(isoCompactMinutiaeBuffer1, &templateSize);
      printf("%s returned: %ld\n",function,err);
      printf("Template Size : [%ld]\n",templateSize);
      sprintf(kbBuffer,"%s1.isocompact",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (isoCompactMinutiaeBuffer1 , sizeof (BYTE) , templateSize , fp);
      fclose(fp);

      strcpy(function,"GetNumOfMinutiae()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetNumOfMinutiae(TEMPLATE_FORMAT_ISO19794_COMPACT, isoCompactMinutiaeBuffer1, &numOfMinutiae);
      printf("%s returned: %ld\n",function,err);
      printf("Minutiae Count : [%ld]\n",numOfMinutiae);

    }


    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////


    ///////////////////////////////////////////////
    // getImage() - 2nd Capture
    printf("Capture 2. Remove and replace [%s] on sensor and press <ENTER> ",finger);
    getc(stdin);
    imageBuffer2 = (BYTE*) malloc(deviceInfo.ImageHeight*deviceInfo.ImageWidth);
    DWORD timeout = 5000; //5000 milliseconds
    DWORD imageQuality = 60;   //60%
    strcpy(function,"GetImageEx(imageBuffer2, 5000, NULL,60)");
    printf("\nCall %s\n",function);
    err = sgfplib->GetImageEx(imageBuffer2, timeout, NULL, imageQuality);
    printf("%s returned: %ld\n",function,err);
    if (err == SGFDX_ERROR_NONE)
    {
      sprintf(kbBuffer,"%s2.raw",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (imageBuffer2 , sizeof (BYTE) , deviceInfo.ImageWidth*deviceInfo.ImageHeight , fp);
      fclose(fp);
    }

    ///////////////////////////////////////////////
    // getImageQuality()
    quality = 0;
    strcpy(function,"GetImageQuality()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetImageQuality(deviceInfo.ImageWidth, deviceInfo.ImageHeight, imageBuffer2, &quality);
    printf("%s returned: %ld\n",function,err);
    printf("Image quality : [%ld]\n",quality);

    ///////////////////////////////////////////////
    // ComputeNFIQ()
    strcpy(function,"ComputeNFIQ()");
    printf("\nCall %s\n",function);
    nfiq = sgfplib->ComputeNFIQ(imageBuffer2, deviceInfo.ImageWidth, deviceInfo.ImageHeight);
    printf("NFIQ : [%ld]\n",nfiq);


    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_SG400)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_SG400)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_SG400);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // getMinutiae()
    strcpy(function,"CreateTemplate()");
    printf("\nCall %s\n",function);
    minutiaeBuffer2 = (BYTE*) malloc(templateSizeMax);
    fingerInfo.FingerNumber = SG_FINGPOS_UK;
    fingerInfo.ViewNumber = 0;
    fingerInfo.ImpressionType = SG_IMPTYPE_LP;
    fingerInfo.ImageQuality = getPIVQuality(quality); //0 to 100
    err = sgfplib->CreateTemplate(&fingerInfo, imageBuffer2, minutiaeBuffer2);
    printf("CreateTemplate returned : [%ld]\n",err);
    if (err == SGFDX_ERROR_NONE)
    {
      ///////////////////////////////////////////////
      // getTemplateSize()
      strcpy(function,"GetTemplateSize()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetTemplateSize(minutiaeBuffer2, &templateSize);
      printf("%s returned: %ld\n",function,err);
      printf("Template Size : [%ld]\n",templateSize);
      sprintf(kbBuffer,"%s2.min",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (minutiaeBuffer2 , sizeof (BYTE) , templateSize , fp);
      fclose(fp);

      strcpy(function,"GetNumOfMinutiae()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetNumOfMinutiae(TEMPLATE_FORMAT_SG400, minutiaeBuffer2, &numOfMinutiae);
      printf("%s returned: %ld\n",function,err);
      printf("Minutiae Count : [%ld]\n",numOfMinutiae);
    }


    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_ANSI378)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_ANSI378)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ANSI378);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // getMaxTemplateSize()
    strcpy(function,"GetMaxTemplateSize()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMaxTemplateSize(&templateSizeMax);
    printf("%s returned: %ld\n",function,err);
    printf("Max Template Size : [%ld]\n",templateSizeMax);

    ///////////////////////////////////////////////
    // getMinutiae()
    strcpy(function,"CreateTemplate()");
    printf("\nCall %s\n",function);
    ansiMinutiaeBuffer2 = (BYTE*) malloc(templateSizeMax);
    fingerInfo.FingerNumber = SG_FINGPOS_UK;
    fingerInfo.ViewNumber = 0;
    fingerInfo.ImpressionType = SG_IMPTYPE_LP;
    fingerInfo.ImageQuality = getPIVQuality(quality); //0 to 100
    err = sgfplib->CreateTemplate(&fingerInfo, imageBuffer2, ansiMinutiaeBuffer2);
    printf("CreateTemplate returned : [%ld]\n",err);
    if (err == SGFDX_ERROR_NONE)
    {
      ///////////////////////////////////////////////
      // getTemplateSize()
      strcpy(function,"GetTemplateSize()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetTemplateSize(ansiMinutiaeBuffer2, &templateSize);
      printf("%s returned: %ld\n",function,err);
      printf("Template Size : [%ld]\n",templateSize);
      sprintf(kbBuffer,"%s2.ansi378",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (ansiMinutiaeBuffer2 , sizeof (BYTE) , templateSize , fp);
      fclose(fp);

      strcpy(function,"GetNumOfMinutiae()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetNumOfMinutiae(TEMPLATE_FORMAT_ANSI378, ansiMinutiaeBuffer2, &numOfMinutiae);
      printf("%s returned: %ld\n",function,err);
      printf("Minutiae Count : [%ld]\n",numOfMinutiae);

    }

    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_ISO19794)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_ISO19794)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ISO19794);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // getMaxTemplateSize()
    strcpy(function,"GetMaxTemplateSize()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMaxTemplateSize(&templateSizeMax);
    printf("%s returned: %ld\n",function,err);
    printf("Max Template Size : [%ld]\n",templateSizeMax);

    ///////////////////////////////////////////////
    // getMinutiae()
    strcpy(function,"CreateTemplate()");
    printf("\nCall %s\n",function);
    isoMinutiaeBuffer2 = (BYTE*) malloc(templateSizeMax);
    fingerInfo.FingerNumber = SG_FINGPOS_UK;
    fingerInfo.ViewNumber = 0;
    fingerInfo.ImpressionType = SG_IMPTYPE_LP;
    fingerInfo.ImageQuality = getPIVQuality(quality); //0 to 100
    err = sgfplib->CreateTemplate(&fingerInfo, imageBuffer2, isoMinutiaeBuffer2);
    printf("CreateTemplate returned : [%ld]\n",err);
    if (err == SGFDX_ERROR_NONE)
    {
      ///////////////////////////////////////////////
      // getTemplateSize()
      strcpy(function,"GetTemplateSize()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetTemplateSize(isoMinutiaeBuffer2, &templateSize);
      printf("%s returned: %ld\n",function,err);
      printf("Template Size : [%ld]\n",templateSize);
      sprintf(kbBuffer,"%s2.iso",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (isoMinutiaeBuffer2 , sizeof (BYTE) , templateSize , fp);
      fclose(fp);

      strcpy(function,"GetNumOfMinutiae()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetNumOfMinutiae(TEMPLATE_FORMAT_ISO19794, isoMinutiaeBuffer2, &numOfMinutiae);
      printf("%s returned: %ld\n",function,err);
      printf("Minutiae Count : [%ld]\n",numOfMinutiae);
    }

    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_ISO19794_COMPACT)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_ISO19794_COMPACT)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ISO19794_COMPACT);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // getMaxTemplateSize()
    strcpy(function,"GetMaxTemplateSize()");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMaxTemplateSize(&templateSizeMax);
    printf("%s returned: %ld\n",function,err);
    printf("Max Template Size : [%ld]\n",templateSizeMax);

    ///////////////////////////////////////////////
    // getMinutiae()
    strcpy(function,"CreateTemplate()");
    printf("\nCall %s\n",function);
    isoCompactMinutiaeBuffer2 = (BYTE*) malloc(templateSizeMax);
    fingerInfo.FingerNumber = SG_FINGPOS_UK;
    fingerInfo.ViewNumber = 0;
    fingerInfo.ImpressionType = SG_IMPTYPE_LP;
    fingerInfo.ImageQuality = getPIVQuality(quality); //0 to 100
    err = sgfplib->CreateTemplate(&fingerInfo, imageBuffer2, isoCompactMinutiaeBuffer2);
    printf("CreateTemplate returned : [%ld]\n",err);
    if (err == SGFDX_ERROR_NONE)
    {
      ///////////////////////////////////////////////
      // getTemplateSize()
      strcpy(function,"GetTemplateSize()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetTemplateSize(isoCompactMinutiaeBuffer2, &templateSize);
      printf("%s returned: %ld\n",function,err);
      printf("Template Size : [%ld]\n",templateSize);
      sprintf(kbBuffer,"%s2.isocompact",finger);
      fp = fopen(kbBuffer,"wb");
      fwrite (isoCompactMinutiaeBuffer2 , sizeof (BYTE) , templateSize , fp);
      fclose(fp);

      strcpy(function,"GetNumOfMinutiae()");
      printf("\nCall %s\n",function);
      err = sgfplib->GetNumOfMinutiae(TEMPLATE_FORMAT_ISO19794, isoCompactMinutiaeBuffer2, &numOfMinutiae);
      printf("%s returned: %ld\n",function,err);
      printf("Minutiae Count : [%ld]\n",numOfMinutiae);
    }




    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////
    ///////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_SG400)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_SG400)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_SG400);
    printf("%s returned: %ld\n",function,err);


    ///////////////////////////////////////////////
    // MatchTemplate()
    strcpy(function,"MatchTemplate(SG4001,SG4002)");
    printf("\nCall %s\n",function);
    err = sgfplib->MatchTemplate(minutiaeBuffer1, minutiaeBuffer2, SL_NORMAL, &matched);
    printf("%s returned: %ld\n",function,err);
    if (matched == true)
      printf("<<MATCH>>\n");
    else
      printf("<<NO MATCH>>\n");

    ///////////////////////////////////////////////
    // GetMatchingScore()
    strcpy(function,"GetMatchingScore(SG4001,SG4002)");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMatchingScore(minutiaeBuffer1, minutiaeBuffer2, &score);
    printf("%s returned: %ld\n",function,err);
    printf("Score is : [%ld]\n",score);


    /////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_ANSI378)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_ANSI378)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ANSI378);
    printf("%s returned: %ld\n",function,err);


    ///////////////////////////////////////////////
    // MatchTemplate()
    strcpy(function,"MatchTemplate(ANSI1,ANSI2)");
    printf("\nCall %s\n",function);
    err = sgfplib->MatchAnsiTemplate(ansiMinutiaeBuffer1, 0, ansiMinutiaeBuffer2, 0, SL_NORMAL, &matched);
    printf("%s returned: %ld\n",function,err);
    if (matched == true)
      printf("<<MATCH>>\n");
    else
      printf("<<NO MATCH>>\n");

    ///////////////////////////////////////////////
    // GetMatchingScore()
    strcpy(function,"GetMatchingScore(ANSI1,ANSI2)");
    printf("\nCall %s\n",function);
    err = sgfplib->GetAnsiMatchingScore(ansiMinutiaeBuffer1, 0, ansiMinutiaeBuffer2, 0, &score);
    printf("%s returned: %ld\n",function,err);
    printf("Score is : [%ld]\n",score);

    ///////////////////////////////////
    //Merge ANSI378 Templates
    printf("--------\nMerge ANSI378 Templates\n--------\n");
    DWORD size = 0;
    err = sgfplib->GetTemplateSizeAfterMerge(ansiMinutiaeBuffer1, ansiMinutiaeBuffer2, &size);
    printf("GetTemplateSizeAfterMerge returned : [%ld]\n",err);
    printf("ANSI-1 + ANSI-2 Size : [%ld]\n",size);
    BYTE* mergedAnsiTemplate1 = (BYTE*) malloc(size);
    err = sgfplib->MergeAnsiTemplate(ansiMinutiaeBuffer1, ansiMinutiaeBuffer2, mergedAnsiTemplate1);
    printf("MergeAnsiTemplate returned : [%ld]\n",err);
    err = sgfplib->MatchAnsiTemplate(ansiMinutiaeBuffer1, 0, mergedAnsiTemplate1, 0, SL_NORMAL, &matched);
    printf("MatchAnsiTemplate(ANSI1,ANSIMERGE) returned : [%ld]\n",err);
    printf("ANSI-1 <> ANSI-MERGED Match Result : [%d]\n",matched);
    err = sgfplib->GetAnsiMatchingScore(ansiMinutiaeBuffer1, 0, mergedAnsiTemplate1, 0, &score);
    printf("GetAnsiMatchingScore(ANSI1,ANSIMERGE) returned : [%ld]\n",err);
    printf("ANSI-1 <> ANSI-MERGED score is : [%ld]\n",score);

    ///////////////////////////////////
    //View ANSI378 Info
    printf("--------\n");
    SGANSITemplateInfo ansiTemplateInfo;
    err = sgfplib->GetAnsiTemplateInfo(mergedAnsiTemplate1, &ansiTemplateInfo);
    printf("GetAnsiTemplateInfo returned : [%ld]\n",err);
    printf("   TotalSamples=%ld\n",ansiTemplateInfo.TotalSamples);
    for (int i=0; i<ansiTemplateInfo.TotalSamples; ++i){
    	printf("   Sample[%d].FingerNumber=%d\n",i,ansiTemplateInfo.SampleInfo[i].FingerNumber);
    	printf("   Sample[%d].ImageQuality=%d\n",i,ansiTemplateInfo.SampleInfo[i].ImageQuality);
    	printf("   Sample[%d].ImpressionType=%d\n",i,ansiTemplateInfo.SampleInfo[i].ImpressionType);
    	printf("   Sample[%d].ViewNumber=%d\n",i,ansiTemplateInfo.SampleInfo[i].ViewNumber);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_ISO19794)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_ISO19794)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ISO19794);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // MatchTemplate()
    strcpy(function,"MatchIsoTemplate(ISO1,ISO2)");
    printf("\nCall %s\n",function);
    err = sgfplib->MatchIsoTemplate(isoMinutiaeBuffer1, 0, isoMinutiaeBuffer2, 0, SL_NORMAL, &matched);
    printf("%s returned: %ld\n",function,err);
    if (matched == true)
      printf("<<MATCH>>\n");
    else
      printf("<<NO MATCH>>\n");

    ///////////////////////////////////////////////
    // GetIsoMatchingScore()
    strcpy(function,"GetIsoMatchingScore(ISO1,ISO2)");
    printf("\nCall %s\n",function);
    err = sgfplib->GetIsoMatchingScore(isoMinutiaeBuffer1, 0, isoMinutiaeBuffer2, 0, &score);
    printf("%s returned: %ld\n",function,err);
    printf("Score is : [%ld]\n",score);


    ///////////////////////////////////
    //Merge ISO Templates
    printf("--------\nMerge ISO Templates\n--------\n");
    DWORD isosize = 0;
    err = sgfplib->GetIsoTemplateSizeAfterMerge(isoMinutiaeBuffer1, isoMinutiaeBuffer2, &isosize);
    printf("SGFPM_GetIsoTemplateSizeAfterMerge returned : [%ld]\n",err);
    printf("ISO-1 + ISO-2 Size : [%ld]\n",size);
    BYTE* mergedIsoTemplate1 = (BYTE*) malloc(size);
    err = sgfplib->MergeIsoTemplate(isoMinutiaeBuffer1, isoMinutiaeBuffer2, mergedIsoTemplate1);
    printf("MergeIsoTemplate returned : [%ld]\n",err);
    err = sgfplib->MatchIsoTemplate(isoMinutiaeBuffer1, 0, mergedIsoTemplate1, 0, SL_NORMAL, &matched);
    printf("MatchIsoTemplate(ISO1,ISOMERGE) returned : [%ld]\n",err);
    printf("ISO-1 <> ISO-MERGED Match Result : [%d]\n",matched);
    err = sgfplib->GetIsoMatchingScore(isoMinutiaeBuffer1, 0, mergedIsoTemplate1, 0, &score);
    printf("GetIsoMatchingScore(ISO-1,ISO-MERGED ) returned : [%ld]\n",err);
    printf("ISO-1 <> ISO-MERGED  score is : [%ld]\n",score);

    ///////////////////////////////////
    //View ISO Info
    printf("--------\n");
    SGISOTemplateInfo isoTemplateInfo;
    err = sgfplib->GetIsoTemplateInfo(mergedIsoTemplate1, &isoTemplateInfo);
    printf("GetIsoTemplateInfo returned : [%ld]\n",err);
    printf("   TotalSamples=%ld\n",isoTemplateInfo.TotalSamples);
    for (int i=0; i<isoTemplateInfo.TotalSamples; ++i){
    	printf("   Sample[%d].FingerNumber=%d\n",i,isoTemplateInfo.SampleInfo[i].FingerNumber);
    	printf("   Sample[%d].ImageQuality=%d\n",i,isoTemplateInfo.SampleInfo[i].ImageQuality);
    	printf("   Sample[%d].ImpressionType=%d\n",i,isoTemplateInfo.SampleInfo[i].ImpressionType);
    	printf("   Sample[%d].ViewNumber=%d\n",i,isoTemplateInfo.SampleInfo[i].ViewNumber);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////
    // SetTemplateFormat(TEMPLATE_FORMAT_ISO19794_COMPACT)
    strcpy(function,"SetTemplateFormat(TEMPLATE_FORMAT_ISO19794_COMPACT)");
    printf("\nCall %s\n",function);
    err = sgfplib->SetTemplateFormat(TEMPLATE_FORMAT_ISO19794_COMPACT);
    printf("%s returned: %ld\n",function,err);

    ///////////////////////////////////////////////
    // MatchTemplate()
    strcpy(function,"MatchTemplate(ISOCOMPACT1,ISOCOMPACT2)");
    printf("\nCall %s\n",function);
    err = sgfplib->MatchTemplate(isoCompactMinutiaeBuffer1, isoCompactMinutiaeBuffer2, SL_NORMAL, &matched);
    printf("%s returned: %ld\n",function,err);
    if (matched == true)
      printf("<<MATCH>>\n");
    else
      printf("<<NO MATCH>>\n");

    ///////////////////////////////////////////////
    // GetMatchingScore()
    strcpy(function,"GetMatchingScore(ISOCOMPACT1,ISOCOMPACT2)");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMatchingScore(isoCompactMinutiaeBuffer1, isoCompactMinutiaeBuffer2, &score);
    printf("%s returned: %ld\n",function,err);
    printf("Score is : [%ld]\n",score);


    ///////////////////////////////////
    //Merge ISO Templates
    printf("--------\nMerge ISOCOMPACT Templates\n--------\n");
    DWORD isocompactsize = 0;
    err = sgfplib->GetIsoCompactTemplateSizeAfterMerge(isoCompactMinutiaeBuffer1, isoCompactMinutiaeBuffer2, &isocompactsize);
    printf("GetIsoCompactTemplateSizeAfterMerge returned : [%ld]\n",err);
    printf("ISOCOMPACT-1 + ISOCOMPACT-2 Size : [%ld]\n",size);
    BYTE* mergedIsoCompactTemplate1 = (BYTE*) malloc(size);
    err = sgfplib->MergeIsoCompactTemplate(isoCompactMinutiaeBuffer1, isoCompactMinutiaeBuffer2, mergedIsoCompactTemplate1);
    printf("MergeIsoCompactTemplate returned : [%ld]\n",err);
    err = sgfplib->MatchIsoCompactTemplate(isoCompactMinutiaeBuffer1, 0, mergedIsoCompactTemplate1, 0, SL_NORMAL, &matched);
    printf("MatchIsoCompactTemplate(ISOCOMPACT1,ISOCOMPACTMERGE) returned : [%ld]\n",err);
    printf("ISO-1 <> ISO-MERGED Match Result : [%d]\n",matched);
    err = sgfplib->GetIsoCompactMatchingScore(isoCompactMinutiaeBuffer1, 0, mergedIsoCompactTemplate1, 0, &score);
    printf("GetIsoCompactMatchingScore(ISOCOMPACT-1,ISOCOMPACT-MERGED ) returned : [%ld]\n",err);
    printf("ISO-1 <> ISO-MERGED  score is : [%ld]\n",score);

    ///////////////////////////////////
    //View ISO Info
    printf("--------\n");
    SGISOTemplateInfo isoCompactTemplateInfo;
    err = sgfplib->GetIsoCompactTemplateInfo(mergedIsoCompactTemplate1, &isoCompactTemplateInfo);
    printf("GetIsoCompactTemplateInfo returned : [%ld]\n",err);
    printf("   TotalSamples=%ld\n",isoCompactTemplateInfo.TotalSamples);
    for (int i=0; i<isoCompactTemplateInfo.TotalSamples; ++i){
    	printf("   Sample[%d].FingerNumber=%d\n",i,isoCompactTemplateInfo.SampleInfo[i].FingerNumber);
    	printf("   Sample[%d].ImageQuality=%d\n",i,isoCompactTemplateInfo.SampleInfo[i].ImageQuality);
    	printf("   Sample[%d].ImpressionType=%d\n",i,isoCompactTemplateInfo.SampleInfo[i].ImpressionType);
    	printf("   Sample[%d].ViewNumber=%d\n",i,isoCompactTemplateInfo.SampleInfo[i].ViewNumber);
    }




    ///////////////////////////////////////////////
    // MatchTemplateEx()
    strcpy(function,"MatchTemplateEx(SG4001,ANSI2)");
    printf("\nCall %s\n",function);
    err = sgfplib->MatchTemplateEx(minutiaeBuffer1, TEMPLATE_FORMAT_SG400, 0, ansiMinutiaeBuffer2, TEMPLATE_FORMAT_ANSI378, 0, SL_NORMAL, &matched);
    printf("%s returned: %ld\n",function,err);
    if (matched == true)
      printf("<<MATCH>>\n");
    else
      printf("<<NO MATCH>>\n");

    ///////////////////////////////////////////////
    // GetMatchingScore()
    strcpy(function,"GetMatchingScoreEx(SG4001,ANSI2)");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMatchingScoreEx(minutiaeBuffer1, TEMPLATE_FORMAT_SG400, 0, ansiMinutiaeBuffer2, TEMPLATE_FORMAT_ANSI378, 0, &score);
    printf("%s returned: %ld\n",function,err);
    printf("Score is : [%ld]\n",score);


    ///////////////////////////////////////////////
    // MatchTemplate()
    strcpy(function,"MatchTemplateEx(ISO1,ANSI2)");
    printf("\nCall %s\n",function);
    err = sgfplib->MatchTemplateEx(isoMinutiaeBuffer1, TEMPLATE_FORMAT_ISO19794, 0, ansiMinutiaeBuffer2, TEMPLATE_FORMAT_ANSI378, 0, SL_NORMAL, &matched);
    printf("%s returned: %ld\n",function,err);
    if (matched == true)
      printf("<<MATCH>>\n");
    else
      printf("<<NO MATCH>>\n");

    ///////////////////////////////////////////////
    // GetMatchingScore()
    strcpy(function,"GetMatchingScoreEx(ISO1,ANSI2)");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMatchingScoreEx(isoMinutiaeBuffer1, TEMPLATE_FORMAT_ISO19794, 0, ansiMinutiaeBuffer2, TEMPLATE_FORMAT_ANSI378, 0, &score);
    printf("%s returned: %ld\n",function,err);
    printf("Score is : [%ld]\n",score);

    ///////////////////////////////////////////////
    // MatchTemplate()
    strcpy(function,"MatchTemplateEx(ISOCOMPACT1,ANSI2)");
    printf("\nCall %s\n",function);
    err = sgfplib->MatchTemplateEx(isoCompactMinutiaeBuffer1, TEMPLATE_FORMAT_ISO19794_COMPACT, 0, ansiMinutiaeBuffer2, TEMPLATE_FORMAT_ANSI378, 0, SL_NORMAL, &matched);
    printf("%s returned: %ld\n",function,err);
    if (matched == true)
      printf("<<MATCH>>\n");
    else
      printf("<<NO MATCH>>\n");

    ///////////////////////////////////////////////
    // GetMatchingScore()
    strcpy(function,"GetMatchingScoreEx(ISOCOMPACT1,ANSI2)");
    printf("\nCall %s\n",function);
    err = sgfplib->GetMatchingScoreEx(isoCompactMinutiaeBuffer1, TEMPLATE_FORMAT_ISO19794_COMPACT, 0, ansiMinutiaeBuffer2, TEMPLATE_FORMAT_ANSI378, 0, &score);
    printf("%s returned: %ld\n",function,err);
    printf("Score is : [%ld]\n",score);




    ///////////////////////////////////////////////
    // closeDevice()TEMPLATE_FORMAT_ISO19794
    printf("\nCall CloseDevice()\n");
    err = sgfplib->CloseDevice();
    printf("CloseDevice returned : [%ld]\n",err);

    ///////////////////////////////////////////////
    // Destroy SGFPLib object
    strcpy(function,"DestroySGFPMObject()");
    printf("\nCall %s\n",function);
    err = DestroySGFPMObject(sgfplib);
    printf("%s returned: %ld\n",function,err);

    free(imageBuffer1);
    free(imageBuffer2);
    free(minutiaeBuffer1);
    free(minutiaeBuffer2);
    free(ansiMinutiaeBuffer1);
    free(ansiMinutiaeBuffer2);
    free(isoMinutiaeBuffer1);
    free(isoMinutiaeBuffer2);
    free(finger);
    imageBuffer1 = NULL;
    imageBuffer2 = NULL;
    minutiaeBuffer1 = NULL;
    minutiaeBuffer2 = NULL;
    ansiMinutiaeBuffer1 = NULL;
    ansiMinutiaeBuffer2 = NULL;
    isoMinutiaeBuffer1 = NULL;
    isoMinutiaeBuffer2 = NULL;
    finger = NULL;
  }
  return 0;
}
