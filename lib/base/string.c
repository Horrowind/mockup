String string(char* data, ulong length) {
    return (String){ .data = data, .length = length };
}

Buffer buffer_from_string(String string) {
    return (Buffer){
        .begin = string.data,
        .end   = string.data + string.length
    };
}

String string_from_buffer(Buffer buffer) {
    return (String){
        .data   = buffer.begin,
        .length = buffer.end - buffer.begin
    };
}


void lowcase_string(String s) {
    for(ulong i = 0; i < s.length; i++) {
        if((uint)s.data[i] - 'A' < 26) s.data[i] |= 0x20;
    }
}

void upcase_string(String s) {
    for(ulong i = 0; i < s.length; i++) {
        if((uint)s.data[i] - 'a' < 26) s.data[i] &= 0x5f;
    }
}


StringBuilder string_builder(Buffer buffer) {
    return (StringBuilder) {
        .buffer = buffer,
        .length = 0
    };
}

void string_builder_append(StringBuilder* builder, String string) {
    Buffer dest = {
        .begin = builder->buffer.begin + builder->length,
        .end   = builder->buffer.end
    };
    copy_buffer(buffer_from_string(string), dest);
    builder->length += string.length;
}

String string_from_builder(StringBuilder builder) {
    return (String) {
        .data = builder.buffer.begin,
        .length = builder.length
    };
}


b32 string_equal(String s1, String s2) {
    if(s1.length != s2.length) return 0;
    for(ulong i = 0; i < s1.length; i++) {
        if(s1.data[i] != s2.data[i]) return 0;
    }
    return 1;
}

u64 string_to_u64_base(String s, int base) {
    u64 num = 0;
    for(ulong i = 0; i < s.length; i++) {
        int digit = 0;
        if(s.data[i] >= '0' && s.data[i] <= '9') {
            digit = s.data[i] - '0';
        } else if(s.data[i] >= 'A' && s.data[i] <= 'Z') {
            digit = s.data[i] - 'A' + 10;
        } else if(s.data[i] >= 'a' && s.data[i] <= 'z') {
            digit = s.data[i] - 'a' + 10;
        } else {
            break;
        }
        if(digit >= base) break;
        num = num * base + digit;
    }
    return num;
}

u64 string_to_u64(String s) {
    int base = 10;
    if(s.length >= 3 && s.data[0] == '0' && (s.data[1] == 'x' || s.data[1] == 'X')) {
        base = 16; s.data += 2; s.length -= 2;
    } else if(s.length >= 3 && s.data[0] == '0' && (s.data[1] == 'b' || s.data[1] == 'B')) {
        base =  2; s.data += 2; s.length -= 2;
    } else if(s.length >= 3 && s.data[0] == '0' && (s.data[1] == 'o' || s.data[1] == 'O')) {
        base =  8; s.data += 2; s.length -= 2;
    } else if(s.length >= 2 && s.data[0] == '%') {
        base =  2; s.data += 1; s.length -= 1;
    } else if(s.length >= 2 && s.data[0] == '$') {
        base = 16; s.data += 1; s.length -= 1;
    }
    return string_to_u64_base(s, base);
}

String string_trim_spaces(String str) {
    while(str.length > 0 && str.data[0] == ' ') {
        str.length--;
        str.data++;
    }
    return str;
}

String string_from_c_string(char* buf) {
    int length = 0;
    String s = {
        .data = buf
    };
    while(*buf != '\0') {
        buf++;
        length++;
    }
    
    s.length = length;
    //s.params = STRING_PARAMETER_ZERO_TERMINATED;
    return s;
}

i32 i32_from_z_internal(char** at_init) {
    i32 result = 0;
    
    char* at = *at_init;
    while((*at >= '0') &&
          (*at <= '9')) {
            result *= 10;
            result += (*at - '0');
            ++at;
        }

    *at_init = at;

    return result;
}

typedef struct {
    uptr size;
    char* at;
} FormatDestination;

