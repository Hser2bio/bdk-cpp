#ifndef ABI_H
#define ABI_H

#include <string>
#include "../core/utils.h"

#include "../libs/json.hpp"

enum ABITypes {
  uint256, uint256Arr,
  address, addressArr,
  boolean, booleanArr,
  bytes, bytesArr,
  string, stringArr
};

class ABIEncoder {
  private:
    // TODO
  public:
    // TODO
};

class ABIDecoder {
  private:
    std::vector<std::variant<
      uint256_t, std::vector<uint256_t>, Address, std::vector<Address>,
      bool, std::vector<bool>, std::string, std::vector<std::string>
    >> data;  // Both bytes and string are stored as std::string

    // Helper functions to parse each type
    uint256_t decodeUint256(const std::string &data, uint64_t &start);
    Address decodeAddress(const std::string &data, uint64_t &start);
    bool decodeBool(const std::string &data, uint64_t &start);
    std::string decodeBytes(const std::string &data, uint64_t &start);
    std::string decodeString(const std::string &data, uint64_t &start);
    std::vector<uint256_t> decodeUint256Arr(const std::string &data, uint64_t &start);
    std::vector<Address> decodeAddressArr(const std::string &data, uint64_t &start);
    std::vector<bool> decodeBoolArr(const std::string &data, uint64_t &start);
    std::vector<std::string> decodeBytesArr(const std::string &data, uint64_t &start);
    std::vector<std::string> decodeStringArr(const std::string &data, uint64_t &start);

  public:
    ABIDecoder(std::vector<ABITypes> const &types, std::string const &abiData); // Data as *bytes*

    template <typename T> T get(uint64_t const &index) {
      if (index >= data.size()) {
        throw std::out_of_range("Index out of range");
      }
      if (std::holds_alternative<T>(data[index])) {
        return std::get<T>(data[index]);
      }
      throw std::runtime_error("Type mismatch");
    }

    size_t size() { return data.size(); }
};

#endif  // ABI_H
