#include "mruby.h"
#include <stddef.h>
#include <string.h>

/*Only to check if directory exists*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/hash.h"
#include "mruby/string.h"
#include "mruby/variable.h"

#if MRUBY_RELEASE_NO < 10000
  #include "error.h"
#else
  #include "mruby/error.h"
#endif

#if defined(__GNUC__)
  // Ensure we get the 64-bit variants of the CRT's file I/O calls
  #ifndef _FILE_OFFSET_BITS
    #define _FILE_OFFSET_BITS 64
  #endif
  #ifndef _LARGEFILE64_SOURCE
    #define _LARGEFILE64_SOURCE 0
  #endif
#endif

#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;

#define BUF_SIZE (1024 * 1024)
static uint8 s_inbuf[BUF_SIZE];
/*static uint8 s_outbuf[BUF_SIZE];*/

static const char *s_pComment = "Comment";

/*TODO Scalone: Exception*/
/*TODO Scalone: check file_loc + 1 and - 1*/
/*TODO Scalone: Support directories*/
static mrb_value
mrb_miniz_zip(mrb_state *mrb, mrb_value klass)
{
  mrb_value *argv, filezip;
  int argc, i;
  mz_bool status;
  FILE *pInfile;
  long file_loc;

  /*printf("miniz.c version: %s\n", MZ_VERSION);*/

  mrb_get_args(mrb, "S|*", &filezip, &argv, &argc);

  // Delete the test archive, so it doesn't keep growing as we run this test
  remove(RSTRING_PTR(filezip));

  for (i=0; i<argc; i++)
  {
    if (mrb_string_p(argv[i]))
    {
      memset(&s_inbuf, 0, sizeof(s_inbuf));

      /*printf("filename %s\n", RSTRING_PTR(argv[i]));*/

      pInfile = fopen(RSTRING_PTR(argv[i]), "rb");
      if (!pInfile)
      {
        printf("Failed opening input file!\n");
        return mrb_false_value();
      }

      fseek(pInfile, 0, SEEK_END);
      file_loc = ftell(pInfile);
      fseek(pInfile, 0, SEEK_SET);

      if (fread(s_inbuf, 1, file_loc, pInfile) != file_loc)
      {
        printf("Failed reading from input file!\n");
        return mrb_false_value();
      }

      status = mz_zip_add_mem_to_archive_file_in_place(RSTRING_PTR(filezip), RSTRING_PTR(argv[i]), s_inbuf, file_loc + 1, s_pComment, (uint16)strlen(s_pComment), MZ_BEST_COMPRESSION);
      if (!status)
      {
        printf("mz_zip_add_mem_to_archive_file_in_place failed!\n");
        return mrb_false_value();
      }
    } else {
      printf("Filename is not a string!\n");
      return mrb_false_value();
    }
  }

  return mrb_true_value();
}

