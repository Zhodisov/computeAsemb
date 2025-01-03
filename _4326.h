#ifndef _4326_H
#define _4326_H

#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <stdexcept>

class Packet {
public:
    Packet(const std::vector<uint8_t>& buffer = std::vector<uint8_t>());

    std::vector<uint8_t> getBytes() const;
    std::string toString() const;

    static std::shared_ptr<Packet> create(uint8_t c, uint8_t cc = 0);
    std::shared_ptr<Packet> copy(bool copy_pos = false) const;

    std::vector<uint8_t> readBytes(size_t nbr);
    std::pair<uint8_t, uint8_t> readCode();
    uint8_t read8();
    uint16_t read16();
    uint32_t read24();
    uint32_t read32();
    bool readBool();
    std::vector<uint8_t> readString();
    std::string readUTF();

    Packet& writeBytes(const std::vector<uint8_t>& content);
    Packet& writeCode(uint8_t c, uint8_t cc);
    Packet& write8(uint8_t value);
    Packet& write16(uint16_t value);
    Packet& write24(uint32_t value);
    Packet& write32(uint32_t value);
    Packet& writeBool(bool value);
    Packet& writeString(const std::string& string);
    Packet& writeUTF(const std::string& string);

    Packet& xorCipher(const std::vector<uint8_t>& key, size_t fp, size_t offset = 2);
    Packet& xorDecipher(const std::vector<uint8_t>& key, size_t fp);

private:
    std::vector<uint8_t> buffer;
    size_t pos;
};

#endif 