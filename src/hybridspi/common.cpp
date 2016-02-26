#include <hybridspi/common.h>

#include <sstream>

namespace hybridspi
{
    
    BaseText::BaseText(string text, int max_length)
        : text(text), max_length(max_length)
    {
        if(text.length() > max_length)
        {
            throw std::length_error("text");
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
    
    Link::Link(string url, string content, string description)
        : url(url), content(content), description(description)
    { }
    
    bool Link::operator== (const Link &that) const
    {
        return url == that.url;
    }
    
    bool Link::operator!= (const Link &that) const
    {
        return !(*this == that);
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
    
    void Named::AddName(Name name)
    {
        names.push_back(name);
    }
    
    void Named::RemoveName(const Name &name)
    {
        auto it = std::find(names.begin(), names.end(), name);
        if(it != names.end())
            names.erase(it);
    }   
    
    ShortDescription::ShortDescription(string text)
        : Description(text, 128)
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
    
    void Described::AddDescription(Description description)
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
    
    void Linked::AddLink(Link link)
    {
        links.push_back(link);
    }
    
    void Linked::RemoveLink(const Link &link)
    {
        auto it = std::find(links.begin(), links.end(), link);
        if(it != links.end())
            links.erase(it);
    }  
    
    void Keyworded::AddKeyword(string keyword)
    {
        keywords.push_back(keyword);
    }
    
    void Keyworded::RemoveKeyword(const string &keyword)
    {
        auto it = std::find(keywords.begin(), keywords.end(), keyword);
        if(it != keywords.end())
            keywords.erase(it);            
    } 
    
    MediaEnabled::MediaEnabled()
    { }
    
    void MediaEnabled::AddMedia(Multimedia multimedia)
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

    void Genred::AddGenre(Genre genre)
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
    
    void GeoLocated::AddLocation(GeoLocation* geolocation)
    {
        geolocations.push_back(geolocation);
    }
    
    void GeoLocated::RemoveLocation(GeoLocation* geolocation)
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
    
    Membership::Membership(short shortcrid, string crid, short index = 1)
        : shortcrid(shortcrid), crid(crid), index(index)
    { }
    
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
    
    DigitalBearer::DigitalBearer(int bitrate, string content, int cost, int offset)
        : Bearer(cost, offset), bitrate(bitrate), content(content)
    { }
    
    DabBearer::DabBearer(int ecc, int eid, int sid, int scids, int bitrate, string content, int cost, int offset)
        : DigitalBearer(bitrate, content, cost, offset), ecc(ecc), eid(eid), sid(sid), scids(scids)
    { }
    
    string DabBearer::URI() const
    {
        stringstream ss;
        ss << "dab://" << ecc << "." << eid << "." << sid << "." << scids;
        return ss.str();
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
        : Bearer(cost, offset), ecc(ecc), pi(pi)
    { }
    
    string FmBearer::URI() const
    {
        stringstream ss;
        ss << "fm://" << ecc << "." << pi << "." << frequency;
        return ss.str();
    }
    
    bool FmBearer::equals(const Bearer &other) const
    {
        const FmBearer* that = dynamic_cast<const FmBearer*>(&other);
        return that != nullptr && 
            (this->ecc == that->ecc &&
             this->pi == that->pi &&
             this->frequency == that->frequency);
    }
    
    IpBearer::IpBearer(string uri, int bitrate, string content, int cost, int offset)
        : DigitalBearer(bitrate, content, cost, offset), uri(uri)
    { }
    
    bool IpBearer::equals(const Bearer &other) const
    {
        const IpBearer* that = dynamic_cast<const IpBearer*>(&other);
        return that != nullptr && 
            (this->uri == that->uri);
    }    
    
}
