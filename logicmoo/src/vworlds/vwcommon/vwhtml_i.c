/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:49 2000
 */
/* Compiler settings for vwhtml.idl:
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

const IID IID_IVWHtml = {0x7E3832C9,0x4F4E,0x11D0,{0x89,0xAB,0x00,0xA0,0xC9,0x05,0x41,0x29}};


const IID IID_IVWHtmlHelper = {0x7E3832CB,0x4F4E,0x11D0,{0x89,0xAB,0x00,0xA0,0xC9,0x05,0x41,0x29}};


#ifdef __cplusplus
}
#endif

