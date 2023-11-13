#ifndef MERGES_DATA_H
#define MERGES_DATA_H

#include <array>
#include <string>

#define SIZE_BPE_RANKS 100000

const std::array<std::string, SIZE_BPE_RANKS>& get_bpe_ranks();

#endif // MERGES_DATA_H