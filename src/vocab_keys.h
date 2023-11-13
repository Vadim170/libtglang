#ifndef VOCAB_KEYS_H
#define VOCAB_KEYS_H

#include <array>
#include <string>

#define SIZE_VOCAB_KEYS 50257

const std::array<std::string, SIZE_VOCAB_KEYS>&  get_vocab_keys();

#endif // VOCAB_KEYS_H