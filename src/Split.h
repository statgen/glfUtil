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

#ifndef __SPLIT_H__
#define __SPLIT_H__

#include "GlfExecutable.h"
#include "GlfFile.h"

class Split : public GlfExecutable
{
public:
    Split();
    static void splitDescription();
    void description();
    void usage();
    int execute(int argc, char **argv);

private:
    void writeRecord(GlfRecord& record, 
                     bool newRef);

    String myOutBase;
    GlfFile myOutFile;
    GlfHeader myHeader;
    GlfRefSection myRefSection;
    uint32_t myOutEndPos;

    uint32_t myChunkSize;
    uint32_t myRecPos;
};

#endif
