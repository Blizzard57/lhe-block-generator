#include "LHEF.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

void write_headers(LHEF::Reader* reader,LHEF::Writer* writer){
    /* Header blocks are to be written once per file. */

    writer->headerBlock() << reader->headerBlock;
    writer->initComments() << reader->initComments;
    writer->heprup = reader->heprup;
    writer->init();

}

void write_events(LHEF::Reader* reader, LHEF::Writer* writer, int events_per_block, char* filename){
    /* Writing events from reader to writer. Generates new file for every events_per_block events. */
    writer->heprup = reader->heprup;
    
    int neve = 0;                   // The event counter
    bool flag = false;              // Write headers only if flag is true

    while (reader->readEvent()){

        if (neve % events_per_block == 0){
                writer = new LHEF::Writer(filename + to_string(neve/events_per_block) + ".lhe");
                write_headers(reader,writer);
                cout << "Number of files processed : " << neve/events_per_block << endl;
        }
        writer->eventComments() << reader->eventComments;
        writer->hepeup = reader->hepeup;
        writer->hepeup.heprup = &(*writer).heprup;
        writer->writeEvent();
        
        ++neve;
    }
    cout << "Number of files processed : " << neve/events_per_block << endl;
}

int main(int argc, char **argv){
    if(argc < 3){
        throw invalid_argument("ERROR : All the arguments are not present. The correct \
                                format for running is ./prog.out lhe_file saved_lhe_file.");
    }

    // events_per_block is the amount of events to be taken per block
    int events_per_block = 500;

    LHEF::Reader* reader = new LHEF::Reader(argv[1]);
    LHEF::Writer* writer = new LHEF::Writer(argv[2] + to_string(0) + ".lhe");
    
    // Writing events from reader
    write_events(reader,writer,events_per_block,argv[2]);

    return 0;
}