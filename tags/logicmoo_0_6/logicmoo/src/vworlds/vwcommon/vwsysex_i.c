/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:35 2000
 */
/* Compiler settings for vwsysex.idl:
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

const IID IID_ISystemExemplar = {0x717B5A86,0x7267,0x11d1,{0x95,0x9F,0x00,0xC0,0x4F,0xD9,0x1F,0x4D}};


const IID IID_IThingExemplar = {0x99AD3618,0xAAB5,0x11D0,{0xBA,0xE3,0x00,0xC0,0x4F,0xC2,0xCA,0xB1}};


const IID IID_IArtifactExemplar = {0x99AD361C,0xAAB5,0x11D0,{0xBA,0xE3,0x00,0xC0,0x4F,0xC2,0xCA,0xB1}};


const IID IID_IAvatarExemplar = {0x99AD361E,0xAAB5,0x11D0,{0xBA,0xE3,0x00,0xC0,0x4F,0xC2,0xCA,0xB1}};


const IID IID_IHostExemplar = {0x44001330,0x1B82,0x11d2,{0xAC,0x71,0x00,0x10,0x4B,0x26,0xA3,0x73}};


const IID IID_IPortalExemplar = {0x99AD3624,0xAAB5,0x11D0,{0xBA,0xE3,0x00,0xC0,0x4F,0xC2,0xCA,0xB1}};


const IID IID_IIWPortalExemplar = {0xFFA01B4F,0x9F4C,0x11d1,{0x80,0xE8,0x00,0xC0,0x4F,0xC2,0xA1,0x29}};


const IID IID_IRoomExemplar = {0x99AD3626,0xAAB5,0x11D0,{0xBA,0xE3,0x00,0xC0,0x4F,0xC2,0xCA,0xB1}};


const IID IID_IStateMachineExemplar = {0x69A2A200,0xF187,0x11d1,{0xA3,0x92,0x00,0x00,0xF8,0x07,0x55,0x8D}};


const IID IID_IWebPageExemplar = {0x63CA06F6,0x5CC2,0x11d2,{0x82,0xF0,0x00,0x10,0x4B,0x26,0xA3,0x73}};


#ifdef __cplusplus
}
#endif
