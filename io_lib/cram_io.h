#ifndef _CRAM_IO_H_
#define _CRAM_IO_H_

/*
 * Implements the low level CRAM I/O primitives.
 * This includes basic data types such as byte, int, ITF-8,
 * maps, bitwise I/O, etc.
 */

#define ITF8_MACROS

#include <stdint.h>
#include <io_lib/misc.h>

/* ----------------------------------------------------------------------
 * ITF8 encoding and decoding.
 *
 * Also see the itf8_get and itf8_put macros.
 */

#define CRAM_KEY(a,b) (((a)<<8)|((b)))

/*
 * Reads an integer in ITF-8 encoding from 'cp' and stores it in
 * *val.
 *
 * Returns the number of bytes read on success
 *        -1 on failure
 */
int itf8_decode(cram_fd *fd, int32_t *val);

#ifndef ITF8_MACROS
/*
 * As above, but decoding from memory
 */
int itf8_get(char *cp, int32_t *val_p);

/*
 * Stores a value to memory in ITF-8 format.
 *
 * Returns the number of bytes required to store the number.
 * This is a maximum of 5 bytes.
 */
int itf8_put(char *cp, int32_t val);

#else

/*
 * Macro implementations of the above
 */
#define itf8_get(c,v) (((uc)(c)[0]<0x80)?(*(v)=(uc)(c)[0],1):(((uc)(c)[0]<0xc0)?(*(v)=(((uc)(c)[0]<<8)|(uc)(c)[1])&0x3fff,2):(((uc)(c)[0]<0xe0)?(*(v)=(((uc)(c)[0]<<16)|((uc)(c)[1]<<8)|(uc)(c)[2])&0x1fffff,3):(((uc)(c)[0]<0xf0)?(*(v)=(((uc)(c)[0]<<24)|((uc)(c)[1]<<16)|((uc)(c)[2]<<8)|(uc)(c)[3])&0x0fffffff,4):(*(v)=(((uc)(c)[0]&0x0f)<<28)|((uc)(c)[1]<<20)|((uc)(c)[2]<<12)|((uc)(c)[3]<<4)|((uc)(c)[4]&0x0f),5)))))

#define itf8_put(c,v) ((!((v)&~0x7f))?((c)[0]=(v),1):(!((v)&~0x3fff))?((c)[0]=((v)>>8)|0x80,(c)[1]=(v)&0xff,2):(!((v)&~0x1fffff))?((c)[0]=((v)>>16)|0xc0,(c)[1]=((v)>>8)&0xff,(c)[2]=(v)&0xff,3):(!((v)&~0xfffffff))?((c)[0]=((v)>>24)|0xe0,(c)[1]=((v)>>16)&0xff,(c)[2]=((v)>>8)&0xff,(c)[3]=(v)&0xff,4):((c)[0]=0xf0|(((v)>>28)&0xff),(c)[1]=((v)>>20)&0xff,(c)[2]=((v)>>12)&0xff,(c)[3]=((v)>>4)&0xff,(c)[4]=(v)&0xf,5))

#endif

/*
 * Pushes a value in ITF8 format onto the end of a block.
 * This shouldn't be used for high-volume data as it is not the fastest
 * method.
 *
 * Returns the number of bytes written
 */
int itf8_put_blk(cram_block *blk, int val);


/* ----------------------------------------------------------------------
 * CRAM blocks - the dynamically growable data block. We have code to
 * create, update, (un)compress and read/write.
 *
 * These are derived from the deflate_interlaced.c blocks, but with the
 * CRAM extension of content types and IDs.
 */

/*
 * Allocates a new cram_block structure with a specified content_type and
 * id.
 *
 * Returns block pointer on success
 *         NULL on failure
 */
cram_block *cram_new_block(enum cram_content_type content_type,
			   int content_id);

/*
 * Reads a block from a cram file.
 * Returns cram_block pointer on success.
 *         NULL on failure
 */
cram_block *cram_read_block(cram_fd *fd);

/*
 * Writes a CRAM block.
 * Returns 0 on success
 *        -1 on failure
 */
int cram_write_block(cram_fd *fd, cram_block *b);

/*
 * Frees a CRAM block, deallocating internal data too.
 */
void cram_free_block(cram_block *b);

