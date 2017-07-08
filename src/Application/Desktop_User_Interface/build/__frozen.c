// This provides the frozen (compiled bytecode) files that are included if
// any.
#include <Python.h>

#include "nuitka/constants_blob.h"

// Blob from which modules are unstreamed.
#define stream_data constant_bin

// These modules should be loaded as bytecode. They may e.g. have to be loadable
// during "Py_Initialize" already, or for irrelevance, they are only included
// in this un-optimized form. These are not compiled by Nuitka, and therefore
// are not accelerated at all, merely bundled with the binary or module, so
// that CPython library can start out finding them.

struct frozen_desc {
    char const *name;
    ssize_t start;
    int size;
};

void copyFrozenModulesTo( struct _frozen *destination )
{
    struct frozen_desc frozen_modules[] = {
        { "base64", 6360696, 11253 },
        { "codecs", 6371949, 36646 },
        { "copy_reg", 6408595, 5083 },
        { "encodings", 6413678, -4362 },
        { "encodings.aliases", 6418040, 8760 },
        { "encodings.ascii", 6426800, 2253 },
        { "encodings.base64_codec", 6429053, 3829 },
        { "encodings.big5", 6432882, 1748 },
        { "encodings.big5hkscs", 6434630, 1788 },
        { "encodings.bz2_codec", 6436418, 4721 },
        { "encodings.charmap", 6441139, 3465 },
        { "encodings.cp037", 6444604, 2829 },
        { "encodings.cp1006", 6447433, 2915 },
        { "encodings.cp1026", 6450348, 2843 },
        { "encodings.cp1140", 6453191, 2829 },
        { "encodings.cp1250", 6456020, 2866 },
        { "encodings.cp1251", 6458886, 2863 },
        { "encodings.cp1252", 6461749, 2866 },
        { "encodings.cp1253", 6464615, 2879 },
        { "encodings.cp1254", 6467494, 2868 },
        { "encodings.cp1255", 6470362, 2887 },
        { "encodings.cp1256", 6473249, 2865 },
        { "encodings.cp1257", 6476114, 2873 },
        { "encodings.cp1258", 6478987, 2871 },
        { "encodings.cp424", 6481858, 2859 },
        { "encodings.cp437", 6484717, 8064 },
        { "encodings.cp500", 6492781, 2829 },
        { "encodings.cp720", 6495610, 2926 },
        { "encodings.cp737", 6498536, 8292 },
        { "encodings.cp775", 6506828, 8078 },
        { "encodings.cp850", 6514906, 7811 },
        { "encodings.cp852", 6522717, 8080 },
        { "encodings.cp855", 6530797, 8261 },
        { "encodings.cp856", 6539058, 2891 },
        { "encodings.cp857", 6541949, 7801 },
        { "encodings.cp858", 6549750, 7781 },
        { "encodings.cp860", 6557531, 8047 },
        { "encodings.cp861", 6565578, 8058 },
        { "encodings.cp862", 6573636, 8193 },
        { "encodings.cp863", 6581829, 8058 },
        { "encodings.cp864", 6589887, 8189 },
        { "encodings.cp865", 6598076, 8058 },
        { "encodings.cp866", 6606134, 8293 },
        { "encodings.cp869", 6614427, 8105 },
        { "encodings.cp874", 6622532, 2957 },
        { "encodings.cp875", 6625489, 2826 },
        { "encodings.cp932", 6628315, 1756 },
        { "encodings.cp949", 6630071, 1756 },
        { "encodings.cp950", 6631827, 1756 },
        { "encodings.euc_jis_2004", 6633583, 1812 },
        { "encodings.euc_jisx0213", 6635395, 1812 },
        { "encodings.euc_jp", 6637207, 1764 },
        { "encodings.euc_kr", 6638971, 1764 },
        { "encodings.gb18030", 6640735, 1772 },
        { "encodings.gb2312", 6642507, 1764 },
        { "encodings.gbk", 6644271, 1740 },
        { "encodings.hex_codec", 6646011, 3781 },
        { "encodings.hp_roman8", 6649792, 4112 },
        { "encodings.hz", 6653904, 1732 },
        { "encodings.idna", 6655636, 6368 },
        { "encodings.iso2022_jp", 6662004, 1801 },
        { "encodings.iso2022_jp_1", 6663805, 1817 },
        { "encodings.iso2022_jp_2", 6665622, 1817 },
        { "encodings.iso2022_jp_2004", 6667439, 1841 },
        { "encodings.iso2022_jp_3", 6669280, 1817 },
        { "encodings.iso2022_jp_ext", 6671097, 1833 },
        { "encodings.iso2022_kr", 6672930, 1801 },
        { "encodings.iso8859_1", 6674731, 2868 },
        { "encodings.iso8859_10", 6677599, 2883 },
        { "encodings.iso8859_11", 6680482, 2977 },
        { "encodings.iso8859_13", 6683459, 2886 },
        { "encodings.iso8859_14", 6686345, 2904 },
        { "encodings.iso8859_15", 6689249, 2883 },
        { "encodings.iso8859_16", 6692132, 2885 },
        { "encodings.iso8859_2", 6695017, 2868 },
        { "encodings.iso8859_3", 6697885, 2875 },
        { "encodings.iso8859_4", 6700760, 2868 },
        { "encodings.iso8859_5", 6703628, 2869 },
        { "encodings.iso8859_6", 6706497, 2913 },
        { "encodings.iso8859_7", 6709410, 2876 },
        { "encodings.iso8859_8", 6712286, 2907 },
        { "encodings.iso8859_9", 6715193, 2868 },
        { "encodings.johab", 6718061, 1756 },
        { "encodings.koi8_r", 6719817, 2890 },
        { "encodings.koi8_u", 6722707, 2876 },
        { "encodings.latin_1", 6725583, 2283 },
        { "encodings.mac_arabic", 6727866, 8014 },
        { "encodings.mac_centeuro", 6735880, 2937 },
        { "encodings.mac_croatian", 6738817, 2945 },
        { "encodings.mac_cyrillic", 6741762, 2935 },
        { "encodings.mac_farsi", 6744697, 2849 },
        { "encodings.mac_greek", 6747546, 2889 },
        { "encodings.mac_iceland", 6750435, 2928 },
        { "encodings.mac_latin2", 6753363, 4907 },
        { "encodings.mac_roman", 6758270, 2906 },
        { "encodings.mac_romanian", 6761176, 2946 },
        { "encodings.mac_turkish", 6764122, 2929 },
        { "encodings.palmos", 6767051, 3066 },
        { "encodings.ptcp154", 6770117, 4890 },
        { "encodings.punycode", 6775007, 7980 },
        { "encodings.quopri_codec", 6782987, 3647 },
        { "encodings.raw_unicode_escape", 6786634, 2202 },
        { "encodings.rot_13", 6788836, 3656 },
        { "encodings.shift_jis", 6792492, 1788 },
        { "encodings.shift_jis_2004", 6794280, 1828 },
        { "encodings.shift_jisx0213", 6796108, 1828 },
        { "encodings.string_escape", 6797936, 2061 },
        { "encodings.tis_620", 6799997, 2938 },
        { "encodings.undefined", 6802935, 2589 },
        { "encodings.unicode_escape", 6805524, 2150 },
        { "encodings.unicode_internal", 6807674, 2176 },
        { "encodings.utf_16", 6809850, 5160 },
        { "encodings.utf_16_be", 6815010, 1990 },
        { "encodings.utf_16_le", 6817000, 1990 },
        { "encodings.utf_32", 6818990, 5724 },
        { "encodings.utf_32_be", 6824714, 1883 },
        { "encodings.utf_32_le", 6826597, 1883 },
        { "encodings.utf_7", 6828480, 1883 },
        { "encodings.utf_8", 6830363, 1942 },
        { "encodings.utf_8_sig", 6832305, 4977 },
        { "encodings.uu_codec", 6837282, 4909 },
        { "encodings.zlib_codec", 6842191, 4641 },
        { "functools", 6846832, 6029 },
        { "locale", 6852861, 54994 },
        { "quopri", 6907855, 6544 },
        { "re", 6914399, 13363 },
        { "sre_compile", 6927762, 12521 },
        { "sre_constants", 6940283, 6177 },
        { "sre_parse", 6946460, 19937 },
        { "string", 6966397, 20349 },
        { "stringprep", 6986746, 14441 },
        { "struct", 7001187, 229 },
        { "types", 7001416, 2703 },
        { NULL, 0, 0 }
    };

    struct frozen_desc *current = frozen_modules;

    for(;;)
    {
        destination->name = (char *)current->name;
        destination->code = (unsigned char *)&constant_bin[ current->start ];
        destination->size = current->size;

        if (destination->name == NULL) break;

        current += 1;
        destination += 1;
    };
}
