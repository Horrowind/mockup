#define BASE_IMPLEMENTATION
#include <base/base.h>


int main() {
    /* char bml[] = */
    /*     "node=test\n" */
    /*     "  child"; */
    char bml[] =
        "node=test attr1=1 attr2=2\n"
        "    child1 attr1=1\n"
        "    child2\n";
    Buffer bml_buffer = buffer(bml, sizeof(bml) - 1);
    BmlParser parser;
#if 0
    bml_parse_init(buffer, &parser);
    
    BmlNode node;
    int indent = 0;
    do {
        node = bml_parse(&parser);
        switch(node.type) {
        case BML_NODE:
            printf("%.*s", indent, "                                                                                ");
            printf("node (%.*s = %.*s)\n", node.name.length, node.name.data, node.value.length, node.value.data);
            indent +=2;
            break;
        case BML_NODE_END:
            indent -= 2;
            printf("%.*s", indent, "                                                                                ");
            printf("end\n");
            break;
        case BML_NODE_EOF:
            printf("eof\n");
            break;
        case BML_NODE_ERROR: printf("error\n");
            return 1;
            
        case BML_NODE_ERROR_INVALID_NODE_NAME: printf("error: invalid node name\n");
            return 1;
        }
    } while(node.type != BML_NODE_EOF);
#endif
    bml_parser_init(&parser, bml_buffer);
    bml_parse(&parser);
    for(BmlIter iter = bml_node(L("node"), &parser);
        bml_end(&iter); bml_next(&iter)) {
        printf("node = %.*s\n", iter.value.length, iter.value.data);
        for(BmlIter iter = bml_node(L("child1"), &parser);
            bml_end(&iter); bml_next(&iter)) {
            printf("child1 = %.*s\n", iter.value.length, iter.value.data);
        }
    }    
}