/*
 * Uncompresses a CRAM block, if compressed.
 */
void cram_uncompress_block(cram_block *b);

/*
 * Compresses a block using one of two different zlib strategies. If we only
 * want one choice set strat2 to be -1.
 *
 * The logic here is that sometimes Z_RLE does a better job than Z_FILTERED
 * or Z_DEFAULT_STRATEGY on quality data. If so, we'd rather use it as it is
 * significantly faster.
 */
void cram_compress_block(cram_fd *fd, cram_block *b, cram_metrics *metrics,
			 int level,  int strat,
			 int level2, int strat2);

cram_metrics *cram_new_metrics(void);
char *cram_block_method2str(enum cram_block_method m);
char *cram_content_type2str(enum cram_content_type t);

/* --- Accessor macros for manipulating blocks on a byte by byte basis --- */

/* Block size and data pointer. */
#define BLOCK_SIZE(b) ((b)->byte)
#define BLOCK_DATA(b) ((b)->data)

/* Returns the address one past the end of the block */
#define BLOCK_END(b) (&(b)->data[(b)->byte])

/* Request block to be at least 'l' bytes long */
#define BLOCK_RESIZE(b,l)					\
    do {							\
	while((b)->alloc <= (l)) {				\
	    (b)->alloc = (b)->alloc ? (b)->alloc*2 : 1024;	\
	    (b)->data = realloc((b)->data, (b)->alloc);		\
	}							\
     } while(0)

/* Ensure the block can hold at least another 'l' bytes */
#define BLOCK_GROW(b,l) BLOCK_RESIZE((b), BLOCK_SIZE((b)) + (l))

/* Append string 's' of length 'l' */
#define BLOCK_APPEND(b,s,l)		  \
    do {				  \
        BLOCK_GROW((b),(l));		  \
        memcpy(BLOCK_END((b)), (s), (l)); \
	BLOCK_SIZE((b)) += (l);		  \
    } while (0)

/* Append as single character 'c' */
#define BLOCK_APPEND_CHAR(b,c)		  \
    do {				  \
        BLOCK_GROW((b),1);		  \
	(b)->data[(b)->byte++] = (c);	  \
    } while (0)

/* Append via sprintf with 1 arg */
#define BLOCK_APPENDF_1(b,buf,fmt, a1)			\
    do {						\
	int l = sprintf((buf), (fmt), (a1));		\
	BLOCK_APPEND((b), (buf), l);			\
    } while (0)

/* Append via sprintf with 2 args */
#define BLOCK_APPENDF_2(b,buf,fmt, a1,a2)		\
    do {						\
	int l = sprintf((buf), (fmt), (a1), (a2));	\
	BLOCK_APPEND((b), (buf), l);			\
    } while (0)

#define BLOCK_UPLEN(b) \
    (b)->comp_size = (b)->uncomp_size = BLOCK_SIZE((b))

/* ----------------------------------------------------------------------
 * Reference sequence handling
 */

void cram_load_reference(cram_fd *fd, char *fn);

/*
 * Indexes references by the order they appear in a BAM file. This may not
 * necessarily be the same order they appear in the fasta reference file.
 */
void refs2id(refs *r, bam_file_t *bfd);

/*
 * Returns a portion of a reference sequence from start to end inclusive.
 * The returned pointer is owned by the cram_file fd and should not be freed
 * by the caller. It is valid only until the next cram_get_ref is called
 * with the same fd parameter (so is thread-safe if given multiple files).
 *
 * To return the entire reference sequence, specify start as 1 and end
 * as 0.
 *
 * Returns reference on success
 *         NULL on failure
 */
char *cram_get_ref(cram_fd *fd, int id, int start, int end);


/* ----------------------------------------------------------------------
 * Containers
 */

/*
 * Creates a new container, specifying the maximum number of slices
 * and records permitted.
 *
 * Returns cram_container ptr on success
 *         NULL on failure
 */
cram_container *cram_new_container(int nrec, int nslice);
void cram_free_container(cram_container *c);

/*
 * Reads a container header.
 * Returns cram_container on success
 *         NULL on failure or no container left (fd->err == 0).
 */
cram_container *cram_read_container(cram_fd *fd);

