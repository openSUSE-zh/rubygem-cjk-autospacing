/*
gcc ./cjkpad.c -o cjkpad `pkg-config --libs --cflags icu-uc icu-io`
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unicode/utext.h>
/*#include <unicode/ustdio.h>*/
#include <ruby.h>
#include "extconf.h"

static UErrorCode err = U_ZERO_ERROR;

UBool u_iscjk(int ublock);

typedef struct gra {
  UChar32 cp; // code point
  UBlockCode block; // unicode block
  size_t start; // starting pos at the str[]
  size_t length; // number of char needed (cjk is mostly 3 char)
  bool cjk;
} gra;

UBool u_isMarkdown(UChar32 cp, bool prefix);

char* padCjk1(const char* str) {
    size_t size = 1024;
    char* formatted = calloc(size, sizeof(char));
    int count = 0;
    int begin = 0;
    int end, lastCjk, lastNonCjk;
    UChar32 prev_cp, lastCjkPlusCp, lastNonCjkPlusCp;
    UText* ut = utext_openUTF8(NULL, str, -1, &err);

    for (UChar32 cp = utext_next32From(ut, 0); cp > -1; cp = utext_next32(ut)) {
      end = utext_getNativeIndex(ut);
      int curr_blk = ublock_getCode(cp);
      UBool cjk = u_iscjk(curr_blk);
      UBool nonCjk = !(cjk || u_isspace(cp) || u_isMarkdown(cp, true));
      int length = end - begin;
      if (lastNonCjk == begin) {
	lastNonCjkPlusCp = cp;
      }
      if (lastCjk == begin) {
	lastCjkPlusCp = cp;
      }

      if (begin > 0) {
        if ((cjk && begin == lastNonCjk) || (nonCjk && begin == lastCjk)) {
   count++;
   if (count > size-1) {
      size = 2 * size;
      formatted = realloc(formatted, size * sizeof(char));
   }
   strcat(formatted, " ");
}

/*你好world_*哈哈哈*_hello
你好_*hello*_哈哈哈world
你好world哈哈哈*/

if (cjk && begin > lastCjk && u_isMarkdown(lastCjkPlusCp, true) && u_isMarkdown(prev_cp, true)) {
   count += 2;
   if (count > size-1) {
      size = 2*size;
      formatted = realloc(formatted, size * sizeof(char));
   }
   char* temp = calloc(begin-lastCjk+3, sizeof(char));
   temp[0] = ' ';
   int i, j;
   for (i = lastCjk, j = 1; i < begin; i++, j++) {
     temp[j] = formatted[i];
   }

   if (!u_isspace(formatted[begin])) {
     temp[j] = ' ';
   } else {
     count--;
   }

   for (i = 0, j = lastCjk; i < strlen(temp) ; i++, j++) {
      formatted[j] = temp[i];
   }
   free(temp);
}

/*你好world_*哈哈哈*_hello
你好_*hello*_哈哈哈world
你好world哈哈哈*/

if (nonCjk && begin > lastNonCjk && u_isMarkdown(lastNonCjkPlusCp, true) && u_isMarkdown(prev_cp, true)) {
  count += 2;
  if (count > size-1) {
    size = 2*size;
    formatted = realloc(formatted, size * sizeof(char));
  }
  char* temp = calloc(begin-lastNonCjk+3, sizeof(char));
   temp[0] = ' ';
   int i, j;
   for (i = lastNonCjk, j = 1; i < begin; i++, j++) {
     temp[j] = formatted[i];
   }

   if (!u_isspace(formatted[begin])) {
     temp[j] = ' ';
   } else {
     count--;
   }

   for (i = 0, j = lastNonCjk; i < strlen(temp); i++, j++) {
      formatted[j] = temp[i];
   }
   free(temp);
}	
      }

      count += length;
      if (count > size-1) {
        formatted = realloc(formatted, size * sizeof(char));
      }
      strncat(formatted, &str[begin], length);

      begin = end;
      if (cjk) {
        lastCjk = end;
      }
      if (nonCjk) {
	lastNonCjk = end;
      }
      prev_cp = cp;
    }

    return formatted;
}

