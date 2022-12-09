#pragma once

#include <stdlib.h>
#include <string.h>

class BufferEx
{
public:
  BufferEx() = default;
  ~BufferEx() { free(buffer_); }

  void Write(const char* data, size_t len)
  {
    Prepare(len);
    memcpy(buffer_ + offset_, data, len);
    offset_ += len;
  }

  void Reset() { offset_ = 0; }

  const char* Data() const { return buffer_; }
  size_t Size() const { return size_; }

private:
  void Prepare(size_t len)
  {
    if (size_ - offset_ >= len) {
      return;
    }

    len = ((len - (size_ - offset_)) / chunk) * chunk + chunk + size_;
    if (buffer_ == nullptr) {
      buffer_ = (char*)malloc(len);
    } else {
      buffer_ = (char*)realloc(buffer_, len);
    }

    size_ = len;
  }

private:
  char* buffer_ {nullptr};
  size_t size_ {0};
  size_t offset_ {0};

  static const size_t chunk {1024};
};