/*
 * Writes a container structure.
 *
 * Returns 0 on success
 *        -1 on failure
 */
int cram_write_container(cram_fd *fd, cram_container *h);

/*
 * Flushes a completely or partially full container to disk, writing
 * container structure, header and blocks. This also calls the encoder
 * functions.
 *
 * Returns 0 on success
 *        -1 on failure
 */
int cram_flush_container(cram_fd *fd, cram_container *c);


/* ----------------------------------------------------------------------
 * Compression headers; the first part of the container
 */

/*
 * Creates a new blank container compression header
 *
 * Returns header ptr on success
 *         NULL on failure
 */
cram_block_compression_hdr *cram_new_compression_header(void);
void cram_free_compression_header(cram_block_compression_hdr *hdr);


/* ----------------------------------------------------------------------
 * Slices and slice headers
 */

void cram_free_slice_header(cram_block_slice_hdr *hdr);
void cram_free_slice(cram_slice *s);

/*
 * Creates a new empty slice in memory, for subsequent writing to
 * disk.
 *
 * Returns cram_slice ptr on success
 *         NULL on failure
 */
cram_slice *cram_new_slice(enum cram_content_type type, int nrecs);

/*
 * Loads an entire slice.
 * FIXME: In 1.0 the native unit of slices within CRAM is broken
 * as slices contain references to objects in other slices.
 * To work around this while keeping the slice oriented outer loop
 * we read all slices and stitch them together into a fake large
 * slice instead.
 *
 * Returns cram_slice ptr on success
 *         NULL on failure
 */
cram_slice *cram_read_slice(cram_fd *fd);



/* ----------------------------------------------------------------------
 * CRAM file definition (header)
 */

/*
 * Reads a CRAM file definition structure.
 * Returns file_def ptr on success
 *         NULL on failure
 */
cram_file_def *cram_read_file_def(cram_fd *fd);
/*
 * Writes a cram_file_def structure to cram_fd.
 * Returns 0 on success
 *        -1 on failure
 */
int cram_write_file_def(cram_fd *fd, cram_file_def *def);

void cram_free_file_def(cram_file_def *def);


/* ----------------------------------------------------------------------
 * SAM header I/O
 */

/*
 * Creates a CRAM header from a SAM header in string format.
 *
 * FIXME: consider either rejecting this completely and using
 * "char *SAM_hdr" throughout, or instead finishing this off by copying
 * the bam_parse_header() code into here.
 *
 * FIXME 2: check consistency of header. Needs SQ:MD5, HD:SO as POS,
 * RG lines, etc.
 *
 * Returns cram_SAM_hdr* on success
 *         NULL on failure
 */
cram_SAM_hdr *cram_new_SAM_hdr(char *str, size_t len);
void cram_free_SAM_hdr(cram_SAM_hdr *hdr);

/*
 * Reads the SAM header from the first CRAM data block.
 * Also performs minimal parsing to extract read-group
 * and sample information.

 * Returns SAM hdr ptr on success
 *         NULL on failure
 */
cram_SAM_hdr *cram_read_SAM_hdr(cram_fd *fd);

/*
 * Writes a CRAM SAM header.
 * Returns 0 on success
 *        -1 on failure
 */
int cram_write_SAM_hdr(cram_fd *fd, cram_SAM_hdr *hdr);


/* ----------------------------------------------------------------------
 * The top-level cram opening, closing and option handling
 */

/*
 * Opens a CRAM file for read (mode "rb") or write ("wb").
 * The filename may be "-" to indicate stdin or stdout.
 *
 * Returns file handle on success
 *         NULL on failure.
 */
cram_fd *cram_open(char *filename, char *mode);

/*
 * Closes a CRAM file.
 * Returns 0 on success
 *        -1 on failure
 */
int cram_close(cram_fd *fd);

/*
 * Returns 1 if we hit an EOF while reading.
 */
int cram_eof(cram_fd *fd);

/* 
 * Sets options on the cram_fd. See CRAM_OPT_* definitions in cram_structs.h.
 * Use this immediately after opening.
 *
 * Returns 0 on success
 *        -1 on failure
 */
int cram_set_option(cram_fd *fd, enum cram_option opt, cram_opt *val);

#endif /* _CRAM_IO_H_ */
