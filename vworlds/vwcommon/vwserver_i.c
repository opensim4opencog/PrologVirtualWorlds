/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:51 2000
 */
/* Compiler settings for vwserver.idl:
    Oicf (OptLev=i2), W0, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IVWServer = {0xF85848FE,0x00F4,0x11D0,{0xA5,0x16,0x00,0xAA,0x00,0xB5,0x95,0xDB}};


const IID IID_DVWServerEvents = {0xF8584900,0x00F4,0x11D0,{0xA5,0x16,0x00,0xAA,0x00,0xB5,0x95,0xDB}};


#ifdef __cplusplus
}
#endif

