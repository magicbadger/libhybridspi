#ifndef HYBRIDSPI_COMMON_H
#define HYBRIDSPI_COMMON_H

#include <chrono>
#include <string>
#include <vector>
#include <regex>
#include <bitset>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;

using Clock = std::chrono::system_clock;

using DateTime = time_point<Clock>;
using Duration = seconds;


using bytes = vector<unsigned char>;

const unsigned int DEFAULT_COST = 0;
const unsigned int DEFAULT_OFFSET = 0;

namespace hybridspi
{

    template<size_t size>
    vector<unsigned char> bits_to_bytes(bitset<size> bits)
    {
        vector<unsigned char> bytes(size/8);
        for(int j=0; j<int(size); j++) {
            bytes[j/8] = bytes[j/8] + ((bits[size-j-1] ? 1:0) << (7-j%8));
        }

        return bytes;
    }

    vector<unsigned char> operator+(const vector<unsigned char> &a, const vector<unsigned char> &b);

    DateTime make_datetime(string text, string format);

    DateTime make_datetime(int year, int month, int day, int hour,
                           int min, int sec, int usec, int isDST = -1);
    
    class BaseText
    {
        public:
        
            BaseText(string text, int max_length);
            
            string Text() const { return text; }
            
            int MaxLength() const { return max_length; }
            
            bool operator== (const BaseText &that) const;
            
            bool operator!= (const BaseText &that) const;            

            friend ostream& operator<< (ostream& stream, const BaseText& text);

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
            
            void AddName(const Name &name);
            
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

            friend ostream& operator<< (ostream& stream, const Point& point);
            
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
        
            friend ostream& operator<< (ostream& stream, const CountryCode& country);

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
        
            friend ostream& operator<< (ostream& stream, const Polygon& polygon);

        protected:
        
            bool equals(const GeoLocation& a) const;
        
        private:
        
            vector<Point> points;
        
    };
    
    class GeoLocated
    {
        public:
            
            vector<GeoLocation*>::iterator GeoLocations() { return geolocations.begin(); };
            
            void AddGeoLocation(GeoLocation* geolocation);
            
            void RemoveGeoLocation(GeoLocation* geolocation);
            
        private:
        
            vector<GeoLocation*> geolocations;
            
    };
    
    class Bearer : public GeoLocated
    {
        public:
        
            Bearer(int cost, int offset);
            
            void SetCost(int cost) { this->cost = cost; };
            
            int Cost() { return cost; }
            
            void SetOffset(int offset) { this->offset = offset; };
            
            int Offset() { return offset; }
            
            bool operator== (const Bearer &that) const;
            
            bool operator!= (const Bearer &that) const;  
            
            virtual string URI() const = 0;  

            friend ostream& operator<< (ostream& stream, const Bearer& bearer);
            
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
            
            void SetBitrate(int bitrate) { this->bitrate = bitrate; };
            
            int Bitrate() { return bitrate; };
            
            void SetContent(string content) { this->content = content; };
            
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
            
            // parse from the bearer URI
            DabBearer(string uri, string content, int cost = DEFAULT_COST, int offset = DEFAULT_OFFSET);
            
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
            
            const regex BEARER_REGEX = regex("^dab:(.{3})\\.(.{4})\\.(.{4})\\.(.{1})[\\.(.+?)]{0,1}$");
    };
    
    class FmBearer : public Bearer
    {
        public:
        
            FmBearer(int ecc, int pi, int frequency, int cost = DEFAULT_COST, int offset = DEFAULT_OFFSET);
            
            // parse from the bearer URI
            FmBearer(string uri, int cost = DEFAULT_COST, int offset = DEFAULT_OFFSET);
            
            int ECC() { return ecc; };
            
            int PI() { return pi; };
            
            int Frequency() { return frequency; };
            
            string URI() const;             
            
        private: 
        
            bool equals(const Bearer& a) const;
        
            int ecc;
            
            int pi;
            
            int frequency;
            
            const regex BEARER_REGEX = regex("fm:(.{3})\\.(.{4})\\.(.{5})");
    };
    
    class IpBearer : public DigitalBearer
    {
        public:
        
