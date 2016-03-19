#ifndef HYBRIDSPI_COMMON_H
#define HYBRIDSPI_COMMON_H

#include <chrono>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

using DateTime = time_point<system_clock>;
using Duration = seconds;

using bytes = vector<unsigned char>;

const unsigned int DEFAULT_COST = 0;
const unsigned int DEFAULT_OFFSET = 0;

namespace hybridspi
{
    
    class BaseText
    {
        public:
        
            BaseText(string text, int max_length);
            
            string Text() const { return text; }
            
            int MaxLength() const { return max_length; }
            
            bool operator== (const BaseText &that) const;
            
            bool operator!= (const BaseText &that) const;            
            
        private:
            
            string text;
            
            int max_length;
           
    };
    
    class Name : public BaseText
    {
        public:
        
            Name(string text, int max_length);
        
    };    
    
    class LongName : public Name
    {
        public:
        
            LongName(string text);
    };
    
    class MediumName : public Name
    {
        public:
        
            MediumName(string text);
    };
    
    class ShortName : public Name
    {
        public:
        
            ShortName(string text);
    };
    
    class Named
    {
        public:
        
            Named();
            
            vector<Name> Names() const { return names; };
            
            vector<Name> Names(int maxlength) const;
            
            void AddName(Name name);
            
            void RemoveName(const Name &name);
            
        private:
        
            vector<Name> names;
    };    
    
    class Description : public BaseText
    {
        public:
        
            Description(string text, int max_length);
        
    };
    
    class ShortDescription : public Description
    {
        public:
        
            ShortDescription(string text);
        
    };
    
    class LongDescription : public Description
    {
        public:
        
            LongDescription(string text);
    };
    
    class GeoLocation
    {
        public:
                    
            bool operator== (const GeoLocation &that) const;
            
            bool operator!= (const GeoLocation &that) const;    
      
      protected:
      
            virtual bool equals(const GeoLocation& a) const = 0;
        
    };
    
    class Point : public GeoLocation
    {
        public:
        
            Point(double latitude, double longitude);
            
            double Latitude() { return latitude; }
            
            double Longitude() { return longitude; }
            
        protected:
        
            bool equals(const GeoLocation& a) const;
            
        private:
        
            double latitude, longitude;
               
    };
    
    class CountryCode : public GeoLocation
    {
        
        public:
        
            CountryCode(string code);
            
            string Code() { return code; };
        
        protected:
        
            bool equals(const GeoLocation& a) const;
            
        private:
        
            string code;
        
    };
    
    class Polygon : public GeoLocation
    {
        
        public:
        
            Polygon(vector<Point> points);
            
            vector<Point>::iterator Points() { return points.begin(); };
        
        protected:
        
            bool equals(const GeoLocation& a) const;
        
        private:
        
            vector<Point> points;
        
    };
    
    class GeoLocated
    {
        public:
            
            vector<GeoLocation*>::iterator Locations() { return geolocations.begin(); };
            
            void AddLocation(GeoLocation* geolocation);
            
            void RemoveLocation(GeoLocation* geolocation);
            
        private:
        
            vector<GeoLocation*> geolocations;
            
    };
    
    class Bearer : public GeoLocated
    {
        public:
        
            Bearer(int cost, int offset);
            
            int Cost() { return cost; }
            
            int Offset() { return offset; }
            
            bool operator== (const Bearer &that) const;
            
            bool operator!= (const Bearer &that) const;  
            
            virtual string URI() const = 0;  
            
        protected:
      
            virtual bool equals(const Bearer& a) const = 0;                      
            
        private:
            
            int cost;
            
            int offset;
    };
    
    class DigitalBearer : public Bearer
    {
        public:
        
            DigitalBearer(string content, int cost, int offset);
            
            void SetBitrate(int bitrate)
            {
                this->bitrate = bitrate;
            }
            
            int Bitrate() { return bitrate; };
            
            string Content() { return content; };
            
            virtual string URI() const = 0;  
            
        protected:
        
            virtual bool equals(const Bearer& a) const = 0;                              
            
        private:
            
            int bitrate;
            
            string content;
    };
    
    class DabBearer : public DigitalBearer
    {
        public:
        
            DabBearer(int ecc, int eid, int sid, int scids, string content, int cost = DEFAULT_COST, int offset = DEFAULT_OFFSET);
            
            unsigned int ECC() { return ecc; };
            
            unsigned int EId() { return eid; };
            
            unsigned int SId() { return sid; };
            
            unsigned int SCIdS() { return scids; };
            
            string URI() const; 
            
