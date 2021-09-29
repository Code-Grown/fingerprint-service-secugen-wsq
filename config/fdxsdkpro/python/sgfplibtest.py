#! /usr/bin/env python

from pysgfplib import *
from ctypes import *

# main application routine.
if '__main__' == __name__:

  sgfplib = PYSGFPLib()
        
  print('====================================')
  print('Test SecuGen Python Library')
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
     print("  ERROR - Unable to initialize SecuGen library. Exiting\n");
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
    #// setLedOn(true)
    raw_input('Press <Enter> to turn fingerprint scanner LED on: ')
    print('+++ Call sgfplib.SetLedOn(True)')
    result = sgfplib.SetLedOn(True)
    print('  Returned : ' + str(result)) 
    raw_input('Fingerprint scanner LED should now be on. Press <Enter> to continue: ')

    #///////////////////////////////////////////////
    #// setLedOn(false)
    print('+++ Call sgfplib.SetLedOn(False)')
    result = sgfplib.SetLedOn(False)
    print('  Returned : ' + str(result)) 
    raw_input('Fingerprint scanner LED should now be off. Press <Enter> to continue: ')

    #///////////////////////////////////////////////
    #///////////////////////////////////////////////
    print('Fingerprint Sensor LEDS should now be off.');
    print('The next tests will require mutiple captures of the same finger.');
    filename = raw_input('Which finger would you like to test with (no spaces)? (e.g. lt) >> ');
   
    #///////////////////////////////////////////////
    #// getImage() - 1st Capture
    raw_input("Capture 1. Please place " + filename + " on sensor and press <ENTER> ");
    print('+++ Call sgfplib.GetImage()')
    cImageBuffer1 = (c_char*cImageWidth.value*cImageHeight.value)()
    result = sgfplib.GetImage(cImageBuffer1)
    print('  Returned : ' + str(result)) 
    if (result == SGFDxErrorCode.SGFDX_ERROR_NONE):
      image1File = open (filename + "1.raw", "wb")    
      image1File.write(cImageBuffer1)
      image1File.close()
    else:
     print("  ERROR - Unable to capture first image. Exiting\n");
     exit()      

    #///////////////////////////////////////////////
    #// getImageQuality()
    cQuality = c_int(0)
    print("+++ Call getImageQuality()")
    result = sgfplib.GetImageQuality(cImageWidth.value, cImageHeight.value, cImageBuffer1, byref(cQuality))
    print('  Returned : ' + str(result)) 
    print("  Image quality : [" + str(cQuality.value) + "]")

    #///////////////////////////////////////////////
    #// CreateTemplate()
    print("+++ Call CreateTemplate");
    cMinutiaeBuffer1 = (c_char*sgfplib.constant_sg400_template_size)() 
    result = sgfplib.CreateSG400Template(cImageBuffer1, cMinutiaeBuffer1);
    print('  Returned : ' + str(result)) 
    if (result == SGFDxErrorCode.SGFDX_ERROR_NONE):
      minutiae1File = open (filename + "1.min", "wb")    
      minutiae1File.write(cMinutiaeBuffer1)
    else:
     print("  ERROR - Unab0le to create first template. Exiting\n");
     exit()      

    #///////////////////////////////////////////////
    #// getImage() - 2st Capture
    raw_input("Capture 2. Remove and replace " + filename + " on sensor and press <ENTER> ");
    print('+++ Call sgfplib.GetImage()')
    cImageBuffer2 = (c_char*cImageWidth.value*cImageHeight.value)()
    result = sgfplib.GetImage(cImageBuffer2)
    print('  Returned : ' + str(result)) 
    if (result == SGFDxErrorCode.SGFDX_ERROR_NONE):
      image2File = open (filename + "2.raw", "wb")    
      image2File.write(cImageBuffer2)
    else:
     print("  ERROR - Unable to capture second image. Exiting\n");
     exit()      

    #///////////////////////////////////////////////
    #// getImageQuality()
    cQuality = c_int(0)
    print("+++ Call getImageQuality()")
    result = sgfplib.GetImageQuality(cImageWidth.value, cImageHeight.value, cImageBuffer2, byref(cQuality))
    print('  Returned : ' + str(result)) 
    print("  Image quality : [" + str(cQuality.value) + "]")

    #///////////////////////////////////////////////
    #// CreateTemplate()
    print("+++ Call CreateTemplate");
    cMinutiaeBuffer2 = (c_char*sgfplib.constant_sg400_template_size)() 
    result = sgfplib.CreateSG400Template(cImageBuffer2, cMinutiaeBuffer2);
    print('  Returned : ' + str(result)) 
    if (result == SGFDxErrorCode.SGFDX_ERROR_NONE):
      minutiae2File = open (filename + "2.min", "wb")    
      minutiae2File.write(cMinutiaeBuffer2)
    else:
     print("  ERROR - Unable to create second template. Exiting\n");
     exit()      

    #///////////////////////////////////////////////
    #// MatchTemplate()
    cMatched = c_bool(False)
    print("+++ Call MatchTemplate");
    result = sgfplib.MatchTemplate(cMinutiaeBuffer1, cMinutiaeBuffer2, SGFDxSecurityLevel.SL_NORMAL, byref(cMatched));
    print('  Returned : ' + str(result)) 
    if (cMatched.value == True):
      print(  "<<MATCH>>");
    else:
      print(  "<<NO MATCH>>");

    #///////////////////////////////////////////////
    #// GetMatchingScore()
    cScore = c_int(0)
    print("+++ Call GetMatchingScore");
    result = sgfplib.GetMatchingScore(cMinutiaeBuffer1, cMinutiaeBuffer2, byref(cScore));
    print('  Returned : ' + str(result)) 
    print("  Score : [" + str(cScore.value) + "]")

    print('+++ Call sgfplib.CloseDevice()')
    result = sgfplib.CloseDevice()
    print('  Returned : ' + str(result)) 
  #}

  print('+++ Call sgfplib.Terminate()')
  result = sgfplib.Terminate()
  print('  Returned : ' + str(result)) 

  print('====================================')
  print('End of SecuGen Python Library Test')
  print('====================================')

#end __main__
