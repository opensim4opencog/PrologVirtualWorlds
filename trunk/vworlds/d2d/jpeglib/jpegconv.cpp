/*
 * jpegconv.cpp
 *
 * Copyright (C) 1994, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 */

#include <windows.h>

#include "all.h"
#include "jpeglib.h"
#include "jerror.h"
//#include "..\..\utility\utility.h"

void *  MemAlloc(size_t cb);
void    MemFree(void *pv);

/*
 * Error exit handler: must not return to caller.
 *
 * Applications may override this if they want to get control back after
 * an error.  Typically one would longjmp somewhere instead of exiting.
 * The setjmp buffer can be made a private field within an expanded error
 * handler object.  Note that the info needed to generate an error message
 * is stored in the error object, so you can generate the message now or
 * later, at your convenience.
 * You should make sure that the JPEG object is cleaned up (with jpeg_abort
 * or jpeg_destroy) at some point.
 */

/* Expanded data source object for stdio input */

typedef struct {
  struct jpeg_source_mgr pub;   /* public fields */

  void * pimgfilt;
  FILE * infile;        /* source stream */
  JOCTET * buffer;      /* start of buffer */
  boolean start_of_file;    /* have we gotten any data yet? */
} my_source_mgr;

typedef my_source_mgr * my_src_ptr;


extern "C" METHODDEF void
error_exit (j_common_ptr cinfo)
{
  /* Let the memory manager delete any temp files before we die */
  jpeg_destroy(cinfo);
}

/*
 * Actual output of an error or trace message.
 * Applications may override this method to send JPEG messages somewhere
 * other than stderr.
 */

extern "C" METHODDEF void
output_message (j_common_ptr cinfo)
{
}


/*
 * Decide whether to emit a trace or warning message.
 * msg_level is one of:
 *   -1: recoverable corrupt-data warning, may want to abort.
 *    0: important advisory messages (always display to user).
 *    1: first level of tracing detail.
 *    2,3,...: successively more detailed tracing messages.
 * An application might override this method if it wanted to abort on warnings
 * or change the policy about which messages to display.
 */

extern "C" METHODDEF void
emit_message (j_common_ptr cinfo, int msg_level)
{
}


/*
 * Format a message string for the most recent JPEG error or message.
 * The message is stored into buffer, which should be at least JMSG_LENGTH_MAX
 * characters.  Note that no '\n' character is added to the string.
 * Few applications should need to override this method.
 */

extern "C" METHODDEF void
format_message (j_common_ptr cinfo, char * buffer)
{
}


/*
 * Reset error state variables at start of a new image.
 * This is called during compression startup to reset trace/error
 * processing to default state, without losing any application-specific
 * method pointers.  An application might possibly want to override
 * this method if it has additional error processing state.
 */

extern "C" METHODDEF void
reset_error_mgr (j_common_ptr cinfo)
{
  cinfo->err->num_warnings = 0;
  /* trace_level is not reset since it is an application-supplied parameter */
  cinfo->err->msg_code = 0; /* may be useful as a flag for "no error" */
}


/*
 * Fill in the standard error-handling methods in a jpeg_error_mgr object.
 * Typical call is:
 *  struct jpeg_compress_struct cinfo;
 *  struct jpeg_error_mgr err;
 *
 *  cinfo.err = jpeg_std_error(&err);
 * after which the application may override some of the methods.
 */

struct jpeg_error_mgr *
jpeg_std_error (struct jpeg_error_mgr * err)
{
  err->error_exit = error_exit;
  err->emit_message = emit_message;
  err->output_message = output_message;
  err->format_message = format_message;
  err->reset_error_mgr = reset_error_mgr;

  err->trace_level = 0;     /* default = no tracing */
  err->num_warnings = 0;    /* no warnings emitted yet */
  err->msg_code = 0;        /* may be useful as a flag for "no error" */

  /* Initialize message table pointers */
  err->jpeg_message_table = NULL;
  err->last_jpeg_message = (int) JMSG_LASTMSGCODE - 1;

  err->addon_message_table = NULL;
  err->first_addon_message = 0; /* for safety */
  err->last_addon_message = 0;

  return err;
}


#define INPUT_BUF_SIZE  4096    /* choose an efficiently fread'able size */
#define IMGFILT_BUF_SIZE  512   /* choose a size to allow overlap */

/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */

extern "C" METHODDEF void
init_source (j_decompress_ptr cinfo)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;

  /* We reset the empty-input-file flag for each image,
   * but we don't clear the input buffer.
   * This is correct behavior for reading a series of images from one source.
   */
  src->start_of_file = TRUE;
}


