/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:31 2000
 */
/* Compiler settings for vwobject.idl:
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

const IID IID_IWorld = {0x590D9854,0xA417,0x11CF,{0x9B,0xF0,0x00,0x80,0xC7,0xA5,0x6A,0x8A}};


const IID IID_IVWObjectSite = {0x590D9856,0xA417,0x11CF,{0x9B,0xF0,0x00,0x80,0xC7,0xA5,0x6A,0x8A}};


const IID IID_IVWDispatch = {0x590D9858,0xA417,0x11CF,{0x9B,0xF0,0x00,0x80,0xC7,0xA5,0x6A,0x8A}};


#ifdef __cplusplus
}
#endif

