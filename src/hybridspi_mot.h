#ifndef HYBRIDSPI_MOT_H_
#define HYBRIDSPI_MOT_H_

#include <mot.h>

using namespace mot;

namespace spi 
{

    namespace ContentTypes
    {
        const ContentType ServiceInformation = {7, 0};
        const ContentType ProgrammeInformation = {7, 1};
        const ContentType GroupInformation = {7, 2};
    }

    class ScopeStart : public HeaderParameter
    {

    public:

        ScopeStart(int timepoint);

        vector<unsigned char> encodeData() const;

    protected:

        bool equals(const HeaderParameter& a) const;

    private:

        int timepoint;
    };

    class ScopeEnd : public HeaderParameter
    {

    public:

        ScopeEnd(int timepoint);

        vector<unsigned char> encodeData() const;

    protected:

        bool equals(const HeaderParameter& a) const;

    private:

        int timepoint;
    };

    class ScopeId : public HeaderParameter
    {

    public:

        ScopeId(int ecc, int eid);

        vector<unsigned char> encodeData() const;

    protected:

        bool equals(const HeaderParameter& a) const;

    private:

        int ecc;

        int eid;
    };

}

#endif /* HYBRIDSPI_MOT_H_ */
