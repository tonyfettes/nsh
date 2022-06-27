#ifndef NSH_PARSE_PIPELINE_H
#define NSH_PARSE_PIPELINE_H

#include "nsh/parse/posix/common.h"
#include "nsh/pipeline.h"
#include "nsh/keyword.h"

// Parse pipeline, and consume following `|` or `||`.
bool parse_pipeline(struct parse *parse, struct pipeline *pipeline);

#endif // NSH_PARSE_PIPELINE_H
