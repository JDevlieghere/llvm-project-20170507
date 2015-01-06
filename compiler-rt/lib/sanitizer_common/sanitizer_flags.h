//===-- sanitizer_flags.h ---------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is a part of ThreadSanitizer/AddressSanitizer runtime.
//
//===----------------------------------------------------------------------===//

#ifndef SANITIZER_FLAGS_H
#define SANITIZER_FLAGS_H

#include "sanitizer_internal_defs.h"

namespace __sanitizer {

void ParseFlag(const char *env, bool *flag,
    const char *name, const char *descr);
void ParseFlag(const char *env, int *flag,
    const char *name, const char *descr);
void ParseFlag(const char *env, uptr *flag,
    const char *name, const char *descr);
void ParseFlag(const char *env, const char **flag,
    const char *name, const char *descr);

struct CommonFlags {
#define COMMON_FLAG(Type, Name, DefaultValue, Description) Type Name;
#include "sanitizer_flags.inc"
#undef COMMON_FLAG

  void SetDefaults();
  void ParseFromString(const char *str);
  void CopyFrom(const CommonFlags &other);
};

// Functions to get/set global CommonFlags shared by all sanitizer runtimes:
extern CommonFlags common_flags_dont_use;
inline const CommonFlags *common_flags() {
  return &common_flags_dont_use;
}

inline void SetCommonFlagsDefaults() {
  common_flags_dont_use.SetDefaults();
}

inline void ParseCommonFlagsFromString(const char *str) {
  common_flags_dont_use.ParseFromString(str);
}

// This function can only be used to setup tool-specific overrides for
// CommonFlags defaults. Generally, it should only be used right after
// SetCommonFlagsDefaults(), but before ParseCommonFlagsFromString(), and
// only during the flags initialization (i.e. before they are used for
// the first time).
inline void OverrideCommonFlags(const CommonFlags &cf) {
  common_flags_dont_use.CopyFrom(cf);
}

void PrintFlagDescriptions();

}  // namespace __sanitizer

#endif  // SANITIZER_FLAGS_H
