#ifndef HYBRIDSPI_MOT_H_
#define HYBRIDSPI_MOT_H_

#include <mot.h>
#include <hybridspi/common.h>

using namespace mot;

namespace spi 
{

    struct content_id
    {
        unsigned int ecc : 8;
        unsigned int eid : 16;
        unsigned int sid : 32;
        unsigned int scids : 4;
    };

    class unknown_content_id: public std::runtime_error
    {
        public:
        
            unknown_content_id(content_id const& scope):
                std::runtime_error("Unknown content id")
            {}
    };

    bool operator==(const content_id& lhs, const content_id& rhs) {
       return lhs.ecc == rhs.ecc &&
              lhs.eid == rhs.eid &&
              lhs.sid == rhs.sid &&
              lhs.scids == rhs.scids; 
    }

    namespace ContentTypes
    {
        const ContentType ServiceInformation = {7, 0};
        const ContentType ProgrammeInformation = {7, 1};
        const ContentType GroupInformation = {7, 2};
    }

    class ScopeStart : public HeaderParameter
    {

    public:

        ScopeStart(DateTime timepoint);

        vector<unsigned char> encodeData() const;

    protected:

        bool equals(const HeaderParameter& a) const;

    private:

        DateTime timepoint;
    };

    class ScopeEnd : public HeaderParameter
    {

    public:

        ScopeEnd(DateTime timepoint);

        vector<unsigned char> encodeData() const;

    protected:

        bool equals(const HeaderParameter& a) const;

    private:

        DateTime timepoint;
    };

    class ScopeId : public HeaderParameter
    {

    public:

        ScopeId(content_id scope);

        vector<unsigned char> encodeData() const;

    protected:

        bool equals(const HeaderParameter& a) const;

    private:

        content_id scope;
    };

}

#endif