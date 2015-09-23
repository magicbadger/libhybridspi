#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <iomanip>
#include <ctime>
#include <time.h>

#include <mot.h>
#include <datagroups.h>
#include <packets.h>

#include "rapidjson/document.h"
#include "../src/hybridspi_mot.h"

using namespace std;
using namespace mot;
using namespace msc;
using namespace spi;
using namespace rapidjson;

vector<unsigned char> read(string path)
{    
    ifstream f;
    f.open(path);
    vector<unsigned char> buffer;
    while(!f.eof())
    {
        string s;
        f >> s; 
        buffer.insert(buffer.end(), s.begin(), s.end());
    }
    f.close();
    return buffer;
}

int main(int argc, char* argv[])
{
    vector<unsigned char> buffer = read("manifest.json");
  
    Document document;
    string tmp(buffer.begin(), buffer.end());
    document.Parse(tmp.c_str());

    // set up a random transport ID generator
    RandomTransportIdGenerator* id = RandomTransportIdGenerator::getInstance();

    // read through each of the objects
    vector<MotObject> objects;
    assert(document.HasMember("entries"));
    const Value& entries = document["entries"];
    for(SizeType i = 0; i < entries.Size(); i++)
    {
        assert(entries[i].IsObject());
        const Value& entry = entries[i];
        assert(entry.HasMember("path"));
        vector<unsigned char> bytes = read(entry["path"].GetString());
        assert(entry.HasMember("contentname"));
        MotObject o(id->next(), entry["contentname"].GetString(), bytes, ContentTypes::ServiceInformation);
        if(entry.HasMember("scopeid")) 
        {
            string scopeid = entry["scopeid"].GetString();
            int ecc = stoul(scopeid.substr(0, 2), nullptr, 16);
            int eid = stoul(scopeid.substr(3, 6), nullptr, 16);
            o.addParameter(new ScopeId(ecc, eid));
        }
        if(entry.HasMember("scopestart")) 
        {
            tm t = {};
            string start = entry["scopestart"].GetString();
            istringstream ss(start);
            ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
            time_t epoch = mktime(&t);
            o.addParameter(new ScopeStart(epoch));
        }
        if(entry.HasMember("scopeend")) 
        {
            tm t = {};
            string end = entry["scopeend"].GetString();
            istringstream ss(end);
            ss >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
            time_t epoch = mktime(&t);
            o.addParameter(new ScopeEnd(epoch));
        }

        objects.push_back(o);
    }

    //cout << "transport id: " << o.getTransportId() << endl;
    SegmentEncoder segment_encoder;
    vector<DirectoryParameter*> parameters;
    //vector<Segment*> segments = segment_encoder.encode(id->next(), objects, parameters);
    vector<Segment*> segments = segment_encoder.encode(id->next(), objects);
    //cout << "encoded into " << segments.size() << " segments" << endl;
    DatagroupEncoder datagroup_encoder;
    vector<Datagroup*> datagroups = datagroup_encoder.encode_datagroups(segments);
    //cout << "encoded into " << datagroups.size() << " datagroups" << endl;
    PacketEncoder packet_encoder(1, 96);
    vector<Packet*> packets = packet_encoder.encode_packets(datagroups);
    //cout << "encoded into " << packets.size() << " packets" << endl;
    for(Packet* packet : packets)
    {
        //cout << "encoding packet: " << packet <<  endl;
        cout << packet->encode() << flush;
    }

}
