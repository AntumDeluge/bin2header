
/* Copyright © 2017-2022 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
 *
 * This file is part of the bin2header project & is distributed under the
 * terms of the MIT/X11 license. See: LICENSE.txt
 */

// misc utility functions

#ifndef B2H_UTIL_H_
#define B2H_UTIL_H_

#include <string>


/** Checks for an empty string.
 *
 *  Checks string length & if contains only whitespace characters.
 *
 *  @param st
 *      String to be checked.
 *  @return
 *      `true` if empty or only whitepace characters found.
 */
extern bool checkEmptyString(const std::string st);

/** Retrieves current timestamp in milliseconds. */
extern long long currentTimeMillis();

/** Formats duration for printing.
 *
 *  @param ts
 *      Process start timestamp (milliseconds).
 *  @param te
 *      Process end timestamp (milliseconds).
 *  @return
 *      String formatted for readability.
 */
extern std::string formatDuration(const long long ts, const long long te);


#endif /* B2H_UTIL_H_ */
