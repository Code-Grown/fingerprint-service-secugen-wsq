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
  printf(  "SecuGen RAW2WSQ Test\n");
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
  
  printf("\n-------------------------------------\n");
  printf(  "SecuGen WSQ Test\n");
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


  RawToWSQTest();
  WSQToRawTest();


    ///////////////////////////////////////////////
    // Destroy SGFPLib object
    strcpy(function,"DestroySGFPMObject()");
    printf("\nCall %s\n",function);
    err = DestroySGFPMObject(sgfplib);
    printf("%s returned: %ld\n",function,err);

  return 0;
}
