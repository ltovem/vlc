/*****************************************************************************
 * css_bridge.h : CSS grammar/lexer bridge
 *****************************************************************************
 * Copyright (C) 2017 VideoLabs, VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#define yyconst const
#define YYSTYPE CSSSTYPE
typedef void* yyscan_t;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
int csslex_init (yyscan_t* scanner);
YY_BUFFER_STATE css_scan_string (yyconst char *yy_str ,yyscan_t yyscanner );
YY_BUFFER_STATE css_scan_bytes (yyconst char *bytes, int, yyscan_t yyscanner );
int csslex_destroy (yyscan_t yyscanner );
void css_delete_buffer (YY_BUFFER_STATE  b , yyscan_t yyscanner);
