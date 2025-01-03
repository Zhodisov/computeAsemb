#include "_4326.h"
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <sstream>

Packet::Packet(const std::vector<uint8_t>& buffer)
    : buffer(buffer), pos(0) {}

std::vector<uint8_t> Packet::getBytes() const {
    return buffer;
}

std::string Packet::toString() const {
    std::ostringstream oss;
    oss << "<Packet ";
    for (const auto& byte : buffer) {
        oss << static_cast<int>(byte) << " ";
    }
    oss << ">";
    return oss.str();
}

std::shared_ptr<Packet> Packet::create(uint8_t c, uint8_t cc) {
    auto packet = std::make_shared<Packet>();
    packet->write8(c).write8(cc);
    return packet;
}

std::shared_ptr<Packet> Packet::copy(bool copy_pos) const {
    auto p = std::make_shared<Packet>(buffer);
    if (copy_pos) {
        p->pos = pos;
    }
    return p;
}

std::vector<uint8_t> Packet::readBytes(size_t nbr) {
    pos += nbr;
    return std::vector<uint8_t>(buffer.begin() + pos - nbr, buffer.begin() + pos);
}

std::pair<uint8_t, uint8_t> Packet::readCode() {
    return { read8(), read8() };
}

uint8_t Packet::read8() {
    return buffer[pos++];
}

uint16_t Packet::read16() {
    auto data = readBytes(2);
    return static_cast<uint16_t>((data[0] << 8) | data[1]);
}

uint32_t Packet::read24() {
    auto data = readBytes(3);
    return static_cast<uint32_t>((data[0] << 16) | (data[1] << 8) | data[2]);
}

uint32_t Packet::read32() {
    auto data = readBytes(4);
    return static_cast<uint32_t>((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
}

bool Packet::readBool() {
    return read8() == 1;
}

std::vector<uint8_t> Packet::readString() {
    auto length = read16();
    return readBytes(length);
}

std::string Packet::readUTF() {
    auto bytes = readString();
    return std::string(bytes.begin(), bytes.end());
}

Packet& Packet::writeBytes(const std::vector<uint8_t>& content) {
    buffer.insert(buffer.end(), content.begin(), content.end());
    return *this;
}

Packet& Packet::writeCode(uint8_t c, uint8_t cc) {
    return write8(c).write8(cc);
}

Packet& Packet::write8(uint8_t value) {
    buffer.push_back(value);
    return *this;
}

Packet& Packet::write16(uint16_t value) {
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back(value & 0xFF);
    return *this;
}

Packet& Packet::write24(uint32_t value) {
    buffer.push_back((value >> 16) & 0xFF);
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back(value & 0xFF);
    return *this;
}

Packet& Packet::write32(uint32_t value) {
    buffer.push_back((value >> 24) & 0xFF);
    buffer.push_back((value >> 16) & 0xFF);
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back(value & 0xFF);
    return *this;
}

Packet& Packet::writeBool(bool value) {
    return write8(value ? 1 : 0);
}

Packet& Packet::writeString(const std::string& string) {
    auto bytes = std::vector<uint8_t>(string.begin(), string.end());
    return write16(static_cast<uint16_t>(bytes.size())).writeBytes(bytes);
}

Packet& Packet::writeUTF(const std::string& string) {
    return writeString(string);
}

Packet& Packet::xorCipher(const std::vector<uint8_t>& key, size_t fp, size_t offset) {
    for (size_t i = offset; i < buffer.size(); ++i) {
        buffer[i] ^= key[(fp + i - offset + 1) % 20];
    }
    return *this;
}

Packet& Packet::xorDecipher(const std::vector<uint8_t>& key, size_t fp) {
    return xorCipher(key, fp, 3);
}
