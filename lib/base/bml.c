typedef enum {
    BML_PARSER_STATE_INDENT,
    BML_PARSER_STATE_INDENT_DECREASE,
    BML_PARSER_STATE_CHILD,
    BML_PARSER_STATE_ATTRIBUTE,
    BML_PARSER_STATE_ATTRIBUTE_END,
    BML_PARSER_STATE_EOF,
} BmlParserState;

BmlNode bml_skip(BmlParser* parser) {
    int depth = 1;
    while(depth > 0) {
        BmlNode node = bml_parse(parser);
        switch(node.type) {
        case BML_NODE: {
            //printf("Node\n");
            depth++;
            break;
        }
        case BML_NODE_END: {
            //printf("End\n");
            depth--;
            break;
        }
        default: {
            return node;
        }
        }
    }
    return bml_parse(parser);
}

static
void bml_parser_advance(BmlParser* parser) {
    if(parser->pos < (char*)parser->buffer.end) {
        parser->pos++;
    } else {
        parser->state = BML_PARSER_STATE_EOF;
    }
}

static
int bml_is_valid_char(char c)  {  //A-Z, a-z, 0-9, -.
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || c == '-' || c == '.';
}


static
void bml_parse_name(BmlParser* parser, BmlNode* node) {
    if(parser->pos >= (char*)parser->buffer.end) {
        parser->state = BML_PARSER_STATE_EOF;
        return;
    }
    char* begin = parser->pos;
    while(parser->state != BML_PARSER_STATE_EOF &&
          bml_is_valid_char(parser->pos[0])) {
        bml_parser_advance(parser);
    }
    if(parser->pos == begin) {
        node->type = BML_NODE_ERROR_INVALID_NODE_NAME;
    }
    node->name = (String) {
        .data = begin,
        .length = parser->pos - begin
    };
}

static
void bml_parse_data(BmlParser* parser, BmlNode* node) {
    char* begin = NULL, *end = NULL;
    if(parser->pos[0] == '=' && parser->pos[1] == '"') {
        bml_parser_advance(parser);
        bml_parser_advance(parser);
        begin = parser->pos;
        int cont = 1;
        while(cont) {
            switch(parser->pos[0]) {
            case '"':
                cont = 0;
                break;
            case '\0':
            case '\n':
                //error_at_pos(parser->text_pos);
                c_error_format("Unescaped value.\n");
                exit(1);
                break;
            case '\r':
                if(parser->pos[1] == '\n') {
                    //error_at_pos(parser->text_pos);
                    c_error_format("Unescaped value.\n");
                    exit(1);
                }
            fallthrough default:
                bml_parser_advance(parser);
            }
        }
        end = parser->pos;
        bml_parser_advance(parser);
    } else if(parser->pos[0] == '=') {
        bml_parser_advance(parser);
        begin = parser->pos;
        int cont = 1;
        while(cont) {
            switch(parser->pos[0]) {
            case '\0':
            case '\n':
            case ' ':
                cont = 0;
                break;
            case '"':
                //error_at_pos(parser->text_pos);
                c_error_format("Illegal character in value.\n");
                exit(1);
            case '\r':
                if(parser->pos[1] == '\n') {
                    cont = 0;
                }
                bml_parser_advance(parser);
                break;
            default:
                bml_parser_advance(parser);
            }
        }
        end = parser->pos;
    } else if(parser->pos[0] == ':') {
        bml_parser_advance(parser);
        begin = parser->pos;
        int cont = 1;
        while(cont) {
            switch(parser->pos[0]) {
            case '\0':
            case '\n':
                cont = 0;
                break;
            default:
                bml_parser_advance(parser);
            }
        }
        end = parser->pos;
    }
    node->value = (String) {
        .data = begin,
        .length = end - begin
    };
}

static
int bml_parse_indent(BmlParser* parser) {
    int result = 0;
    while(parser->state != BML_PARSER_STATE_EOF &&
          (parser->pos[0] == ' ' || parser->pos[0] == '\t')) {
        result++;
        bml_parser_advance(parser);
    }
    return result;
}


void bml_parser_init(BmlParser* parser, Buffer buffer) {
    *parser = (BmlParser) {
        .buffer        = buffer,
        .pos           = buffer.begin,
        .indent_levels = { },
        .state         = BML_PARSER_STATE_CHILD,
    };
}

