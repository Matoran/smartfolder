/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Parse the programs args
 * @version 0.1
 * @date December 2016 and January 2017
 * @file parser.h
 *
 * Parse args, test if the directories are valid, set the linker destination
 * and parse infixe to postfix expression
 */

#ifndef SEARCHFOLDER_PARSER_H
#define SEARCHFOLDER_PARSER_H

#define NOT 0
#define OR 1
#define AND 2
#define BRACKET_OPEN 3
#define BRACKET_CLOSE 4
#define NAMES 3
#define SIZES 4
#define DATES 5
#define OWNERS 6
#define PERMS 7

extern void parser(int argc, char *argv[]);

#endif //SEARCHFOLDER_PARSER_H
