#include <concepts>
#include <cstddef>
#include <string>
#pragma pack(push, 1)
struct baseHeader {
  size_t HeaderLength;
  baseHeader get() const { return *this; };
  void set(size_t headerLength) { HeaderLength = headerLength; };
  auto next() { return nullptr; }
};
struct baseBody {
  size_t BodyLength;
  auto next() { return nullptr; }
};
struct customHeader {
  baseHeader hd;
  size_t offset;
  customHeader get() const { return *this; }
  void set(size_t ofs) { offset = ofs; }
};
#pragma pack(pop)

template <typename T, typename... Args>
concept PacketRequire = requires(T t, Args... args) {
  { t.get() } -> std::same_as<T>;
  { t.set(args...) } -> std::same_as<void>;
};
