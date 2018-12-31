typedef enum {
    FORMAT_ARGUMENT_TYPE_NULL,
    FORMAT_ARGUMENT_TYPE_CHAR,
    FORMAT_ARGUMENT_TYPE_UCHAR,
    FORMAT_ARGUMENT_TYPE_SHORT,
    FORMAT_ARGUMENT_TYPE_USHORT,
    FORMAT_ARGUMENT_TYPE_INT,
    FORMAT_ARGUMENT_TYPE_UINT,
    FORMAT_ARGUMENT_TYPE_LONG,
    FORMAT_ARGUMENT_TYPE_ULONG,
    FORMAT_ARGUMENT_TYPE_U8,
    FORMAT_ARGUMENT_TYPE_U16,
    FORMAT_ARGUMENT_TYPE_U32,
    FORMAT_ARGUMENT_TYPE_U64,
    FORMAT_ARGUMENT_TYPE_I8,
    FORMAT_ARGUMENT_TYPE_I16,
    FORMAT_ARGUMENT_TYPE_I32,
    FORMAT_ARGUMENT_TYPE_I64,
    FORMAT_ARGUMENT_TYPE_POINTER,
    FORMAT_ARGUMENT_TYPE_CSTRING,
    FORMAT_ARGUMENT_TYPE_STRING,
} FormatArgumentType;

typedef union {
    i64    i;
    void*  p;
    String s;
} FormatArgument;

static
void pop_format_argument(FormatArgument* arg, FormatArgumentType type,
                         VariableArgumentList* va_list) {
    switch (type) {
    case FORMAT_ARGUMENT_TYPE_CHAR:    arg->i = (char  )variable_argument_get(*va_list, int   ); break;
    case FORMAT_ARGUMENT_TYPE_UCHAR:   arg->i = (uchar )variable_argument_get(*va_list, uint  ); break;
    case FORMAT_ARGUMENT_TYPE_SHORT:   arg->i = (short )variable_argument_get(*va_list, int   ); break;
    case FORMAT_ARGUMENT_TYPE_USHORT:  arg->i = (ushort)variable_argument_get(*va_list, uint  ); break;
    case FORMAT_ARGUMENT_TYPE_INT:     arg->i = (int   )variable_argument_get(*va_list, int   ); break;
    case FORMAT_ARGUMENT_TYPE_UINT:    arg->i = (uint  )variable_argument_get(*va_list, uint  ); break;
    case FORMAT_ARGUMENT_TYPE_LONG:    arg->i = (long  )variable_argument_get(*va_list, long  ); break;
    case FORMAT_ARGUMENT_TYPE_ULONG:   arg->i = (ulong )variable_argument_get(*va_list, ulong ); break;
    case FORMAT_ARGUMENT_TYPE_U8:      arg->i = (u8    )variable_argument_get(*va_list, uint  ); break;
    case FORMAT_ARGUMENT_TYPE_U16:     arg->i = (u16   )variable_argument_get(*va_list, uint  ); break;
    case FORMAT_ARGUMENT_TYPE_U32:     arg->i = (u32   )variable_argument_get(*va_list, u32   ); break;
    case FORMAT_ARGUMENT_TYPE_U64:     arg->i = (u64   )variable_argument_get(*va_list, u64   ); break;
    case FORMAT_ARGUMENT_TYPE_I8:      arg->i = (i8    )variable_argument_get(*va_list, int   ); break;
    case FORMAT_ARGUMENT_TYPE_I16:     arg->i = (i16   )variable_argument_get(*va_list, int   ); break;
    case FORMAT_ARGUMENT_TYPE_I32:     arg->i = (i32   )variable_argument_get(*va_list, i32   ); break;
    case FORMAT_ARGUMENT_TYPE_I64:     arg->i = (i64   )variable_argument_get(*va_list, i64   ); break;
    case FORMAT_ARGUMENT_TYPE_POINTER: arg->p = (void* )variable_argument_get(*va_list, void* ); break;
    case FORMAT_ARGUMENT_TYPE_CSTRING: arg->p = (void* )variable_argument_get(*va_list, char* ); break;
    case FORMAT_ARGUMENT_TYPE_STRING:  arg->s = (String)variable_argument_get(*va_list, String); break;
    }
}

