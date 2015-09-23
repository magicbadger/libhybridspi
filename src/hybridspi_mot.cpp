#include "hybridspi_mot.h"
#include "mot.h"

using namespace spi;

ScopeStart::ScopeStart(int timepoint)
    : HeaderParameter(0x25), timepoint(timepoint)
{ }

bool ScopeStart::equals(const HeaderParameter& other) const
{
    const ScopeStart* that = dynamic_cast<const ScopeStart*>(&other);
    return that != nullptr && (this->timepoint == that->timepoint);
}

vector<unsigned char> ScopeStart::encodeData() const
{
    return mot::timepoint_to_encoded_utc(timepoint);
}

ScopeEnd::ScopeEnd(int timepoint)
    : HeaderParameter(0x26), timepoint(timepoint)
{ }

bool ScopeEnd::equals(const HeaderParameter& other) const
{
    const ScopeEnd* that = dynamic_cast<const ScopeEnd*>(&other);
    return that != nullptr && (this->timepoint == that->timepoint);
}

vector<unsigned char> ScopeEnd::encodeData() const
{
    return mot::timepoint_to_encoded_utc(timepoint);
}

ScopeId::ScopeId(int ecc, int eid)
    : HeaderParameter(0x27), ecc(ecc), eid(eid)
{ }

bool ScopeId::equals(const HeaderParameter& other) const
{
    const ScopeId* that = dynamic_cast<const ScopeId*>(&other);
    return that != nullptr && (this->ecc == that->ecc) && (this->eid == that->ecc);
}

vector<unsigned char> ScopeId::encodeData() const
{
    bitset<24> bits(eid + // EId (16)
                   (ecc << 16)); // ECC(8)

    vector<unsigned char> bytes = bits_to_bytes(bits);

    return bytes;
}
