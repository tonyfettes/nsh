#ifndef NSH_PARSE_SEGMENT_H
#define NSH_PARSE_SEGMENT_H

#include "nsh/segment.h"
#include "nsh/parse/common.h"

bool is_expansion(char ch);

bool parse_segment(struct parse *parse, struct segment *segment);

#endif // NSH_PARSE_SEGMENT_H
