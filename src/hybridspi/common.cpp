#include <hybridspi/common.h>

#include <sstream>
#include <string>
#include <iostream>

namespace hybridspi
{

    vector<unsigned char> operator+(const vector<unsigned char> &a, const vector<unsigned char> &b)
    {
        vector<unsigned char> result(b);
        result.insert(result.begin(), a.begin(), a.end());
        return result;
    }

    DateTime make_datetime(string text, string format)
    {
        tm t = {};
        stringstream ss;
        ss << text;
        ss >> get_time(&t, format.c_str());
        auto epoch = mktime(&t);
        return Clock::from_time_t(epoch);
    }

    DateTime make_datetime(int year, int month, int day, int hour,
                           int min, int sec, int usec, int isDST)
    {
        tm t;
        t.tm_year = year-1900;
        t.tm_mon = month-1;
        t.tm_mday = day;
        t.tm_hour = hour;
        t.tm_min = min;
        t.tm_sec = sec;
        t.tm_isdst = isDST;
        auto epoch = mktime(&t);
        return Clock::from_time_t(epoch) + microseconds{usec};
    }    
    
    BaseText::BaseText(string text, int max_length)
        : text(text), max_length(max_length)
    {
        if(text.length() > max_length)
        {
            throw std::length_error("text is longer than the max_length: " + text);
        }        
    }
    
    bool BaseText::operator== (const BaseText &that) const
    {
        return text == that.text;
    }
    
    bool BaseText::operator!= (const BaseText &that) const
    {
        return !(*this == that);
    }
    
    Name::Name(string text, int max_length)
        : BaseText(text, max_length)
    { }
    
    LongName::LongName(string text) 
        : Name(text, 128)
    { }
    
    MediumName::MediumName(string text)
        : Name(text, 16)
    { }
    
    ShortName::ShortName(string text)
        : Name(text, 8)
    { }
    
    Link::Link(string uri, string content, string description)
        : uri(uri), content(content), description(description)
    { }
    
    bool Link::operator== (const Link &that) const
    {
        return uri == that.uri;
    }
    
    bool Link::operator!= (const Link &that) const
    {
        return !(*this == that);
    }   
    
    ostream& operator<< (ostream& stream, const Link& link) 
    {
        stream << link.URI();
        return stream;
    }
    
    Named::Named()
    { }
    
    vector<Name> Named::Names(int max_length) const
    {
        vector<Name> result;
        for(auto &name : names)
        {
            if(name.Text().length() <= max_length) result.push_back(name);
        }
        
        return result;
    }
    
    void Named::AddName(const Name &name)
    {
        names.push_back(name);
    }
    
    void Named::RemoveName(const Name &name)
    {
        auto it = std::find(names.begin(), names.end(), name);
        if(it != names.end())
            names.erase(it);
    }  
    
    Description::Description(string text, int max_length)
        : BaseText(text, max_length)
    { } 
    
    ShortDescription::ShortDescription(string text)
        : Description(text, 180)
    { }
    
    LongDescription::LongDescription(string text)
        : Description(text, 1024)
    { }
    
    Described::Described()
    { }
    
    vector<Description> Described::Descriptions(int max_length) const
    {
        vector<Description> result;
        for(auto &description : descriptions)
        {
            if(description.Text().length() <= max_length) result.push_back(description);
        }
        
        return result;
    }
    
    void Described::AddDescription(const Description &description)
    {
        descriptions.push_back(description);
    }
    
    void Described::RemoveDescription(const Description &description)
    {
        auto it = std::find(descriptions.begin(), descriptions.end(), description);
        if(it != descriptions.end())
            descriptions.erase(it);
    }
    
    Linked::Linked()
    { }
    
    void Linked::AddLink(const Link &link)
    {
        links.push_back(link);
    }
    
    void Linked::RemoveLink(const Link &link)
    {
        auto it = std::find(links.begin(), links.end(), link);
        if(it != links.end())
            links.erase(it);
    }  
    
    void Keyworded::AddKeyword(const string & keyword)
    {
        keywords.push_back(keyword);
    }
    
    void Keyworded::AddKeywords(const string &keywords)
    {
        auto t = strtok(const_cast<char*>(keywords.c_str()), ",");
        while(t != NULL)
        {
            this->AddKeyword(t);
            t = strtok(NULL, ",");
        }
    }
    
    void Keyworded::RemoveKeyword(const string &keyword)
    {
        auto it = std::find(keywords.begin(), keywords.end(), keyword);
        if(it != keywords.end())
            keywords.erase(it);            
    } 
    
    MediaEnabled::MediaEnabled()
    { }
    
    void MediaEnabled::AddMedia(const Multimedia &multimedia)
    {
        media.push_back(multimedia);
    }
    
    void MediaEnabled::RemoveMedia(const Multimedia &multimedia)
    {
        auto it = std::find(media.begin(), media.end(), multimedia);
        if(it != media.end())
            media.erase(it);
    } 
    
    Genred::Genred()
    { }

    void Genred::AddGenre(const Genre &genre)
    {
        genres.push_back(genre);
    }
    
    void Genred::RemoveGenre(const Genre &genre)
    {
        auto it = std::find(genres.begin(), genres.end(), genre);
        if(it != genres.end())
            genres.erase(it);
    }              
    
    bool GeoLocation::operator== (const GeoLocation &that) const
    {
        return equals(that);
    }
    
    bool GeoLocation::operator!= (const GeoLocation &that) const
    {
        return !(*this == that);
    }    
    
    void GeoLocated::AddGeoLocation(GeoLocation* geolocation)
    {
        geolocations.push_back(geolocation);
    }
    
    void GeoLocated::RemoveGeoLocation(GeoLocation* geolocation)
    {
        auto it = std::find(geolocations.begin(), geolocations.end(), geolocation);
        if(it != geolocations.end())
            geolocations.erase(it);
    }
    
