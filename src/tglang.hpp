#ifndef TGLANG_H
#define TGLANG_H

/**
 * Library for determining programming or markup language of a code snippet.
 */
 
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
#  ifdef tglang_EXPORTS
#    define TGLANG_EXPORT __declspec(dllexport)
#  else
#    define TGLANG_EXPORT __declspec(dllimport)
#  endif
#else
#  define TGLANG_EXPORT __attribute__((visibility("default")))
#endif

#define TGLANG_LANGUAGE_COUNT 29
/**
 * List of supported languages.
 */
enum TglangLanguage {
  TGLANG_LANGUAGE_OTHER,
  TGLANG_LANGUAGE_C,
  TGLANG_LANGUAGE_CPLUSPLUS,
  TGLANG_LANGUAGE_CSHARP,
  TGLANG_LANGUAGE_CSS,
  TGLANG_LANGUAGE_DART,
  TGLANG_LANGUAGE_DOCKER,
  TGLANG_LANGUAGE_FUNC,
  TGLANG_LANGUAGE_GO,
  TGLANG_LANGUAGE_HTML,
  TGLANG_LANGUAGE_JAVA,
  TGLANG_LANGUAGE_JAVASCRIPT,
  TGLANG_LANGUAGE_JSON,
  TGLANG_LANGUAGE_KOTLIN,
  TGLANG_LANGUAGE_LUA,
  TGLANG_LANGUAGE_NGINX,
  TGLANG_LANGUAGE_OBJECTIVE_C,
  TGLANG_LANGUAGE_PHP,
  TGLANG_LANGUAGE_POWERSHELL,
  TGLANG_LANGUAGE_PYTHON,
  TGLANG_LANGUAGE_RUBY,
  TGLANG_LANGUAGE_RUST,
  TGLANG_LANGUAGE_SHELL,
  TGLANG_LANGUAGE_SOLIDITY,
  TGLANG_LANGUAGE_SQL,
  TGLANG_LANGUAGE_SWIFT,
  TGLANG_LANGUAGE_TL,
  TGLANG_LANGUAGE_TYPESCRIPT,
  TGLANG_LANGUAGE_XML
};

float categoryThresholds[TGLANG_LANGUAGE_COUNT] = {
    0, // Threshold for TGLANG_LANGUAGE_OTHER
    0.9, // Threshold for TGLANG_LANGUAGE_C
    0.95, // Threshold for TGLANG_LANGUAGE_CPLUSPLUS
    0.95, // Threshold for TGLANG_LANGUAGE_CSHARP
    0.95, // Threshold for TGLANG_LANGUAGE_CSS
    0.95, // Threshold for TGLANG_LANGUAGE_DART
    0.95, // Threshold for TGLANG_LANGUAGE_DOCKER
    0.95, // Threshold for TGLANG_LANGUAGE_FUNC
    0.95, // Threshold for TGLANG_LANGUAGE_GO
    0.9, // Threshold for TGLANG_LANGUAGE_HTML
    0.95, // Threshold for TGLANG_LANGUAGE_JAVA
    0.95, // Threshold for TGLANG_LANGUAGE_JAVASCRIPT
    0.95, // Threshold for TGLANG_LANGUAGE_JSON
    0.95, // Threshold for TGLANG_LANGUAGE_KOTLIN
    0.95, // Threshold for TGLANG_LANGUAGE_LUA
    0.95, // Threshold for TGLANG_LANGUAGE_NGINX
    0.95, // Threshold for TGLANG_LANGUAGE_OBJECTIVE_C
    0.95, // Threshold for TGLANG_LANGUAGE_PHP
    0.95, // Threshold for TGLANG_LANGUAGE_POWERSHELL
    0.95, // Threshold for TGLANG_LANGUAGE_PYTHON
    0.95, // Threshold for TGLANG_LANGUAGE_RUBY
    0.95, // Threshold for TGLANG_LANGUAGE_RUST
    0.95, // Threshold for TGLANG_LANGUAGE_SHELL
    0.95, // Threshold for TGLANG_LANGUAGE_SOLIDITY
    0.95, // Threshold for TGLANG_LANGUAGE_SQL
    0.95, // Threshold for TGLANG_LANGUAGE_SWIFT
    0.95, // Threshold for TGLANG_LANGUAGE_TL
    0.95, // Threshold for TGLANG_LANGUAGE_TYPESCRIPT
    0.95, // Threshold for TGLANG_LANGUAGE_XML
};

/**
 * Detects programming language of a code snippet.
 * \param[in] text Text of a code snippet. A null-terminated string in UTF-8 encoding.
 * \return detected programming language.
 */
TGLANG_EXPORT enum TglangLanguage tglang_detect_programming_language(const char *text);

#ifdef __cplusplus
}
#endif

#endif