static BmlNode end_node = { .type = BML_NODE_END };
static BmlNode eof_node = { .type = BML_NODE_EOF };

BmlNode bml_parse(BmlParser* parser) {
    BmlNode result = {};
    int cont = 1;
    while(cont) {
        switch(parser->state) {
        case BML_PARSER_STATE_INDENT: {
            int indent = bml_parse_indent(parser);
            if(parser->state == BML_PARSER_STATE_EOF) break;
            parser->indent = indent;
            int index = parser->indent_index;
            int current_indent = parser->indent_levels[index];
            if(current_indent < indent) {
                if(index == 15) {
                    return (BmlNode){ .type = BML_NODE_ERROR /* _NESTING_TOO_DEEP*/ };
                }
                parser->indent_index++;
                parser->indent_levels[index + 1] = parser->indent;
                parser->state = BML_PARSER_STATE_CHILD;
                break;
            } else if(current_indent > indent) {
                parser->state = BML_PARSER_STATE_INDENT_DECREASE;
                result = end_node;
                cont = 0;
                break;
            } else {
                parser->state = BML_PARSER_STATE_CHILD;
                result = end_node;
                cont = 0;
                break;
            }
        }
        case BML_PARSER_STATE_INDENT_DECREASE: {
            int index = parser->indent_index;
            int current_indent = parser->indent_levels[index];
            if(current_indent > parser->indent) {
                parser->indent_index--;
                result = end_node;
                cont = 0;
                break;
            } else if(current_indent < parser->indent) {
                result = (BmlNode){ .type = BML_NODE_ERROR /* _INDENTATION*/ };
                cont = 0;
                break;
            }
            parser->state = BML_PARSER_STATE_CHILD;
            break;
            //return end_node;
        }
        case BML_PARSER_STATE_CHILD: {
            result = (BmlNode) { .type = BML_NODE };
            if(parser->pos[0] == '\n') {
                parser->state = BML_PARSER_STATE_INDENT;
                bml_parser_advance(parser);
                break;
            }
            bml_parse_name(parser, &result);
            bml_parse_data(parser, &result);
            if(parser->state == BML_PARSER_STATE_EOF) break;
            parser->state = BML_PARSER_STATE_ATTRIBUTE;
            cont = 0;
            break;
        }
        case BML_PARSER_STATE_ATTRIBUTE: {
            while(parser->pos[0] == ' ') {
                bml_parser_advance(parser);
                if(parser->state == BML_PARSER_STATE_EOF) break;
            }
            if(parser->pos[0] == '\n') {
                bml_parser_advance(parser);
                if(parser->state == BML_PARSER_STATE_EOF) break;
                parser->state = BML_PARSER_STATE_INDENT;
                break;
            }
            result = (BmlNode) { .type = BML_NODE };
            bml_parse_name(parser, &result);
            bml_parse_data(parser, &result);
            if(parser->state == BML_PARSER_STATE_EOF) break;
            parser->state = BML_PARSER_STATE_ATTRIBUTE_END;
            cont = 0;
            break;
        }
        case BML_PARSER_STATE_ATTRIBUTE_END: {
            parser->state = BML_PARSER_STATE_ATTRIBUTE;
            result = end_node;
            cont = 0;
            break;
        }
        case BML_PARSER_STATE_EOF: {
            if(parser->indent_index >= 0) {
                parser->indent_index--;
                result =  end_node;
                cont = 0;
                break;
            } else {
                result = eof_node;
                cont = 0;
                break;
            }
        }
        }        
    }
#if 0
    switch(result.type) {
    case BML_NODE:
        printf("NODE %.*s: %.*s\n", result.name.length, result.name.data,
               result.value.length, result.value.data);
        break;
    case BML_NODE_END:
        printf("END\n");
        break;
    case BML_NODE_ERROR:
    case BML_NODE_ERROR_INVALID_NODE_NAME:
        printf("ERROR\n");
        break;
    case BML_NODE_EOF:
        printf("EOF\n");
        break;
    }
#endif
    return result;
}

