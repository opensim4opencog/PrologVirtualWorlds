/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:36 2000
 */
/* Compiler settings for authntic.idl:
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

const IID IID_IVWAuthenticate = {0x955A1319,0x731F,0x11D1,{0x90,0x82,0x00,0xC0,0x4F,0xD9,0x1E,0xFE}};


const IID IID_IAuthentication = {0x3D3CE78F,0x7323,0x11D1,{0x90,0x82,0x00,0xC0,0x4F,0xD9,0x1E,0xFE}};


#ifdef __cplusplus
}
#endif