/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In typical applications, this should read fresh data into the buffer
 * (ignoring the current state of next_input_byte & bytes_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been reloaded.  It is not necessary to
 * fill the buffer entirely, only to obtain at least one more byte.
 *
 * There is no such thing as an EOF return.  If the end of the file has been
 * reached, the routine has a choice of ERREXIT() or inserting fake data into
 * the buffer.  In most cases, generating a warning message and inserting a
 * fake EOI marker is the best course of action --- this will allow the
 * decompressor to output however much of the image is there.  However,
 * the resulting error message is misleading if the real problem is an empty
 * input file, so we handle that case specially.
 *
 * In applications that need to be able to suspend compression due to input
 * not being available yet, a FALSE return indicates that no more data can be
 * obtained right now, but more may be forthcoming later.  In this situation,
 * the decompressor will return to its caller (with an indication of the
 * number of scanlines it has read, if any).  The application should resume
 * decompression after it has loaded more data into the input buffer.  Note
 * that there are substantial restrictions on the use of suspension --- see
 * the documentation.
 *
 * When suspending, the decompressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_input_byte & bytes_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point must be rescanned after resumption, so move it to
 * the front of the buffer rather than discarding it.
 */

extern "C" METHODDEF boolean
fill_input_buffer (j_decompress_ptr cinfo)
{
    return TRUE;
}


/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */

extern "C" METHODDEF void
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  my_src_ptr src = (my_src_ptr) cinfo->src;

  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      (void) fill_input_buffer(cinfo);
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}


/*
 * An additional method that can be provided by data source modules is the
 * resync_to_restart method for error recovery in the presence of RST markers.
 * For the moment, this source module just uses the default resync method
 * provided by the JPEG library.  That method assumes that no backtracking
 * is possible.
 */


/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

extern "C" METHODDEF void
term_source (j_decompress_ptr cinfo)
{
  /* no work necessary here */
}

/*
 * Prepare for input from a large memory buffer.
 */

extern "C" GLOBAL void
jpeg_memory_src (j_decompress_ptr cinfo, unsigned char *pdata, int len)
{
  my_src_ptr src;

  /* The source object and input buffer are made permanent so that a series
   * of JPEG images can be read from the same file by calling jpeg_stdio_src
   * only before the first one.  (If we discarded the buffer at the end of
   * one image, we'd likely lose the start of the next one.)
   * This makes it unsafe to use this manager and a different source
   * manager serially with the same JPEG object.  Caveat programmer.
   */
  if (cinfo->src == NULL) { /* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                  J_SIZEOF(my_source_mgr));
    src = (my_src_ptr) cinfo->src;

    /*
        TODO Is it a problem that this buffer was allocated with malloc
        instead of the jpeg library routines?
    */
    src->buffer = pdata;
  }

  src = (my_src_ptr) cinfo->src;
  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = term_source;
  src->infile = NULL;
  src->pub.bytes_in_buffer = len; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = pdata; /* until buffer loaded */
}


/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

struct my_error_mgr {
  struct jpeg_error_mgr pub;    /* "public" fields */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

extern "C" METHODDEF void
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
//  longjmp(myerr->setjmp_buffer, 1);
}





/*
 * Sample routine for JPEG decompression.  We assume that the JPEG file image
 * is passed in.  We want to return a pointer on success, NULL on error.
 */ 

#define     artworkDirectory    "artwork\\"
#define     urlDirectory        "http://fedsrv/artwork/"