void out_char(FormatDestination* Dest, char Value) {
    if(Dest->size) {
        Dest->size++;
        *Dest->at++ = Value;
    }
}

void out_chars(FormatDestination* Dest, char* Value) {
    // NOTE(casey): Not particularly speedy, are we?  :P
    while(*Value) {
        out_char(Dest, *Value++);
    }
}

char dec_chars[] = "0123456789";
char lower_hex_chars[] = "0123456789abcdef";
char upper_hex_chars[] = "0123456789ABCDEF";
void u64_to_ascii(FormatDestination* dest, u64 value, u32 base, char* digits) {
    assert(base != 0);

    char* start = dest->at;
    do {
        u64 digit_index = (value % base);
        out_char(dest, digits[digit_index]);
        value /= base;
    } while(value != 0);
    char* end = dest->at;
    
    while(start < end) {
        end--;
        char temp = *end;
        *end = *start;
        *start = temp;
        start++;
    }
}

void f64_to_ascii(FormatDestination* dest, f64 value, u32 precision) {
    if(value < 0) {
        out_char(dest, '-');
        value = -value;
    }

    u64 integer_part = (u64)value;
    value -= (f64)integer_part;
    u64_to_ascii(dest, integer_part, 10, dec_chars);

    out_char(dest, '.');

    // TODO(casey): Note that this is NOT an accurate way to do this!
    for(u32 precision_index = 0; precision_index < precision; precision_index++) {
        value *= 10.0f;
        u32 integer = (u32)value;
        value -= (f32)integer;
        out_char(dest, dec_chars[integer]);
    }
}

#define variable_argument_get_uint(length, arg_list) ((length) == 8) ? variable_argument_get(arg_list, u64) : (u64)variable_argument_get(arg_list, u32)
#define read_var_arg_signed_integer(length, arg_list) ((length) == 8) ? variable_argument_get(arg_list, i64) : (i64)variable_argument_get(arg_list, i32)
#define read_var_arg_float(length, arg_list) variable_argument_get(arg_list, f64)