static mrb_value
mrb_miniz_unzip(mrb_state *mrb, mrb_value klass)
{
  mrb_value filezip, dir;
  char path[50]={0x00};
  struct stat st = {0};
  int i;
  mz_bool status;
  size_t uncomp_size;
  mz_zip_archive zip_archive;
  void *p;
  int n=0;
  /*static const char *s_Test_archive_filename = "test.zip";*/
  FILE *pOutfile;

  /*printf("miniz.c version: %s\n", MZ_VERSION);*/

  mrb_get_args(mrb, "SS", &filezip, &dir);

  // Now try to open the archive.
  memset(&zip_archive, 0, sizeof(zip_archive));

  if (! mrb_string_p(filezip))
  {
    printf("Filename is not a string!\n");
    return mrb_false_value();
  }

  if (mrb_string_p(dir))
  {
    if (stat(RSTRING_PTR(dir), &st) == -1) {
      printf("Directory do not exist!\n");
      return mrb_false_value();
    }
  } else {
    printf("Dirname is not a string!\n");
    return mrb_false_value();
  }

  status = mz_zip_reader_init_file(&zip_archive, RSTRING_PTR(filezip), 0);
  if (!status)
  {
    printf("mz_zip_reader_init_file() failed!\n");
    return mrb_false_value();
  }

  n = (int)mz_zip_reader_get_num_files(&zip_archive);

  mz_zip_reader_end(&zip_archive);

  // Get and print information about each file in the archive.
  for (i = 0; i < n; i++)
  {
    memset(&zip_archive, 0, sizeof(zip_archive));
    status = mz_zip_reader_init_file(&zip_archive, RSTRING_PTR(filezip), 0);

    mz_zip_archive_file_stat file_stat;
    if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
    {
      printf("mz_zip_reader_file_stat() failed!\n");
      mz_zip_reader_end(&zip_archive);
      return mrb_false_value();
    }

    /*printf("Filename: \"%s\", Comment: \"%s\", Uncompressed size: %u, Compressed size: %u, Is Dir: %u\n", file_stat.m_filename, file_stat.m_comment, (uint)file_stat.m_uncomp_size, (uint)file_stat.m_comp_size, mz_zip_reader_is_file_a_directory(&zip_archive, i));*/

    mz_zip_reader_end(&zip_archive);

    memset(&zip_archive, 0, sizeof(zip_archive));
    status = mz_zip_reader_init_file(&zip_archive, RSTRING_PTR(filezip), 0);

    /*TODO Scalone: Check if directory*/
    /*status = mz_zip_reader_init_file(&zip_archive, RSTRING_PTR(filezip), sort_iter ? MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY : 0);*/

    if (!status)
    {
      printf("mz_zip_reader_init_file() failed!\n");
      return mrb_false_value();
    }

    p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &uncomp_size, 0);
    if (!p) {
      printf("mz_zip_reader_extract_file_to_heap(%d) failed!\n", (int)p);
      mz_zip_reader_end(&zip_archive);
      return mrb_false_value();
    }

    // Make sure the extraction really succeeded.
    if (uncomp_size > 0)
    {
      sprintf(path, "%s/%s", RSTRING_PTR(dir), file_stat.m_filename);
      pOutfile = fopen(path, "wb");
      if (!pOutfile)
      {
        printf("Failed opening output file!\n");
        return mrb_false_value();
      }

      if (fwrite(p, 1, uncomp_size, pOutfile) != uncomp_size)
      {
        printf("Failed writing to output file!\n");
        return mrb_false_value();
      }

      if (EOF == fclose(pOutfile))
      {
        printf("Failed writing to output file!\n");
        return mrb_false_value();
      }
    } else {
      printf("mz_zip_reader_extract_file_to_heap() failed to extract the proper data\n");
      mz_free(p);
      mz_zip_reader_end(&zip_archive);
      return mrb_false_value();
    }

    /*printf("Successfully extracted file \"%s\", size %u\n", file_stat.m_filename, (uint)uncomp_size);*/
    /*printf("File data: \"%s\"\n", (const char *)p);*/

    /*We're done.*/
    mz_free(p);
  }

  /*Close the archive, freeing any resources it was using*/
  mz_zip_reader_end(&zip_archive);

  return mrb_true_value();
}

static mrb_value
mrb_miniz_inflate(mrb_state *mrb, mrb_value klass)
{
  mrb_int iRet=0;
  mrb_value string, value;
  unsigned char *pDest;
  unsigned long ulDest=0;

  mrb_get_args(mrb, "S", &string);

  ulDest = RSTRING_LEN(string) * 100;
  pDest  = (unsigned char *) mrb_malloc(mrb, ulDest);
  memset(pDest, 0, ulDest);

  iRet = mz_uncompress(pDest, &ulDest, (const unsigned char *)RSTRING_PTR(string), ulDest);

  if (iRet == 0)
    value = mrb_str_new(mrb, (const char *)pDest, ulDest);
  else
    value = mrb_fixnum_value(iRet);

  mrb_free(mrb, pDest);
  return value;
}

static mrb_value
mrb_miniz_deflate(mrb_state *mrb, mrb_value klass)
{
  mrb_int iRet=0;
  mrb_value string, value;
  unsigned char *pDest;
  unsigned long ulDest=0;

  mrb_get_args(mrb, "S", &string);

  ulDest = RSTRING_LEN(string);
  pDest = (unsigned char *) mrb_malloc(mrb, ulDest);
  memset(pDest, 0, ulDest);

  iRet = mz_compress(pDest, &ulDest, (const unsigned char *)RSTRING_PTR(string), ulDest);

  if (iRet == 0)
    value = mrb_str_new(mrb, (const char *)pDest, ulDest);
  else
    value = mrb_fixnum_value(iRet);

  mrb_free(mrb, pDest);
  return value;
}

void
mrb_init_miniz(mrb_state* mrb)
{
  struct RClass *miniz;

  miniz = mrb_define_class(mrb, "Miniz", mrb->object_class);

  mrb_define_class_method(mrb, miniz, "zip", mrb_miniz_zip, MRB_ARGS_ANY());
  mrb_define_class_method(mrb, miniz, "_unzip", mrb_miniz_unzip, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, miniz, "inflate", mrb_miniz_inflate, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, miniz, "deflate", mrb_miniz_deflate, MRB_ARGS_REQ(1));
}

