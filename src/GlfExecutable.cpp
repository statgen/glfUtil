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
// This file contains the processing for a glf executable.

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "GlfExecutable.h"

GlfExecutable::GlfExecutable()
{
}


GlfExecutable::~GlfExecutable()
{
}


void GlfExecutable::glfVersion()
{
    std::cerr << "Version: " << VERSION
              << "; Built: " << DATE << " by "<< USER << std::endl;
}

void GlfExecutable::glfExecutableDescription()
{
    std::cerr << "Set of tools for operating on GLF files." << std::endl;
    glfVersion();
}


void GlfExecutable::description()
{
    glfExecutableDescription();
}


void GlfExecutable::usage()
{
    glfVersion();
    std::cerr << std::endl;
    description();
}
