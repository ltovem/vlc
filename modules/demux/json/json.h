/*****************************************************************************
 * json/json.h:
 *****************************************************************************
 * Copyright © 2020 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef JSON_H
#define JSON_H

#include <stdbool.h>
#include <stdio.h>

#define YYSTYPE JSONSTYPE

enum json_type {
    JSON_NULL,
    JSON_BOOLEAN,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT,
};

struct json_array {
    size_t size;
    struct json_value *entries;
};

struct json_object {
    size_t count;
    struct json_member *members;
};

struct json_value {
    enum json_type type;
    union {
        bool boolean;
        double number;
        char *string;
        struct json_array array;
        struct json_object object;
    };
};

struct json_member {
    char *name;
    struct json_value value;
};

size_t json_read(void *data, void *buf, size_t max);
void json_parse_error(void *log, const char *msg);
char *json_unescape(const char *, size_t);

int json_parse(void *log, struct json_object *result);
void json_free(struct json_object *);

const struct json_value *json_get(const struct json_object *obj,
                                  const char *name);
const char *json_get_str(const struct json_object *obj, const char *name);
double json_get_num(const struct json_object *obj, const char *name);

#endif
