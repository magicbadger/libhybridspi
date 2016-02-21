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
            
            vector<Name> GetNames() const;
            
            vector<Name> GetNames(int maxlength) const;
            
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
            
            double GetLatitude() { return latitude; }
            
            double GetLongitude() { return longitude; }
            
        protected:
        
            bool equals(const GeoLocation& a) const;
            
        private:
        
            double latitude, longitude;
               
    };
    
    class CountryCode : public GeoLocation
    {
        
        public:
        
            CountryCode(string code);
            
            string GetCode() { return code; };
        
        protected:
        
            bool equals(const GeoLocation& a) const;
            
        private:
        
            string code;
        
    };
    
    class Polygon : public GeoLocation
    {
        
        public:
        
            Polygon(vector<Point> points);
            
            vector<Point>::iterator GetPoints() { return points.begin(); };
        
        protected:
        
            bool equals(const GeoLocation& a) const;
        
        private:
        
            vector<Point> points;
        
    };
    
    class GeoLocated
    {
        public:
            
            vector<GeoLocation*>::iterator GetLocations() { return geolocations.begin(); };
            
            void AddLocation(GeoLocation* geolocation);
            
            void RemoveLocation(GeoLocation* geolocation);
            
        private:
        
            vector<GeoLocation*> geolocations;
            
    };
    
    class Bearer : public GeoLocated
    {
        public:
        
            Bearer(int cost, int offset);
            
            int GetCost() { return cost; }
            
            int GetOffset() { return offset; }
            
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
        
            DigitalBearer(int bitrate, string content, int cost, int offset);
            
            virtual string URI() const = 0;  
            
        private:
            
            int bitrate;
            
            string content;
    };
    
    class DabBearer : public DigitalBearer
    {
        public:
        
            DabBearer(int ecc, int eid, int sid, int scids, int bitrate, string content, int cost, int offset);
            
            int GetECC() { return ecc; };
            
            int GetEId() { return eid; };
            
            int GetSId() { return sid; };
            
            int GetSCIdS() { return scids; };
            
            string URI() const; 
            
        private:
        
            bool equals(const Bearer& a) const;
        
            int ecc;
            
            int eid;
            
            int sid;
            
            int scids;
    };
    
    class FmBearer : public Bearer
    {
        public:
        
            FmBearer(int ecc, int pi, int frequency, int cost, int offset);
            
            int GetECC() { return ecc; };
            
            int GetPI() { return pi; };
            
            int GetFrequency() { return frequency; };
            
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
        
            IpBearer(string uri, int bitrate, string content, int cost, int offset);
            
            string URI() const { return uri; };
            
        private:
        
            bool equals(const Bearer& a) const;
        
            string uri;
            
    };
    
    class Link
    {
        public:
        
            Link(string url, string content, string description);
            
            string GetUrl() { return url; };
            
            string GetContent() { return content; };
            
            string GetDescription() { return description; };
            
            bool operator== (const Link &that) const;
            
            bool operator!= (const Link &that) const;                       
            
        private:
        
            string url;
            
            string content;
            
            string description;
    };
    
    class AbsoluteTime
    {
        public:
        
            AbsoluteTime(DateTime billedTime, Duration billedDuration, DateTime actualTime, Duration actualDuration);
            
            DateTime GetBilledTime() { return billedTime; };
            
            DateTime GetActualTime() { return actualTime; };
            
            Duration GetBilledDuration() { return billedDuration; };
            
            Duration GetActualDuration() { return actualDuration; };
            
        private:
        
            DateTime billedTime, actualTime;
            
            Duration billedDuration, actualDuration;
            
    };
    
    class Membership
    {
        public:
        
            Membership(short shortcrid, string crid, short index);
            
            short GetShortCrid() { return shortcrid; };
            
            string GetCrid() { return crid; };
            
            short GetIndex() { return index; };
        
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
            
            string ContentType() { return content; };
            
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
            
            string GetHref() { return href; };
            
            string GetName() { return name; };
            
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
            
            vector<Description> GetDescriptions() const;
            
            vector<Description> GetDescriptions(int max_length) const;
            
            void AddDescription(Description description);
            
            void RemoveDescription(const Description &description);
            
        private:
        
            vector<Description> descriptions;
    };
    
    class Linked
    {
        public:
        
            Linked();
            
            vector<Link> GetLinks() const;
            
            void AddLink(Link link);
            
            void RemoveLink(const Link &link);
            
        private:
        
            vector<Link> links;        
    };
    
    class Keyworded
    {
        public:
        
            void AddKeyword(string keyword);
            
            void RemoveKeyword(const string &keyword);
            
            vector<string> Keywords() const { return keywords; };
            
        private:
        
            vector<string> keywords;
    };
    
    class MultimediaEnabled
    {
        public:
        
            MultimediaEnabled();
            
            vector<Multimedia> GetMultimedia() const;
            
            void AddMultimedia(Multimedia multimedia);
            
            void RemoveMultimedia(const Multimedia &multimedia);
            
        private:
        
            vector<Multimedia> multimedias;
    };
    
    class Genred
    {
        public:
        
            Genred();
            
            vector<Genre> GetGenres() const;
            
            void AddGenre(Genre genre);
            
            void RemoveGenre(const Genre &genre);
            
        private:
        
            vector<Genre> genres;        
    };  
    
}

#endif
