#ifndef MERGES_DATA_H
#define MERGES_DATA_H

#include <array>
#include <string>

#define SIZE_BPE_RANKS 50000
#define SIZE_BPE_RANKS2 50000

const std::array<std::string, SIZE_BPE_RANKS>& get_bpe_ranks();
const std::array<std::string, SIZE_BPE_RANKS2>& get_bpe_ranks2();

#endif // MERGES_DATA_H