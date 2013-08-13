/*
 *  Copyright (C) 2010  Regents of the University of Michigan
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
// This file contains the processing for the executable option "dump"
// which writes a file with the reads in the specified region.

#include "Dump.h"
#include "GlfFile.h"
#include "Parameters.h"
#include "BgzfFileType.h"

Dump::Dump()
    : GlfExecutable()
{
    
}

void Dump::dumpDescription()
{
    std::cerr << " dump - Write a file with reads in the specified region and/or have the specified read name" << std::endl;
}

void Dump::description()
{
    dumpDescription();
}

void Dump::usage()
{
    GlfExecutable::usage();
    std::cerr << "\t./glfUtil dump --in <inputFilename> [--params]\n";
    std::cerr << "\tRequired Parameters:" << std::endl;
    std::cerr << "\t\t--in        : the GLF file to be read" << std::endl;
    std::cerr << "\tOptional Parameters For Other Operations:\n";
    std::cerr << "\t\t--params    : print the parameter settings" << std::endl;
    std::cerr << std::endl;
}


int Dump::execute(int argc, char **argv)
{
    // Extract command line arguments.
    String inFile = "";
    bool params = false;

    ParameterList inputParameters;
    BEGIN_LONG_PARAMETERS(longParameterList)
        LONG_PARAMETER_GROUP("Required Parameters")
        LONG_STRINGPARAMETER("in", &inFile)
        LONG_PARAMETER_GROUP("Optional Other Parameters")
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
    if(params)
    {
        inputParameters.Status();
    }

    GlfFile glfIn;
    GlfHeader glfHeader;

    // Open the file for reading.   
    glfIn.openForRead(inFile);

    // Read the glf header.
    glfIn.readHeader(glfHeader);

    // Output the glf header.
    std::string headerText = "";
    glfHeader.getHeaderTextString(headerText);
    std::cout << "GlfHeader:\n";
    std::cout << headerText << std::endl;

    int numSections = 0;

    // Set returnStatus to success.  It will be changed
    // to the failure reason if any of the writes fail.
    GlfStatus::Status returnStatus = GlfStatus::SUCCESS;
    
    GlfRefSection refSection;
    while(glfIn.getNextRefSection(refSection))
    {
        ++numSections;
        std::string refName;
        refSection.getName(refName);
        std::cout << "\tRefName = " << refName 
                  << "; RefLen = " << refSection.getRefLen() << "\n";
        int64_t numSectionRecords = 0;
        GlfRecord record;
        while(glfIn.getNextRecord(record))
        {
            record.print();
            ++numSectionRecords;
        }
    }
//         // Keep reading records until they aren't anymore.
//         while(glfIn.ReadRecord(glfHeader, glfRecord))
//         {
//             if(!readName.IsEmpty())
//             {
//                 // Check for readname.
//                 if(strcmp(glfRecord.getReadName(), readName.c_str()) != 0)
//                 {
//                     // not a matching read name, so continue to the next record.
//                     continue;
//                 }
//             }
            
//             // Check to see if the read has already been processed.
//             if(myPrevEnd != UNSPECIFIED_INT)
//             {
//                 // Because we already know that the bed was sorted, 
//                 // we know that the previous section started before
//                 // this one, so if the previous end is greater than
//                 // this record's end position we know that it
//                 // was already written in the previous section.
//                 // Note: can't be equal to the previous end since
//                 // the end range was exclusive, while
//                 // get0BasedAlignmentEnd is inclusive.
//                 // myPrevEnd is reset by getNextSection when a new
//                 // chromosome is hit.
//                 if(glfRecord.get0BasedAlignmentEnd() < myPrevEnd)
//                 {
//                     // This record was already written.
//                     continue;
//                 }
//             }

//             // Shift left if applicable.
//             if(lshift)
//             {
//                 glfRecord.shiftIndelsLeft();
//             }

//             // Successfully read a record from the file, so write it.
//             glfOut.WriteRecord(glfHeader, glfRecord);
//             ++numSectionRecords;
//         }
//         myWroteReg = true;
//     }

//     if(myBedFile != NULL)
//     {
//         ifclose(myBedFile);
//     }
//     std::cerr << "Wrote " << outFile << " with " << numSectionRecords
//               << " records.\n";
     return(returnStatus);
}
