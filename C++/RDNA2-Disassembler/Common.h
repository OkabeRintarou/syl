#pragma once

#define DISALLOW_COPYABLE(ClassName)                    \
    ClassName(const ClassName &) = delete;              \
    ClassName& operator=(const ClassName &) = delete

#define DISALLOW_MOVEABLE(ClassName)                    \
    ClassName(ClassName &&) = delete;                   \
    ClassName &operator=(ClassName &&) = delete
