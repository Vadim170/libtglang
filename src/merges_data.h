#ifndef MERGES_DATA_H
#define MERGES_DATA_H

#include <array>
#include <string>

#define SIZE_BPE_RANKS 50000
#define SIZE_BPE_RANKS2 50000

extern const std::array<std::string_view, SIZE_BPE_RANKS> global_bpe_ranks;
extern const std::array<std::string_view, SIZE_BPE_RANKS2> global_bpe_ranks2;

#endif // MERGES_DATA_H