int string_format_list(Buffer buffer, String format, VariableArgumentList arg_list) {
    char* end = format.data + format.length;
    FormatDestination dest = { .size = buffer.end - buffer.begin, .at = buffer.begin };
    if(dest.size) {
        char* at = format.data;
        while(at < end) {
            if(*at == '%') {
                at++;
                
                b32 force_sign = 0;
                b32 pad_with_zeros = 0;
                b32 left_justify = 0;
                b32 positive_sign_is_blank = 0;
                b32 annotate_if_not_zero = 0;
                
                b32 parsing = 1;
                
                //
                // NOTE(casey): Handle the flags
                //
                while(parsing) {
                    switch(*at) {
                        case '+': {force_sign = 1;} break;
                        case '0': {pad_with_zeros = 1;} break;
                        case '-': {left_justify = 1;} break;
                        case ' ': {positive_sign_is_blank = 1;} break;
                        case '#': {annotate_if_not_zero = 1;} break;
                        default: {parsing = 0;} break;
                    }
                    
                    if(parsing) {
                        at++;
                    }
                }
                
                //
                // NOTE(casey): Handle the width
                //
                b32 width_specified = 0;
                (void)width_specified;
                i32 width = 0;
                if(*at == '*') {
                    width = variable_argument_get(arg_list, int);
                    width_specified = 1;
                    at++;
                } else if((*at >= '0') && (*at <= '9')) {
                    width = i32_from_z_internal(&at);
                    width_specified = 1;
                }
                
                //
                // NOTE(casey): Handle the precision
                //
                b32 precision_specified = 0;
                u32 precision = 0;
                if(*at == '.') {
                    at++;
                    
                    if(*at == '*') {
                        precision = variable_argument_get(arg_list, int);
                        precision_specified = 1;
                        at++;
                    } else if((*at >= '0') && (*at <= '9')) {
                        precision = i32_from_z_internal(&at);
                        precision_specified = 1;
                    } else {
                        assert(!"Malformed precision specifier!");
                    }
                }
                
                // TODO(casey): Right now our routine doesn't allow non-specified
                // precisions, so we just set non-specified precisions to a specified value
                if(!precision_specified) {
                    precision = 6;
                }
                
                //
                // NOTE(casey): Handle the length
                //
                u32 integer_length = 4;
                u32 float_length = 8;
                (void)float_length;
                // TODO(casey): Actually set different values here!
                if((at[0] == 'h') && (at[1] == 'h')) {
                    at += 2;
                } else if((at[0] == 'l') && (at[1] == 'l')) {
                    at += 2;
                } else if(*at == 'h') {
                    at++;
                } else if(*at == 'l') {
                    integer_length = 8;
                    at++;
                } else if(*at == 'j') {
                    at++;
                } else if(*at == 'z') {
                    at++;
                } else if(*at == 't') {
                    at++;
                } else if(*at == 'L') {
                    at++;
                }
                
                char temp_buffer[64];
                char* temp = temp_buffer;
                FormatDestination temp_dest = { .size = array_length(temp_buffer), .at = temp};
                char* prefix = "";
                b32 is_float = 0;
                
                switch(*at) {
                case 'd': case 'i': {
                    i64 value = read_var_arg_signed_integer(integer_length, arg_list);
                    b32 was_negative = (value < 0);
                    if(was_negative) {
                        value = -value;
                    }
                    u64_to_ascii(&temp_dest, (u64)value, 10, dec_chars);
                    
                    // TODO(casey): Make this a common routine once floating
                    // point is available.
                    if(was_negative) {
                        prefix = "-";
                    } else if(force_sign) {
                        assert(!positive_sign_is_blank); // NOTE(casey): Not a problem here, but probably shouldn't be specified?
                        prefix = "+";
                    } else if(positive_sign_is_blank) {
                        prefix = " ";
                    }
                    break;
                } 
                    
                case 'u': {
                    u64 value = variable_argument_get_uint(integer_length, arg_list);
                    u64_to_ascii(&temp_dest, value, 10, dec_chars);
                    break;
                }
                    
                case 'o': {
                    u64 value = variable_argument_get_uint(integer_length, arg_list);
                    u64_to_ascii(&temp_dest, value, 8, dec_chars);
                    if(annotate_if_not_zero && (value != 0)) {
                        prefix = "0";
                    }
                    break;
                }
                    
                case 'x': {
                    u64 value = variable_argument_get_uint(integer_length, arg_list);
                    u64_to_ascii(&temp_dest, value, 16, lower_hex_chars);
                    if(annotate_if_not_zero && (value != 0)) {
                        prefix = "0x";
                    }
                    break;
                }
                    
                case 'X': {
                    u64 value = variable_argument_get_uint(integer_length, arg_list);
                    u64_to_ascii(&temp_dest, value, 16, upper_hex_chars);
                    if(annotate_if_not_zero && (value != 0)) {
                        prefix = "0X";
                    }
                    break;
                }
                    
                // TODO(casey): Support other kinds of floating point prints
                // (right now we only do basic decimal output)
                case 'f': case 'F': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A': {
                    f64 value = read_var_arg_float(float_length, arg_list);
                    f64_to_ascii(&temp_dest, value, precision);
                    is_float = 1;
                    break;
                }
                    
                case 'c': {
                    int value = variable_argument_get(arg_list, int);
                    out_char(&temp_dest, (char)value);
                    break;
                }
                    
                case 's': {
                    char* string = variable_argument_get(arg_list, char*);
                    
                    // TODO(casey): Obey precision, width, etc.
                    temp = string;
                    if(precision_specified) {
                        temp_dest.size = 0;
                        for(char* Scan = string; *Scan && (temp_dest.size < precision); ++Scan) {
                            ++temp_dest.size;
                        }
                    } else {
                        temp_dest.size = c_string_length(string);
                    }
                    temp_dest.at = string + temp_dest.size;
                    break;
                }
                    
                case 'S': {
                    String string = variable_argument_get(arg_list, String);
                    
                    // TODO(casey): Obey precision, width, etc.
                    
                    temp = (char*)string.data;
                    temp_dest.size = string.length;
                    if(precision_specified && (temp_dest.size > precision)) {
                        temp_dest.size = precision;
                    }
                    temp_dest.at = temp + temp_dest.size;
                    break;
                }
                    
                case 'p': {
                    uptr value = (uptr)variable_argument_get(arg_list, void*);
                    u64_to_ascii(&temp_dest, value, 16, lower_hex_chars);
                    break;
                }
                    
                case 'n': {
                    int* tab_dest = variable_argument_get(arg_list, int*);
                    *tab_dest = (int)(dest.at - (char*)buffer.begin);
                    break;
                }
                    
                case '%': {
                    out_char(&dest, '%');
                    break;
                }
                    
                default: {
                    assert(!"Unrecognized format specifier");
                    break;
                }
                }
                
                if(temp_dest.at - temp) {
                    iptr use_precision = precision;
                    if(is_float || !precision_specified) {
                        use_precision = (temp_dest.at - temp);
                    }
                    
                    iptr prefix_length = c_string_length(prefix);
                    iptr use_width = width;
                    iptr ComputedWidth = use_precision + prefix_length;
                    if(use_width < ComputedWidth) {
                        use_width = ComputedWidth;
                    }
                    
                    if(pad_with_zeros) {
                        assert(!left_justify); // NOTE(casey): Not a problem, but no way to do it?
                        left_justify = 0;
                    }
                    
                    if(!left_justify) {
                        while(use_width > (use_precision + prefix_length)) {
                            out_char(&dest, pad_with_zeros ? '0' : ' ');
                            use_width--;
                        }
                    }
                    
                    for(char* pre = prefix; *pre && use_width; ++pre) {
                        out_char(&dest, *pre);
                        use_width--;
                    }
                    
                    if(use_precision > use_width) {
                        use_precision = use_width;
                    }
                    while(use_precision > (temp_dest.at - temp)) {
                        out_char(&dest, '0');
                        --use_precision;
                        use_width--;
                    }
                    while(use_precision && (temp_dest.at != temp)) {
                        out_char(&dest, *temp++);
                        --use_precision;
                        use_width--;
                    }
                    
                    if(left_justify) {
                        while(use_width) {
                            out_char(&dest, ' ');
                            use_width--;
                        }
                    }
                }
                
                if(*at) {
                    at++;
                }
            } else {
                out_char(&dest, *at++);
            }
        }
    }
    
    int result = dest.at - (char*)buffer.begin;
    return result;
}

int c_string_format(char* raw_buffer, int length, char* format, ...) {
    VariableArgumentList arg_list;
    
    variable_argument_begin(arg_list, format);
    int result = string_format_list(buffer(raw_buffer, length), string_from_c_string(format), arg_list);
    variable_argument_end(arg_list);
    
    return result;
}

ulong c_string_length(const char* s) {
    if(!s) return 0;
    const char* begin = s;
    while(*s) s++;
    return s - begin;
}


int c_string_format_unsafe(char* raw_buffer, char* format, ...) {
    VariableArgumentList arg_list;
    
    variable_argument_begin(arg_list, format);
    int result = string_format_list(buffer(raw_buffer, INT_MAX), string_from_c_string(format), arg_list);
    variable_argument_end(arg_list);
    
    return result;
}


int c_string_equal(const char* restrict s1, const char* restrict s2) {
    do {
        if(*s1 != *s2) return 0;
    } while(*s1++);
    return 1;
}


void c_string_copy(char* restrict dest, const char* restrict src, uint n) {
    if(n == 0) return;
    uint i = 0;
    for(; i < n - 1 && src[i]; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

