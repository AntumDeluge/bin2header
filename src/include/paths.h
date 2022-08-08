
/* Copyright © 2017-2022 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
 *
 * This file is part of the bin2header project & is distributed under the
 * terms of the MIT/X11 license. See: LICENSE.txt
 */

#ifndef B2H_PATHS_H_
#define B2H_PATHS_H_

#include <string>


/** Normalizes the path node separators for the current system.
 *
 *  @tparam string path
 *      Path to be parsed.
 *  @return
 *      Path formatted with native directory/node delimeters.
 */
extern std::string normalizePath(std::string path);

/** Concatenates two paths into one.
 *
 *  @tparam string a
 *     Leading path.
 *  @tparam string b
 *     Trailing path.
 *  @return
 *     Concatenated path.
 */
extern std::string joinPath(std::string a, std::string b);

/** Removes path to parent directory from path name.
 *
 *  @tparam string f
 *      Path to be parsed.
 *  @return
 *      Name of last node in path.
 */
extern std::string getBaseName(std::string f);

/** Removes last node from path name.
 *
 *  @tparam string f
 *      Path to be parsed.
 *  @return
 *      Path to parent directory.
 */
extern std::string getDirName(std::string f);


#endif /* B2H_PATHS_H_ */
