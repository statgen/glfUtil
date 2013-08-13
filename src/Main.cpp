/*
 *  Copyright (C) 2010-2011  Regents of the University of Michigan
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

#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "Dump.h"

void Usage()
{
    GlfExecutable::glfExecutableDescription();
    std::cerr << std::endl;
    std::cerr << "\nPrint Information In Readable Format\n";
    Dump::dumpDescription();

    std::cerr << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << "\tglfUtil <tool> [<tool arguments>]" << std::endl;
    std::cerr << "The usage for each tool is described by specifying the tool with no arguments." << std::endl;
}


int main(int argc, char ** argv)
{
    GlfExecutable* glfExe = NULL;

    // Verify at least one arg.
    if(argc < 2)
    {
        // Not enough args...
        Usage();
        exit(-1);
    }

    if(strcmp(argv[1], "dump") == 0)
    {
        glfExe = new Dump();
    }
    else
    {
        std::cerr << "Unknown option: " << argv[1] << std::endl;
        Usage();
        exit(-1);
    }
    
    if(glfExe != NULL)
    {
        int returnVal = glfExe->execute(argc, argv);
        delete glfExe;
        glfExe = NULL;
        return(returnVal);
    }
    return(-1);
}



