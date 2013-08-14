/*
 *  Copyright (C) 2013  Regents of the University of Michigan
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//////////////////////////////////////////////////////////////////////////
// This file contains the processing for the executable option "split"
// which splits glf files into the specified regions.

#include "Split.h"
#include "GlfFile.h"
#include "Parameters.h"
#include "BgzfFileType.h"

Split::Split()
    : GlfExecutable(),
      myOutBase(""),
      myOutFile(),
      myHeader(),
      myRefSection(),
      myOutEndPos(0),
      myChunkSize(0),
      myRecPos(0),
      myEmptyGlfs(false)
{
    
}

void Split::splitDescription()
{
    std::cerr << " split - Split a GLF file into files by section." << std::endl;
}

void Split::description()
{
    splitDescription();
}

void Split::usage()
{
    GlfExecutable::usage();
    std::cerr << "\t./glfUtil split --in <inputFilename> [--params]\n";
    std::cerr << "\tRequired Parameters:" << std::endl;
    std::cerr << "\t\t--in        : the GLF file to be read" << std::endl;
    std::cerr << "\tOptional Parameters For Other Operations:\n";
    std::cerr << "\t\t--outBase   : the base GLF filename to write" << std::endl;
    std::cerr << "\t\t--chunkSize : the region covered by each GLF file" << std::endl;
    std::cerr << "\t\t--params    : print the parameter settings" << std::endl;
    std::cerr << std::endl;
}


int Split::execute(int argc, char **argv)
{
    // Extract command line arguments.
    String inFile = "";
    bool params = false;
    myOutBase = "";
    myOutEndPos = 0;
    myChunkSize = 5000000;
    myRecPos = 0;

    ParameterList inputParameters;
    BEGIN_LONG_PARAMETERS(longParameterList)
        LONG_PARAMETER_GROUP("Required Parameters")
        LONG_STRINGPARAMETER("in", &inFile)
        LONG_PARAMETER_GROUP("Optional Other Parameters")
        LONG_STRINGPARAMETER("outBase", &myOutBase)
        LONG_INTPARAMETER("chunkSize", &myChunkSize)
        LONG_PARAMETER("emptyGlfs", &myEmptyGlfs)
        LONG_PARAMETER("params", &params)
        END_LONG_PARAMETERS();
   
    inputParameters.Add(new LongParameters ("Input Parameters", 
                                            longParameterList));

    inputParameters.Read(argc-1, &(argv[1]));

    // Check to see if the in file was specified, if not, report an error.
    if(inFile == "")
    {
        usage();
        // mandatory argument was not specified.
        inputParameters.Status();
        std::cerr << "Missing mandatory argument: --in" << std::endl;
        return(-1);
    }

    // if outBase wasn't specified, base it on in.
    if(myOutBase.IsEmpty())
    {
        myOutBase = inFile.Left(inFile.FindLastChar('.'));
    }

    if(params)
    {
        inputParameters.Status();
    }

    GlfFile glfIn;
    GlfFile glfOut;

    // Open the files for reading.
    glfIn.openForRead(inFile);

    // Read the glf header.
    glfIn.readHeader(myHeader);

    int numSections = 0;

    // Set returnStatus to success.  It will be changed
    // to the failure reason if any of the writes fail.
    GlfStatus::Status returnStatus = GlfStatus::SUCCESS;
    
    while(glfIn.getNextRefSection(myRefSection))
    {
        ++numSections;
        std::string refName;
        myRefSection.getName(refName);
        std::cout << "\tRefName = " << refName 
                  << "; RefLen = " << myRefSection.getRefLen() << "\n";
        int64_t numSectionRecords = 0;
        GlfRecord record;
        bool newRef = true;

        while(glfIn.getNextRecord(record))
        {
            writeRecord(record, newRef);
            ++numSectionRecords;
            newRef = false;
        }
    }
     return(returnStatus);
}


void Split::writeRecord(GlfRecord& record, 
                        bool newRef)
{
    static String newName;

    // Get the position for this record.
    if(newRef)
    {
        // New ref sequence, so the position is just the offset from 0.
        myRecPos = record.getOffset();
    }
    else
    {
        myRecPos = myRecPos + record.getOffset();
    }

    // Check if this should be a new file.
    if((myRecPos > myOutEndPos) || (newRef))
    {
        // New file.
        uint32_t startPos = (uint32_t)(myRecPos/myChunkSize);
        startPos *= myChunkSize;
        uint32_t prevEndPos = myOutEndPos;
        myOutEndPos = startPos + myChunkSize;
        ++startPos; // increase start position by 1
        std::string refName;
        myRefSection.getName(refName);

        if(myEmptyGlfs)
        {
            // Write empty glfs from the previous position to this one.
            if(newRef)
            {
                prevEndPos = 0;
            }
            uint32_t prevStartPos = prevEndPos + 1;
            prevEndPos += myChunkSize;
            while(myOutEndPos != prevEndPos)
            {
                // until we get to the current chunk, write empty GLFs.
                newName = myOutBase + '.' + refName.c_str() + '.' 
                    + prevStartPos + '.' + prevEndPos + ".glf";
                prevEndPos += myChunkSize;
                prevStartPos += myChunkSize;
                myOutFile.openForWrite(newName);
                myOutFile.writeHeader(myHeader);
                //                myOutFile.writeRefSection(myRefSection);
            }
        }

        newName = myOutBase + '.' + refName.c_str() + '.' + startPos + '.' 
            + myOutEndPos + ".glf";
        myOutFile.openForWrite(newName);
        myOutFile.writeHeader(myHeader);
        myOutFile.writeRefSection(myRefSection);

        // New output file, so set the offset as if from 0.
        record.setOffset(myRecPos);
    }

    // Write the record.
    myOutFile.writeRecord(record);
}
