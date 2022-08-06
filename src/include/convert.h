
/* Copyright © 2017-2022 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
 *
 * This file is part of the bin2header project & is distributed under the
 * terms of the MIT/X11 license. See: LICENSE.txt
 */

#ifndef B2H_CONVERT_H_
#define B2H_CONVERT_H_

#include <csignal>
#include <string>


#define len(a) (sizeof(a)/sizeof(*a))

extern void setChunkSize(const unsigned int sz);
extern void setNumberDataPerLine(const unsigned int nd);
extern void setShowDataContent(const bool dc);
extern void setOutputBitLength(const unsigned int bl);
extern void setReadOffset(const unsigned long ofs);
extern void setReadLength(const unsigned long lgt);
extern void setSwapEndianess(void);
extern void setEol(const std::string newEol);

extern void sigintHandler(int sig_num);

extern int convert(const std::string fin, const std::string fout, const std::string hname, const bool store_vector=false);


#endif /* B2H_CONVERT_H_ */
