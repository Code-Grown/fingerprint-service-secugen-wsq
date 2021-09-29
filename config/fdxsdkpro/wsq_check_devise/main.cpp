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
  float r_bitrate = WSQ_BITRATE_5_TO_1; 
  DWORD depth = 8;
  DWORD ppi = 500;
  BYTE* WsqRec = NULL;
  DWORD WsqRecLen = 0;
  char szFileName[100];

  printf("\n-------------------------------------\n");
  printf(  "SecuGen WSQ Check Devise\n");
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
  } else {
    return 0;
  }

  return 0;
}
