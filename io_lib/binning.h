#ifndef CRAM_BINNING_H
#define CRAM_BINNING_H

/* See http://res.illumina.com/documents/products/whitepapers/whitepaper_datacompression.pdf */
static unsigned int illumina_bin[256] = {
     0,                                     /* 0 reserved for N */
     1,                                     /* Unused, but for completeness */
     6,  6,  6,  6,  6,  6,  6, 6,          /* 2-9 */
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, /* 10-19 */
    22, 22, 22, 22, 22,                     /* 20-24 */
    27, 27, 27, 27, 27,                     /* 25-29 */
    33, 33, 33, 33, 33,                     /* 30-34 */
    37, 37, 37, 37, 37,                     /* 35-39 */
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40, /* 40+ */
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    40, 40, 40, 40, 40, 40,
};

static unsigned int illumina_bin_33[256] = {
     0+33,
     1+33,
     6+33,  6+33,  6+33,  6+33,  6+33,  6+33,  6+33,  6+33,
    15+33, 15+33, 15+33, 15+33, 15+33, 15+33, 15+33, 15+33, 15+33, 15+33,
    22+33, 22+33, 22+33, 22+33, 22+33,
    27+33, 27+33, 27+33, 27+33, 27+33,
    33+33, 33+33, 33+33, 33+33, 33+33,
    37+33, 37+33, 37+33, 37+33, 37+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
    40+33, 40+33, 40+33, 40+33, 40+33, 40+33,
};

enum quality_binning {
    BINNING_NONE       = 0,
    BINNING_ILLUMINA   = 1,
};

#endif /* CRAM_BINNING_H */