    bool Point::equals(const GeoLocation& other) const
    {
        const Point* that = dynamic_cast<const Point*>(&other);
        return that != nullptr && 
        (this->latitude == that->latitude && this->longitude == that->longitude);
    }
    
    bool CountryCode::equals(const GeoLocation& other) const
    {
        const CountryCode* that = dynamic_cast<const CountryCode*>(&other);
        return that != nullptr && 
        (this->code == that->code);
    }
    
    bool Polygon::equals(const GeoLocation& other) const
    {
        const Polygon* that = dynamic_cast<const Polygon*>(&other);
        return that != nullptr &&
        (this->points == that->points);
    }
    
    Membership::Membership(string id, unsigned short shortId, unsigned short index)
        : id(id), shortId(shortId), index(index)
    { }
            
    bool Membership::operator== (const Membership &that) const
    {
        return (id == that.id &&
                shortId == that.shortId &&
                index == that.index);
    }
            
    bool Membership::operator!= (const Membership &that) const
    {
        return !(*this == that);        
    }     
    
    Multimedia::Multimedia(string location, string content)
        : location(location), content(content)
    { }
    
    Multimedia::Multimedia(string location, string content, int width, int height)
        : location(location), content(content), width(width), height(height)
    { }
    
    bool Multimedia::operator== (const Multimedia &that) const
    {
        return (location == that.location &&
                content == that.content &&
                width == that.width &&
                height == that.height);
    }
    
    bool Multimedia::operator!= (const Multimedia &that) const
    {
        return !(*this == that);
    }  
    
    ostream& operator<< (ostream& stream, const Multimedia& media) 
    {
        stream << media.Location();
        return stream;
    }        
      
    Genre::Genre(string href)
        : href(href)
    { }
    
    Genre::Genre(string href, string name)
        : href(href), name(name)
    { }
    
    bool Genre::operator== (const Genre &that) const
    {
        return this->href == that.href;
    }
    
    bool Genre::operator!= (const Genre &that) const
    {
        return !(*this == that);
    }
    
    ostream& operator<< (ostream& stream, const Genre& genre) 
    {
        stream << genre.Href();
        return stream;
    }    
    
    Bearer::Bearer(int cost, int offset)
        : GeoLocated(), cost(cost), offset(offset)
    { }
    
    bool Bearer::operator== (const Bearer &that) const
    {
        return equals(that);
    }
    
    bool Bearer::operator!= (const Bearer &that) const
    {
        return !(*this == that);
    }  
    
    DigitalBearer::DigitalBearer(string content, int cost, int offset)
        : Bearer(cost, offset), content(content)
    { }
    
    DabBearer::DabBearer(int ecc, int eid, int sid, int scids, string content, int cost, int offset)
        : DigitalBearer(content, cost, offset), ecc(ecc), eid(eid), sid(sid), scids(scids)
    { }
    
    DabBearer::DabBearer(string uri, string content, int cost, int offset)
        : DigitalBearer(content, cost, offset)
    {
        smatch match;
        regex_search(uri, match, BEARER_REGEX);
        if(match.size() <= 4)
        {
            // throw some kind of error? have to check whether throwing an error in an
            // constructor is kosher
        }
        
        ecc = stoul(match.str(1).substr(1), nullptr, 16);
        eid = stoul(match.str(2), nullptr, 16);
        sid = stoul(match.str(3), nullptr, 16);
        scids = stoul(match.str(4), nullptr, 16);        
    }
    
    string DabBearer::URI() const
    {   
        char* x = new char[17];
        sprintf(x, "dab:%03x.%04x.%04x.%01d", (eid >> 4 & 0xf00) + ecc, eid, sid, scids);
        string s(x);
        return s;        
    }
    
    bool DabBearer::equals(const Bearer& other) const
    {
        const DabBearer* that = dynamic_cast<const DabBearer*>(&other);
        return that != nullptr && 
            (this->ecc == that->ecc &&
             this->eid == that->eid &&
             this->sid == that->sid &&
             this->scids == that->scids);
    }
    
    FmBearer::FmBearer(int ecc, int pi, int frequency, int cost, int offset)
        : Bearer(cost, offset), ecc(ecc), pi(pi), frequency(frequency)
    { }
    
    FmBearer::FmBearer(string uri, int cost, int offset)
        : Bearer(cost, offset)
    {
        smatch match;
        regex_search(uri, match, BEARER_REGEX);
        if(match.size() <= 3)
        {
            // throw some kind of error? have to check whether throwing an error in an
            // constructor is kosher
        }
        
        ecc = stoul(match.str(1).substr(1), nullptr, 16);
        pi = stoul(match.str(2), nullptr, 16);
        frequency = stoul(match.str(3), nullptr)*10;
    }
    
    string FmBearer::URI() const
    {
        char* x = new char[17];
        sprintf(x, "fm:%03x.%04x.%05d", (pi >> 4 & 0xf00) + ecc, pi, frequency/10);
        string s(x);
        return s;
    }
    
    bool FmBearer::equals(const Bearer &other) const
    {
        const FmBearer* that = dynamic_cast<const FmBearer*>(&other);
        return that != nullptr && 
            (this->ecc == that->ecc &&
             this->pi == that->pi &&
             this->frequency == that->frequency);
    }
    
    IpBearer::IpBearer(string uri, string content, int cost, int offset)
        : DigitalBearer(content, cost, offset), uri(uri)
    { }
    
    bool IpBearer::equals(const Bearer &other) const
    {
        const IpBearer* that = dynamic_cast<const IpBearer*>(&other);
        return that != nullptr && 
            (this->uri == that->uri);
    }    
   
    
}
