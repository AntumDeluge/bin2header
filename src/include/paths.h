
/* Copyright © 2017-2022 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
 *
 * This file is part of the bin2header project & is distributed under the
 * terms of the MIT/X11 license. See: LICENSE.txt
 */

#ifndef B2H_PATHS_H_
#define B2H_PATHS_H_

#include <string>


extern std::string normalizePath(std::string path);

extern std::string joinPath(std::string a, std::string b);

extern std::string getBaseName(std::string f);

extern std::string getDirName(std::string f);


#endif /* B2H_PATHS_H_ */
