#include <hybridspi/mot.h>
#include <hybridspi/util.h>
#include <hybridspi/binary.h>
#include <bitset>


namespace hybridspi
{

    ScopeStart::ScopeStart(DateTime timepoint)
        : HeaderParameter(0x25), timepoint(timepoint)
    { }

    bool ScopeStart::equals(const HeaderParameter& other) const
    {
        const ScopeStart* that = dynamic_cast<const ScopeStart*>(&other);
        return that != nullptr && (this->timepoint == that->timepoint);
    }

    vector<unsigned char> ScopeStart::EncodeData() const
    {
        return hybridspi::binary::encode_timepoint(timepoint);
    }

    ScopeEnd::ScopeEnd(DateTime timepoint)
        : HeaderParameter(0x26), timepoint(timepoint)
    { }

    bool ScopeEnd::equals(const HeaderParameter& other) const
    {
        const ScopeEnd* that = dynamic_cast<const ScopeEnd*>(&other);
        return that != nullptr && (this->timepoint == that->timepoint);
    }

    vector<unsigned char> ScopeEnd::EncodeData() const
    {
        return hybridspi::binary::encode_timepoint(timepoint);
    }

    ScopeId::ScopeId(content_id scope)
        : HeaderParameter(0x27), scope(scope)
    { }

    ScopeId::ScopeId(unsigned int ecc, unsigned int eid)
        : HeaderParameter(0x27), scope(content_id({ecc, eid}))
    { }

    bool ScopeId::equals(const HeaderParameter& other) const
    {
        const ScopeId* that = dynamic_cast<const ScopeId*>(&other);
        return that != nullptr && this->scope == that->scope;
    }

    vector<unsigned char> ScopeId::EncodeData() const
    {
        bitset<24> bits(scope.eid + // EId (16)
                        ((scope.ecc) << 16)); // ECC (8)
        return bits_to_bytes(bits);
    }
}