        private:
        
            bool equals(const Bearer& a) const;
        
            unsigned int ecc;
            
            unsigned int eid;
            
            unsigned int sid;
            
            unsigned int scids;
    };
    
    class FmBearer : public Bearer
    {
        public:
        
            FmBearer(int ecc, int pi, int frequency, int cost = DEFAULT_COST, int offset = DEFAULT_OFFSET);
            
            int ECC() { return ecc; };
            
            int PI() { return pi; };
            
            int Frequency() { return frequency; };
            
            string URI() const;             
            
        private: 
        
            bool equals(const Bearer& a) const;
        
            int ecc;
            
            int pi;
            
            int frequency;
    };
    
    class IpBearer : public DigitalBearer
    {
        public:
        
            IpBearer(string uri, string content, int cost, int offset);
            
            string URI() const { return uri; };
            
        private:
        
            bool equals(const Bearer& a) const;
        
            string uri;
            
    };
    
    class Link
    {
        public:
        
            Link(string uri, string content = "", string description = "");
            
            string URI() { return uri; };
            
            Link* SetContent(string content)
            {
                this->content = content;
                return this;
            }
            
            string Content() { return content; };
            
            Link* SetDescription(string description)
            {
                this->description = description;
                return this;
            }
            
            string Description() { return description; };
            
            bool operator== (const Link &that) const;
            
            bool operator!= (const Link &that) const;                       
            
        private:
        
            string uri;
            
            string content;
            
            string description;
    };
    
    class AbsoluteTime
    {
        public:
        
            AbsoluteTime(DateTime billedTime, Duration billedDuration, DateTime actualTime, Duration actualDuration);
            
            DateTime BilledTime() { return billedTime; };
            
            DateTime ActualTime() { return actualTime; };
            
            Duration BilledDuration() { return billedDuration; };
            
            Duration ActualDuration() { return actualDuration; };
            
        private:
        
            DateTime billedTime, actualTime;
            
            Duration billedDuration, actualDuration;
            
    };
    
    class Membership
    {
        public:
        
            Membership(short shortcrid, string crid, short index);
            
            short ShortCrid() { return shortcrid; };
            
            string Crid() { return crid; };
            
            short Index() { return index; };
        
        private:
        
            short shortcrid;
            
            string crid;
            
            short index;
        
    };
    
    class Multimedia
    {
        public:
        
            Multimedia(string location, string content);    
        
            Multimedia(string location, string content, int width, int height);
            
            string Location() { return location; };
            
            string Content() { return content; };
            
            int Height() { return height; };
            
            int Width() { return width; };
            
            bool operator== (const Multimedia &that) const;
            
            bool operator!= (const Multimedia &that) const;                  
            
        private:
            
            string location;
            
            string content;
            
            int height;
            
            int width;
                  
    };
    
    class Genre
    {
        public:
        
            Genre(string href);
            
            Genre(string href, string name);
            
            string Href() { return href; };
            
            string Name() { return name; };
            
            bool operator== (const Genre &that) const;
            
            bool operator!= (const Genre &that) const;                
            
        private:
        
            string href;
            
            string name;
    };
    
   class Described
    {
        public:
        
            Described();
            
            vector<Description> Descriptions() const { return descriptions; };
            
            vector<Description> Descriptions(int max_length) const;
            
            void AddDescription(Description description);
            
            void RemoveDescription(const Description &description);
            
        private:
        
            vector<Description> descriptions;
    };
    
    class Linked
    {
        public:
        
            Linked();
            
            vector<Link> Links() const { return links; };
            
            void AddLink(Link link);
            
            void RemoveLink(const Link &link);
            
        private:
        
            vector<Link> links;        
    };
    
    class Keyworded
    {
        public:
        
            void AddKeyword(string keyword);
        
            void AddKeywords(string keywords);
            
            void RemoveKeyword(const string &keyword);
            
            vector<string> Keywords() const { return keywords; };
            
        private:
        
            vector<string> keywords;
    };
    
    class MediaEnabled
    {
        public:
        
            MediaEnabled();
            
            vector<Multimedia> Media() const { return media; };
            
            void AddMedia(Multimedia multimedia);
            
            void RemoveMedia(const Multimedia &multimedia);
            
        private:
        
            vector<Multimedia> media;
    };
    
    class Genred
    {
        public:
        
            Genred();
            
            vector<Genre> Genres() const { return genres; };
            
            void AddGenre(Genre genre);
            
            void RemoveGenre(const Genre &genre);
            
        private:
        
            vector<Genre> genres;        
    };  
    
}

#endif
