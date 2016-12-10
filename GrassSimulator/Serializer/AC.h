/* Arithmetic Coding */

#ifndef AC_h
#define AC_h

#include "Conversion.h"

#define Code_value_bits 16

#define Top_value 65535
#define First_qtr (Top_value / 4 + 1)
#define Half	  (2 * First_qtr)
#define Third_qtr (3 * First_qtr)
#define MaxFreq 16383
#define NSYM 257
#define EOF_symbol 256

struct AcModel{
    int nsym;
    int *freq;
    int *cfreq;
    
    AcModel (int, int *);
    ~AcModel ();
    void update_model (int);
};

class AcEncoder{
    FILE *fp;
    long low;
    long high;
    long fbits;
    int buffer;
    int bits_to_go;
    long total_bits;
    
public:
    AcEncoder(const char *);
    ~AcEncoder();
    
    void output_bit(int);
    void bit_plus_follow(int);
    long ac_encoder_bits();
    void ac_encode_symbol(AcModel &, int);
};

class AcDecoder{
    FILE *fp;
    long value;
    long low;
    long high;
    int buffer;
    int bits_to_go;
    int garbage_bits;

public:
    AcDecoder(const char *);
    ~AcDecoder();
    
    int ac_decode_symbol (AcModel &);
    int input_bit ();
};

class DataBase
{
    FILE *fd;

    int y_i = 0;
    
    vector<ulong> data = vector<ulong>();
    queue<uchar> stream = queue<uchar>();
    
public:
    DataBase(const char *n, const char *m)
    { fd = fopen(n, m); }

    void ac_encode();
    void ac_decode();

    void next(Color c);
    
private:
    void data_to_stream();
    void stream_to_data();
    
    void encode_stream(AcEncoder &, AcModel &);
    void decode_stream(AcDecoder &, AcModel &);
};

#endif /* AC_h */
