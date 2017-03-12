#ifndef HYBRIDSPI_UTIL_H_
#define HYBRIDSPI_UTIL_H_

#include <vector>
#include <bitset>
#include <sstream>

template<typename T>
string join(vector<T> items, string sep = ", ") {
    stringstream ss;
    std::for_each(items.begin(), items.end(), [&](T t){ ss << t << sep; });
    return ss.str().substr(0, ss.str().length() - sep.length());
}

template<size_t size>
inline std::vector<unsigned char> bits_to_bytes(bitset<size> bits)
{
    std::vector<unsigned char> bytes(size/8);
    for(int j=0; j<int(size); j++) {
        bytes[j/8] = bytes[j/8] + ((bits[size-j-1] ? 1:0) << (7-j%8));
    }

    return bytes;
}

char const h[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
inline std::string bytes_to_hex(std::vector<unsigned char> bytes)
{
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setfill('0');
    for (int i = 0; i < bytes.size(); i++)
    {
        //ss << std::uppercase << std::setw(2) << static_cast<unsigned>(bytes[i]);
        ss << std::setw(2) << static_cast<unsigned int>(bytes[i]);
        if(i < bytes.size() - 1) ss << ' ';
    }
    return ss.str();
}

template <int size>
inline std::bitset<size> bitstring_to_bits(std::string string)
{
    std::bitset<size> bits;
    for(int i=0; i < bits.count(); i++) {
        bits.setBit(i, string[i] == '0' ? false : true);
    }
    return bits;
}

inline std::vector<unsigned char> operator+(const std::vector<unsigned char> &a, const std::vector<unsigned char> &b)
{
    std::vector<unsigned char> result(b);
    result.insert(result.begin(), a.begin(), a.end());
    return result;
}

inline std::ostream& operator<< (std::ostream& stream, const std::vector<unsigned char> bytes) {
    std::string text(bytes.begin(), bytes.end());
    stream << text;
    return stream;
}

inline std::ostream& operator<< (std::ostream& stream, const std::bitset<8>& bits) {
    std::string text;
    for (size_t i = 0; i < bits.size(); ++i)
        text.insert(0, bits[i] ? "1": "0");
    stream << text;
    return stream;
}

inline std::ostream& operator<< (std::ostream& stream, const std::bitset<16>& bits) {
    std::string text;
    for (size_t i = 0; i < bits.size(); ++i)
        text.insert(0, bits[i] ? "1": "0");
    stream << text;
    return stream;
}

inline std::ostream& operator<< (std::ostream& stream, const std::bitset<24>& bits) {
    std::string text;
    for (size_t i = 0; i < bits.size(); ++i)
        text.insert(0, bits[i] ? "1": "0");
    stream << text;
    return stream;
}

inline std::ostream& operator<< (std::ostream& stream, const std::bitset<32>& bits) {
    std::string text;
    for (size_t i = 0; i < bits.size(); ++i)
        text.insert(0, bits[i] ? "1": "0");
    stream << text;
    return stream;
}

inline std::ostream& operator<< (std::ostream& stream, const std::bitset<48>& bits) {
    std::string text;
    for (size_t i = 0; i < bits.size(); ++i)
        text.insert(0, bits[i] ? "1": "0");
    stream << text;
    return stream;
}

inline std::ostream& operator<< (std::ostream& stream, const std::bitset<56>& bits) {
    std::string text;
    for (size_t i = 0; i < bits.size(); ++i)
        text.insert(0, bits[i] ? "1": "0");
    stream << text;
    return stream;
}

inline std::ostream& operator<< (std::ostream& stream, const std::bitset<64>& bits) {
    std::string text;
    for (size_t i = 0; i < bits.size(); ++i)
        text.insert(0, bits[i] ? "1": "0");
    stream << text;
    return stream;
}

#endif // HYBRIDSPI_UTIL_H