            IpBearer(string uri, string content, int cost = DEFAULT_COST, int offset = DEFAULT_OFFSET);
            
            string URI() const { return uri; };
            
        private:
        
            bool equals(const Bearer& a) const;
        
            string uri;
            
    };
    
    class Link
    {
        public:
        
            Link(string uri, string content = "", string description = "");
            
            string URI() const { return uri; };
            
            Link* SetContent(string content)
            {
                this->content = content;
                return this;
            }
            
            string Content() const { return content; };
            
            Link* SetDescription(string description)
            {
                this->description = description;
                return this;
            }
            
            string Description() const { return description; };
            
            bool operator== (const Link &that) const;
            
            bool operator!= (const Link &that) const; 
            
            friend ostream& operator<< (ostream& stream, const Link& link);
            
        private:
        
            string uri;
            
            string content;
            
            string description;
    };
    
    class Membership
    {
        public:
        
            Membership(string id, unsigned short shortId, unsigned short index = 1);
            
            unsigned short ShortId() const { return shortId; };
            
            string Id() const { return id; };
            
            unsigned short Index() const { return index; };
            
            bool operator== (const Membership &that) const;
            
            bool operator!= (const Membership &that) const;             
        
            friend ostream& operator<< (ostream& stream, const Membership& membership);

        private:
        
            unsigned short shortId;
            
            string id;
            
            unsigned short index;
        
    };
    
    class Multimedia
    {
        public:
        
            Multimedia(string location, string content);    
        
            Multimedia(string location, string content, int width, int height);
            
            void SetLocation(string location) { this->location = location; };
            
            string Location() const { return location; };
            
            void SetContent(string content) { this->content = content; };
            
            string Content() const { return content; };
            
            void SetHeight(int height) { this->height = height; };
            
            int Height() const { return height; };
            
            void SetWidth(int width) { this->width = width; };
            
            int Width() const { return width; };
            
            bool operator== (const Multimedia &that) const;
            
            bool operator!= (const Multimedia &that) const;   
            
            friend ostream& operator<< (ostream& stream, const Multimedia& multimedia);                                       
            
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
            
            string Href() const { return href; };
            
            string Name() const { return name; };
            
            bool operator== (const Genre &that) const;
            
            bool operator!= (const Genre &that) const;    
            
            friend ostream& operator<< (ostream& stream, const Genre& genre);            
            
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
            
            void AddDescription(const Description &description);
            
            void RemoveDescription(const Description &description);
            
        private:
        
            vector<Description> descriptions;
    };
    
    class Linked
    {
        public:
        
            Linked();
            
            vector<Link> Links() const { return links; };
            
            void AddLink(const Link &link);
            
            void RemoveLink(const Link &link);
            
        private:
        
            vector<Link> links;        
    };
    
    class Keyworded
    {
        public:
        
            void AddKeyword(const string &keyword);
        
            void AddKeywords(const string &keywords);
            
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
            
            void AddMedia(const Multimedia &multimedia);
            
            void RemoveMedia(const Multimedia &multimedia);
            
        private:
        
            vector<Multimedia> media;
    };
    
    class Genred
    {
        public:
        
            Genred();
            
            vector<Genre> Genres() const { return genres; };
            
            void AddGenre(const Genre &genre);
            
            void RemoveGenre(const Genre &genre);
            
        private:
        
            vector<Genre> genres;        
    };  
    
    // class Interval
    // {
    //     public:
        
    //         Interval(DateTime start, DateTime end);
            
    //         DateTime Start() { return start; };
            
    //         DateTime End() { return end; };
            
    //     private:
        
    //         DateTime start, end;
    // }
    
    static std::ostream& operator<< (std::ostream& stream, const DateTime& timepoint)
    {
        std::time_t timepoint_t = std::chrono::system_clock::to_time_t(timepoint);
        stream << std::put_time(std::localtime(&timepoint_t), "%FT%T%Z");
        return stream;
    }

    static std::ostream& operator<< (std::ostream& stream, const Duration& duration)
    {
        stream << "PT" << duration.count() << "S"; // hardcore to seconds for now
        return stream;
    }

}

#endif