#if 0
void bml_parse_attributes(BmlNode* node, BmlParser* parser) {
    while(1) {
        if(!parser->pos[0]) break;
        if(parser->pos[0] == '\n') {
            bml_parser_advance(parser);
            break;
        }
        if(parser->pos[0] != ' ') {
            //error_at_pos(parser->text_pos);
            c_error_format("Invalid node name.\n");
            exit(1);
        }
        while(parser->pos[0] == ' ') bml_parser_advance(parser); //skip excess spaces
        if(parser->pos[0] == '/' && parser->pos[1] == '/') {     //skip comments
            while(parser->pos[0] && parser->pos[0] != '\n' &&
                  !(parser->pos[0] == '\r' && parser->pos[1] == '\n')) {
                bml_parser_advance(parser);
            }
            break;
        }

        char* begin = parser->pos;
        while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
        if(parser->pos == begin) {
            //error_at_pos(parser->text_pos);
            c_error_format("Invalid attribute name.\n");
            exit(1);
        }
        new_node->name = (String) {
            .data = begin,
            .length = parser->pos - begin
        };
        bml_parse_data(new_node, parser);
        *next_node_ptr = new_node;
        next_node_ptr = &new_node->next;
    }
}


BmlNode* bml_parse_node(BmlParser* parser, int* indent) {
    if(parser->pos[0] == '\0') {
        return NULL;
    }
    if(parser->pos[0] == '\n') {
        if(*indent == 0) {
            bml_parser_advance(parser);
            *indent = bml_parse_indent(parser);
        }
    }
    BmlNode* node = arena_alloc_type(parser->arena, BmlNode);
    *node = (BmlNode) { };
    bml_parse_name(node, parser);
    bml_parse_data(node, parser);

    BmlNode** next_node_ptr = &node->child;

    // Parse attributes
    while(1) {
        if(!parser->pos[0]) break;
        if(parser->pos[0] == '\n') {
            bml_parser_advance(parser);
            break;
        }
        if(parser->pos[0] != ' ') {
            //error_at_pos(parser->text_pos);
            c_error_format("Invalid node name.\n");
            exit(1);
        }
        while(parser->pos[0] == ' ') bml_parser_advance(parser); //skip excess spaces
        if(parser->pos[0] == '/' && parser->pos[1] == '/') {     //skip comments
            while(parser->pos[0] && parser->pos[0] != '\n' &&
                  !(parser->pos[0] == '\r' && parser->pos[1] == '\n')) {
                bml_parser_advance(parser);
            }
            break;
        }

        char* begin = parser->pos;
        while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
        if(parser->pos == begin) {
            //error_at_pos(parser->text_pos);
            c_error_format("Invalid attribute name.\n");
            exit(1);
        }
        new_node->name = (String) {
            .data = begin,
            .length = parser->pos - begin
        };
        bml_parse_data(new_node, parser);
        *next_node_ptr = new_node;
        next_node_ptr = &new_node->next;
    }
    
    
    // Parse child nodes
    if(parser->pos[0] == '\0') {
        return node;
    }

    int child_indent = bml_parse_indent(parser);
    int new_indent = child_indent;
    while(1) {
        if(parser->pos[0] == '\0') {
            return node;
        }
        if(new_indent <= *indent) {
            *indent = new_indent;
            return node;
        } else if(new_indent != child_indent) {
            //error_at_pos(parser->text_pos);
            c_error_format("Indentation error.\n");
            exit(1);
        } else {
            BmlNode* new_node = bml_parse_node(parser, &new_indent);
            *next_node_ptr = new_node;
            next_node_ptr = &new_node->next;
            if(new_indent <= *indent) {
                *indent = new_indent;
                return node;
            } else if(new_indent != child_indent) {
                //error_at_pos(parser->text_pos);
                c_error_format("Indentation error.\n");
                exit(1);
            }
        }
    }
}

BmlNode* bml_parse(Buffer buffer, Arena* arena) {
    BmlParser parser = { };
    parser.buffer = buffer;
    parser.pos    = (char*)buffer.begin;
    //parser.text_pos.line_number = 1;
    parser.arena = arena;
    int indent = 0;
    BmlNode* root;
    BmlNode** next_node_ptr = &root;
    while(parser.pos[0] != '\0') {
        *next_node_ptr =  bml_parse_node(&parser, &indent);
        next_node_ptr = &((*next_node_ptr)->next);
    }
    return root;
}



void bml_print_node(BmlNode* node, int indent) {
    if(node) {
        printf("%*s%.*s: %.*s\n", indent, "", node->name.length, node->name.data,
               node->value.length, node->value.data);
        bml_print_node(node->child, indent + 2);
        bml_print_node(node->next, indent);
    }
}

#endif
