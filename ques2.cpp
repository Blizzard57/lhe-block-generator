#include "LHEF.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

bool check_init_block(LHEF::HEPRUP* hep1,LHEF::HEPRUP* hep2){
    /* Checking if all the values in the <init> block are 
    the same. This is because the LHE doc requires this information 
    to be compulsorily present and should be unique. */

    if (hep1->IDBMUP.first != hep2->IDBMUP.first ||
        hep1->IDBMUP.second != hep2->IDBMUP.second ||
        hep1->EBMUP.first != hep2->EBMUP.first ||
        hep1->EBMUP.second != hep2->EBMUP.second ||
        hep1->PDFGUP.first != hep2->PDFGUP.first ||
        hep1->PDFGUP.second != hep2->PDFGUP.second ||
        hep1->PDFSUP.first != hep2->PDFSUP.first ||
        hep1->PDFSUP.second != hep2->PDFSUP.second ||
        hep1->IDWTUP != hep2->IDWTUP ||
        hep1->NPRUP != hep2->NPRUP){
        return false;
    }

    for ( int i = 0; i < hep1->NPRUP; ++i ){
        if(hep1->XSECUP[i] != hep2->XSECUP[i] ||
           hep1->XERRUP[i] != hep2->XERRUP[i] ||
           hep1->XMAXUP[i] != hep2->XMAXUP[i] ||
           hep1->LPRUP[i] != hep2->LPRUP[i]){
            return false;
           }
    }
    return true;
}

void write_headers(LHEF::Reader* reader1,LHEF::Reader* reader2,LHEF::Writer* writer, int priority){
    /* Header blocks are to be written once. In this code, if the non compulsory tags 
    have information that is different, it will write the data from the priority file, 
    which be default is the first one. For the compulsory tags, if there is a mismatch, 
    it will throw out an error. */

    if (reader1->headerBlock.compare(reader2->headerBlock) == 0)
        writer->headerBlock() << reader1->headerBlock;
    else{
        if(priority == 1)
            writer->headerBlock() << reader1->headerBlock;
        else
            writer->headerBlock() << reader2->headerBlock;
    }


    if (reader1->initComments.compare(reader2->initComments) == 0)
        writer->initComments() << reader1->initComments;
    
    else{
        if(priority == 1)
            writer->headerBlock() << reader1->initComments;
        else
            writer->headerBlock() << reader2->initComments;
    }

    if(check_init_block(&(reader1->heprup),&(reader2->heprup))){
        writer->heprup = reader1->heprup;
        writer->init();
    }
    else
        throw invalid_argument("ERROR : The init blocks for the two files do not match. That should not happen.");
}

void write_events(LHEF::Reader* reader, LHEF::Writer* writer, int events_per_block,int loc){
    /* Writing events from reader to writer. The amount of files written are loc*events_per_loc 
    and the start entry is (loc-1)*events_per_loc. */
    writer->heprup = reader->heprup;
    
    int neve = 0;                   // The event counter
    bool flag = false;              // Write only if flag is true
    while (reader->readEvent()){

        if (neve % events_per_block == 0){
            if (neve == events_per_block*(loc-1)){
                flag = true;
            }
            else if(neve == loc*events_per_block){
                break;
            }
        }
        if(flag){
            writer->eventComments() << reader->eventComments;
            writer->hepeup = reader->hepeup;
            writer->hepeup.heprup = &(*writer).heprup;
            writer->writeEvent();
        }
        ++neve;
    }
}

int main(int argc, char **argv){
    if(argc < 4){
        throw invalid_argument("ERROR : All the arguments are not present. The correct format \
                                for running is ./prog.out lhe_file1 lhe_file2 saved_lhe_file.");
    }
    
    // loc_i is the nth block 500 length block to be used for file i
    int loc1 = 1, loc2 = 2;
    int prioirty = 1;

    // events_per_block is the amount of events to be taken per block
    int events_per_block1 = 500, events_per_block2 = 500;

    LHEF::Reader* reader1 = new LHEF::Reader(argv[1]);
    LHEF::Reader* reader2 = new LHEF::Reader(argv[2]);
    LHEF::Writer* writer = new LHEF::Writer(argv[3]);

    // Getting the Header Information
    write_headers(reader1,reader2,writer,prioirty);
    
    // Writing events from file1
    write_events(reader1,writer,events_per_block1,loc1);
    
    // Writing events from file2
    write_events(reader2,writer,events_per_block2,loc2);

    return 0;
}