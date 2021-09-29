#! /usr/bin/env python

from pysgfplib import *
import time

#///////////////////////////////////////////////////////////////////////////
#//START AUTO ON
#// EnableAutoOnEvent(true,NULL,NULL)
def StartAutoOn():
#{
  StartAutoOn = False
  print('Calling EnableAutoOnEvent(True) ... ')
  result = sgfplib.EnableAutoOnEvent(True)
  print("\treturned "+ str(result));  
  if (result == SGFDxErrorCode.SGFDX_ERROR_NONE):
     StartAutoOn = True
  return StartAutoOn
#}

#///////////////////////////////////////////////////////////////////////////
#//STOP AUTO ON
#//EnableAutoOnEvent(false,NULL,NULL)
def StopAutoOn():
#{
  StopAutoOn = False
  print('Calling EnableAutoOnEvent(False) ... ')
  result = sgfplib.EnableAutoOnEvent(False)
  print("\treturned "+ str(result));  
  if (result == SGFDxErrorCode.SGFDX_ERROR_NONE):
     StopAutoOn = True
  return StopAutoOn
#}

#///////////////////////////////////////////////////////////////////////////
# main application routine.
if '__main__' == __name__:
#{

  sgfplib = PYSGFPLib()

  print('====================================')
  print('Test SecuGen Python Library AUTO_ON')
  print('====================================')

  #///////////////////////////////////////////////
  #// Instantiate SGFPLib object
  print('+++ Call sgfplib.Create()')
  result = sgfplib.Create()
  print('  Returned : ' + str(result))

  if (result != SGFDxErrorCode.SGFDX_ERROR_NONE):
     print("  ERROR - Unable to open SecuGen library. Exiting\n");
     exit()

  #///////////////////////////////////////////////
  #// Init()
  print('+++ Call sgfplib.Init()')
  result = sgfplib.Init(SGFDxDeviceName.SG_DEV_AUTO)
  print('  Returned : ' + str(result))

  if (result != SGFDxErrorCode.SGFDX_ERROR_NONE):
     print("  ERROR - Unable to initialize SecuGen library. Exiting\n");
     exit()

  #///////////////////////////////////////////////
  #// OpenDevice()
  print('Call sgfplib.OpenDevice()')
  result = sgfplib.OpenDevice(0)
  print('  Returned : ' + str(result))

  if (result != SGFDxErrorCode.SGFDX_ERROR_NONE):
  #{
     print("  ERROR - Unable to open the SecuGen device. Exiting\n");
     exit()
  else:

    #///////////////////////////////////////////////
    #// GetDeviceInfo()
    cImageWidth = c_int(0)
    cImageHeight = c_int(0)
    print('+++ Call sgfplib.GetDeviceInfo()')
    result = sgfplib.GetDeviceInfo(byref(cImageWidth), byref(cImageHeight))
    print('  Returned : ' + str(result)) 
    print('  ImageWidth  : ' + str(cImageWidth.value));
    print('  ImageHeight : ' + str(cImageHeight.value));

    #///////////////////////////////////////////////
    cImageBuffer1 = (c_char*cImageWidth.value*cImageHeight.value)()

    #///////////////////////////////////////////////
    #// Set Callback Function
    result = sgfplib.SetCallBackFunction()
    print("SetCallBackFunction()  returned ... " + str(result));
    if (result != SGFDxErrorCode.SGFDX_ERROR_NONE):
       print("\tFAIL\n")
       exit();
    else:
       print("\tSUCCESS\n");

    if (StartAutoOn()):
      n=0  #//Visual feedback
      while (True):
        if (sgfplib.FingerPresent()):
          n=0  #//Reset visual feedback
          print("Finger Present\n")
          if (StopAutoOn() == 0):
            print("StopAutoOn() returned False.\n")
            break
          print("Call GetImage()\n")
          result = sgfplib.GetImage(cImageBuffer1)
          print('GetImage() returned ... '  + str(n)+ '\n')
          if (result != SGFDxErrorCode.SGFDX_ERROR_NONE):
            print("FAIL\n")
          else:
            print("SUCCESS\n")
            image1File = open ("auto_on_image.raw", "wb")
            image1File.write(cImageBuffer1)
            image1File.close()
            print(".............................................................\n")
            continueloop = raw_input("Press 'X' to exit, any other key to continue >> ")
            if (continueloop == 'X'):
              break
            if (StartAutoOn() == False):
              printf("StartAutoOn() returned False.\n")
              break;
        else:
          print("Finger Not Present\n")
          time.sleep(0.5)
          n=n+1 #//Visual feedback
          print('place finger on sensor ... '  + str(n))     
        #}if fingerPresent
      #}end while
    else:
      print("StartAutoOn() returned False.\n")
  #}

  #//////////////////////////////////////////////////////////////////////////
  #// EnableAutoOnEvent(false)
  print("Call sgfplib->EnableAutoOnEvent(false) ... \n");
  result = sgfplib.EnableAutoOnEvent(False);
 
  print('+++ Call sgfplib.CloseDevice()')
  result = sgfplib.CloseDevice()
  print('  Returned : ' + str(result))

  print('+++ Call sgfplib.Terminate()')
  result = sgfplib.Terminate()
  print('  Returned : ' + str(result))

  print('==========================================')
  print('End of SecuGen Python Library Test AUTO_ON')
  print('==========================================')

#end __main__
#}