char* padCjk(const char* str) {
    char* formatted = calloc(1024, sizeof(char));

    gra* graphemes = malloc(1024 * sizeof(gra));
    int graLen; // number of graphemes

    UText *ut = utext_openUTF8(NULL, str, -1, &err);

    int begin = 0;
    int end;
    int i = 0;
    int blk;
    int filled = 0;

    for (UChar32 cp = utext_next32From(ut, 0);
         cp > -1;
         cp = utext_next32(ut), i++) {
      end = utext_getNativeIndex(ut);
      graphemes[i].cp = cp;
      blk = ublock_getCode(cp);
      graphemes[i].block = blk;
      graphemes[i].start = begin;
      graphemes[i].length = end - begin;
      graphemes[i].cjk = u_iscjk(blk);

      begin = end;
    }

    graLen = i;

    filled += graphemes[0].length;
    strncat(formatted, &str[graphemes[0].start], graphemes[0].length);

    for (int j = 1; j <= graLen; j++) {
      gra *prev = &graphemes[j-1];
      gra *curr = &graphemes[j];
      gra *next = &graphemes[j+1];

      //u_printf("%C%C%C\n", prev->cp, curr->cp, next->cp);

      if ((u_isMarkdown(curr->cp, true) && !prev->cjk && !u_isspace(prev->cp) && next->cjk) || // d*哈
         (!prev->cjk && !u_isspace(prev->cp) && !u_isMarkdown(prev->cp, true) && curr->cjk) || // *哈
         (!curr->cjk && !u_isspace(curr->cp) && !u_isMarkdown(curr->cp, false) && prev->cjk)) { // 哈*
        filled++;
        if (filled == strlen(formatted)-1) {
          formatted = realloc(formatted, strlen(formatted) * 2 * sizeof(char));
        }
        strcat(formatted, " ");
      }
      filled += curr->length;
      if (filled == strlen(formatted)-1) {
        formatted = realloc(formatted, strlen(formatted) * 2 * sizeof(char));
      }
      strncat(formatted, &str[curr->start], curr->length);

      if (u_isMarkdown(curr->cp, false) && prev->cjk && !u_isspace(next->cp) && !next->cjk) { // 哈*n
        filled++;
        if (filled == strlen(formatted)-1) {
          formatted = realloc(formatted, strlen(formatted) * 2 * sizeof(char));
        }
        strcat(formatted, " ");
      }
    }

    free(graphemes);

    return formatted;
}

UBool u_isMarkdown(UChar32 cp, bool prefix) {
  UBool markdown;

  switch (cp) {
  case 0x005F: // _
  case 0x002A: // *
  case 0x0060: // `
    markdown = 1;
    break;
  case 0x003E:
  case 0x005B:
    if (prefix) {
      markdown = 1;
      break;
    }
  case 0x005D:
    if (!prefix) {
      markdown = 1;
      break;
    }
  default:
    markdown = 0;
    break;
  };
  return markdown;
}

UBool u_iscjk(int ublock) {
  UBool cjk;

  switch (ublock) {
  case UBLOCK_CJK_UNIFIED_IDEOGRAPHS:
  case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A:
  case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B:
  case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C:
  case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D:
  case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E:
  case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F:
  case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G:
  case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_H:
  case UBLOCK_CJK_RADICALS_SUPPLEMENT:
  case UBLOCK_CJK_SYMBOLS_AND_PUNCTUATION:
  case UBLOCK_HIRAGANA:
  case UBLOCK_KATAKANA:
  case UBLOCK_BOPOMOFO:
  case UBLOCK_BOPOMOFO_EXTENDED:
  case UBLOCK_KANBUN:
  case UBLOCK_CJK_STROKES:
  case UBLOCK_KATAKANA_PHONETIC_EXTENSIONS:
  case UBLOCK_ENCLOSED_CJK_LETTERS_AND_MONTHS:
  case UBLOCK_CJK_COMPATIBILITY:
  case UBLOCK_HANGUL_JAMO:
  case UBLOCK_HANGUL_COMPATIBILITY_JAMO:
  case UBLOCK_HANGUL_JAMO_EXTENDED_A:
  case UBLOCK_HANGUL_SYLLABLES:
  case UBLOCK_HANGUL_JAMO_EXTENDED_B:
  case UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS:
  case UBLOCK_CJK_COMPATIBILITY_FORMS:
  case UBLOCK_KANA_EXTENDED_B:
  case UBLOCK_KANA_SUPPLEMENT:
  case UBLOCK_KANA_EXTENDED_A:
  case UBLOCK_SMALL_KANA_EXTENSION:
    cjk = 1;
    break;
  default:
    cjk = 0;
    break;
  };
  return cjk;
}

static VALUE rb_pad_cjk1(VALUE self) {
  Check_Type(self, T_STRING);

  char* in = StringValueCStr(self);
  char* out = padCjk1(in);
  return rb_str_new_cstr(out);
}

static VALUE rb_pad_cjk(VALUE self) {
  Check_Type(self, T_STRING);

  char* in = StringValueCStr(self);
  char* out = padCjk(in);
  return rb_str_new_cstr(out);
}

void Init_cjk_auto_space(void) {
  rb_define_method(rb_cString, "cjk_auto_space", rb_pad_cjk, 0);
  rb_define_method(rb_cString, "cjk_auto_space1", rb_pad_cjk1, 0);
}
