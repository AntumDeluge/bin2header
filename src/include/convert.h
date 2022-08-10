
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

/** Sets the read buffer chunk size.
 *
 *  @tparam int sz
 *      Chunk size (in bytes).
 */
extern void setChunkSize(const unsigned int sz);

/** Sets number of bytes per line.
 *
 *  @tparam int nd
 *      Bytes per line.
 */
extern void setNumberDataPerLine(const unsigned int nd);

/** Enables showing data content as comments.
 *
 *  @tparam bool dc
 *      `true` shows content comments, `false` disables.
 */
extern void setShowDataContent(const bool dc);

/** Sets data type length.
 *
 *  Supported values are 8, 16, & 32.
 *
 *  @tparam int bl
 *      Bit length.
 */
extern void setOutputBitLength(const unsigned int bl);

/** Sets begin read offset position.
 *
 *  @tparam long ofs
 *      Position (in bytes).
 */
extern void setReadOffset(const unsigned long ofs);

/** Sets number to bytes to process.
 *
 *  @tparam long lgt
 *      Number of bytes to process.
 */
extern void setReadLength(const unsigned long lgt);

/** Changes output bytes to big endian.
 *
 *  16 & 32 bit data types only.
 */
extern void setSwapEndianess();

/** Sets end of line character.
 *
 *  @tparam string newEol
 *      Character to be used for EOL.
 */
extern void setEol(const std::string newEol);

/** Cancels current write iteration.
 *
 *  @tparam int sig_num
 */
extern void sigintHandler(const int sig_num);

/** Reads data from input & writes header.
 *
 *  @tparam string fin
 *      Path to file to be read.
 *  @tparam string fout
 *      Path to file to be written.
 *  @tparam string hname
 *      Text to be used for header definition & array variable name.
 *  @tparam stdvector
 *      Flag to additionally store data in C++ std::vector (default: `false`).
 */
extern int convert(const std::string fin, const std::string fout, std::string hname,
		const bool stdvector=false);


#endif /* B2H_CONVERT_H_ */