static
void get_format_argument_at_index(FormatArgument* arg, FormatArgumentType* types,
                                  VariableArgumentList* va_list, int index) {
    FormatArgument dummy;
    for(int i = 0; i < index - 1; i++) {
        pop_format_argument(&dummy, types[i], va_list);
    }
    pop_format_argument(arg, types[index - 1], va_list);
}

static
int format_string_compare_(char* const restrict fp, String s) {
    for(int i = 0; i < s.length; i++) {
        if(s.data[i] != fp[i]) {
            return 0;
        }
    }
    return 1;
}

static
char* format_string_list(String string, char* const format, va_list arg_list) {
    char* sp = string.data;
    char* fp = format;

    int arg_pos = 0;
    
    FormatArgumentType types[16] = { };
    
    while(fp) {
        if(*fp == '{') {
            fp++;
            if(*fp == '{') {
                continue;
            }

            if('0' <= *fp && *fp <= '9') {
                arg_pos = 0;
                while('0' <= *fp && *fp <= '9') {
                    arg_pos = 10 * arg_pos + *fp - '0';
                    fp++;
                }
            } else {
                arg_pos++;
            }

            types[arg_pos] = FORMAT_ARGUMENT_TYPE_NULL;
            if(format_string_compare_(fp, L("c")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_CHAR;
            if(format_string_compare_(fp, L("u")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_UINT;
            if(format_string_compare_(fp, L("i")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_INT;
            if(format_string_compare_(fp, L("p")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_POINTER;
            if(format_string_compare_(fp, L("s")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_CSTRING;
            if(format_string_compare_(fp, L("S")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_STRING;
            if(format_string_compare_(fp, L("char")))    types[arg_pos] = FORMAT_ARGUMENT_TYPE_CHAR;
            if(format_string_compare_(fp, L("short")))   types[arg_pos] = FORMAT_ARGUMENT_TYPE_SHORT;
            if(format_string_compare_(fp, L("ushort")))  types[arg_pos] = FORMAT_ARGUMENT_TYPE_USHORT;
            if(format_string_compare_(fp, L("int")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_INT;
            if(format_string_compare_(fp, L("uint")))    types[arg_pos] = FORMAT_ARGUMENT_TYPE_UINT;
            if(format_string_compare_(fp, L("long")))    types[arg_pos] = FORMAT_ARGUMENT_TYPE_LONG;
            if(format_string_compare_(fp, L("ulong")))   types[arg_pos] = FORMAT_ARGUMENT_TYPE_ULONG;
            if(format_string_compare_(fp, L("u8")))      types[arg_pos] = FORMAT_ARGUMENT_TYPE_U8;
            if(format_string_compare_(fp, L("u16")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_U16;
            if(format_string_compare_(fp, L("u32")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_U32;
            if(format_string_compare_(fp, L("u64")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_U64;
            if(format_string_compare_(fp, L("i8")))      types[arg_pos] = FORMAT_ARGUMENT_TYPE_I8;
            if(format_string_compare_(fp, L("i16")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_I16;
            if(format_string_compare_(fp, L("i32")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_I32;
            if(format_string_compare_(fp, L("i64")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_I64;
            if(format_string_compare_(fp, L("pointer"))) types[arg_pos] = FORMAT_ARGUMENT_TYPE_POINTER;
            if(format_string_compare_(fp, L("cstring"))) types[arg_pos] = FORMAT_ARGUMENT_TYPE_CSTRING;
            if(format_string_compare_(fp, L("string")))  types[arg_pos] = FORMAT_ARGUMENT_TYPE_STRING;
            
            int field_width = 0;
            while('0' <= *fp && *fp <= '9') {
                field_width = 10 * field_width + *fp - '0';
            }
        }
        fp++;
    }
    
    fp = format;
    while(fp) {
        if(*fp == '{') {
            
        }
    }

    return NULL;
}

static
char* format_string(String string, char *format, ...) {
    VariableArgumentList va_list;
    variable_argument_begin(va_list, format);
    char* result = format_string_list(string, format, va_list);
    variable_argument_end(va_list);
    return result;
}