HBITMAP LoadImage(HINSTANCE inst, char * name)
{

int datalen = 0;
unsigned char * inputdata = NULL;   
FILE* f = fopen(name, "rb");
HBITMAP hbm = NULL;

	if (f)
	{
		//Get the length of the file
		fseek(f, 0L, SEEK_END);
		datalen = ftell(f);
		fseek(f, 0L, SEEK_SET);
		if (datalen > 0)
		{
			inputdata = new unsigned char [datalen];
			if (inputdata)
				datalen = fread(inputdata, 1, datalen, f);
			else
				datalen = 0;
		}
		else
		{
			inputdata = NULL;
		}

		fclose(f);
	}
	else
		return NULL;
 
	/* This struct contains the JPEG decompression parameters and pointers to
	* working space (which is allocated as needed by the JPEG library).
	*/
	struct jpeg_decompress_struct cinfo;
	/* We use our private extension JPEG error handler. */
	struct my_error_mgr jerr;
	/* More stuff */
	JSAMPARRAY buffer;        /* Output row buffer */
	int row_stride;       /* physical row width in output buffer */

	unsigned char *pRGB = NULL;
	unsigned char *pCurRow = NULL;;
	int bytes = 0;
	int xsize;
	int ysize;
	int irow;
	int x;
	int y;
	int padded_xsize;
	int num_rows_read;
	int xPixel;
	int notifyRow = 0;

	
	/* Step 1: allocate and initialize JPEG decompression object */

	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	/* Now we can initialize the JPEG decompression object. */  
	jpeg_create_decompress(&cinfo);

	/* Step 2: specify data source (eg, a file, or a memory buffer) */

	jpeg_memory_src(&cinfo, inputdata, datalen);

	/* Step 3: read file parameters with jpeg_read_header() */

	(void) jpeg_read_header(&cinfo, TRUE);
	/* We can ignore the return value from jpeg_read_header since
	*   (a) suspension is not possible with the stdio data source, and
	*   (b) we passed TRUE to reject a tables-only JPEG file as an error.
	* See libjpeg.doc for more info.
	*/

	/* Step 4: set parameters for decompression */

	cinfo.dct_method = JDCT_IFAST;
	switch (cinfo.jpeg_color_space)  
	{      
	case JCS_GRAYSCALE:    
	   cinfo.out_color_space = JCS_GRAYSCALE;      
	   break;
	default:
	   cinfo.out_color_space = JCS_RGB;
	   break;
	}

	/* We want the actual RGB data here */
	cinfo.quantize_colors = FALSE;

	/* Step 5: Start decompressor */
	jpeg_start_decompress(&cinfo);

	/* We may need to do some setup of our own at this point before reading
	* the data.  After jpeg_start_decompress() we have the correct scaled
	* output image dimensions available, as well as the output colormap
	* if we asked for color quantization.
	* In this example, we need to make an output work buffer of the right size.
	*/

	xsize = cinfo.output_width;     
	ysize = cinfo.output_height;
    
	/*      
	TODO is it really ok to call my_error_exit this way, from here?
	*/
    
	padded_xsize = xsize*3;
    
	if (padded_xsize%4)
	{
	   padded_xsize = padded_xsize + 4 - (padded_xsize%4);
	}


	BITMAPINFO * pBMI = (BITMAPINFO*) malloc(sizeof(BITMAPINFOHEADER));
	if (!pBMI)
	{
		if (inputdata)
			delete [] inputdata;
		return NULL;
	}


	// Fill in the header info.
	BITMAPINFOHEADER* pBI = (BITMAPINFOHEADER*) pBMI;
	pBI->biSize = sizeof(BITMAPINFOHEADER);
	pBI->biWidth = xsize;
	pBI->biHeight = ysize;
	pBI->biPlanes = 1;
	pBI->biBitCount = 24;
	pBI->biCompression = BI_RGB;
	pBI->biSizeImage = 0;
	pBI->biXPelsPerMeter = 0;
	pBI->biYPelsPerMeter = 0;
	pBI->biClrUsed = 0;
	pBI->biClrImportant = 0;

	HDC src = CreateCompatibleDC(NULL);

	VOID * pbits;
	hbm = ::CreateDIBSection(src,pBMI,DIB_RGB_COLORS,&pbits,NULL,NULL);

	DeleteDC(src);

	free(pBMI);     //DEP

	pRGB = (BYTE *)pbits;
	if (!pRGB)
	{
	   my_error_exit((j_common_ptr) &cinfo);
	}
    
	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;

	/* Make a sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 8);

	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */


  
	y = 0;
  
	while (y < ysize) {
	   num_rows_read = jpeg_read_scanlines(&cinfo, buffer, 8);
	   if (!num_rows_read)
			goto abort;

	   if (cinfo.out_color_space == JCS_RGB)
	   {
		   for (irow = 0; irow < num_rows_read; irow++)
		   {
			   pCurRow = pRGB + padded_xsize*(ysize - y - 1);	/* the DIB is stored upside down */
			   for (x=0; x<xsize; x++)
				{
					/*
						DIB's are stored blue-green-red (backwards)
					*/
					int r, g, b;
					*pCurRow++ = b = buffer[irow][x*3+RGB_BLUE];
					*pCurRow++ = g = buffer[irow][x*3+RGB_GREEN];
					*pCurRow++ = r = buffer[irow][x*3+RGB_RED];
					//SetPixel(src, bytes % xsize, bytes/xsize, RGB(r,g,b));
					bytes++;
				}
				y++;
			}
		}
		else
		{
			for (irow = 0; irow < num_rows_read; irow++)
			{
				pCurRow = pRGB + padded_xsize*(ysize - y - 1);  /* the DIB is stored upside down */

				for (x=0; x<xsize; x++)
				{
					xPixel = buffer[irow][x];
					*pCurRow++ = xPixel;
					*pCurRow++ = xPixel;
					*pCurRow++ = xPixel;
				}
				y++;
			}
		}
	}

	/* Step 7: Finish decompression */
	(void) jpeg_finish_decompress(&cinfo);
 
	/* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

	/* Step 8: Release JPEG decompression object */

	/* This is an important step since it will release a good deal of memory. */   
	//jpeg_destroy_decompress(&cinfo);

	/* After finish_decompress, we can close the input file.
	* Here we postpone it until after no more JPEG errors are possible,
	* so as to simplify the setjmp error logic above.  (Actually, I don't
	* think that jpeg_destroy can do an error exit, but why assume anything...)
	*/

	/* At this point you may want to check to see whether any corrupt-data
	* warnings occurred (test whether jerr.pub.num_warnings is nonzero).
	*/

	/* And we're done! */


	abort:
	jpeg_destroy_decompress(&cinfo);
	if (inputdata)
	   delete [] inputdata;
	return hbm;
}
