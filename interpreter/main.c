#include "kutval.h"
#include "kutstring.h"
#include "kuttable.h"
#include "kutfunc.h"
#include "kutreference.h"

#include <stdio.h>
#include <math.h>
#include <inttypes.h>


int main() {    
    KutValue literals[] = {
        kutnumber_wrap(25),
        kutnumber_wrap(16),
        kutstring_wrap(kutstring_literal("atan2")),
        kutnumber_wrap(3),
        kutnumber_wrap(2),
        kutstring_wrap(kutstring_literal("tan")),
    };

    const KutInstruction inner_instructions[] = {
        kutfunc_returncall(0),
        kutfunc_noperation(),
    };
    const KutFuncTemplate* inner_template = kutfunc_templateLiteral(inner_instructions, literals, 2, 2);
    
    const KutFuncTemplate* templates[] = {
        inner_template,
    };

    const KutInstruction instructions[] = {
        kutfunc_getliteral(1, 2),
        kutfunc_gettmplate(0, 0),
        kutfunc_returncall(1),
        kutfunc_noperation(),
    };
    KutFuncTemplate template = {
        .capture_count = 0,
        .instructions = instructions,
        .literals = literals,
        .register_count = 7,
        .function_templates = templates,
    };

    KutValue func = kutfunc_wrap(kutfunc_new(NULL, &template));
    KutValue ret = kutfunc_run(&func, empty_table);
    KutString* str = kut_tostring(&func);
    // printf("%.*s\n", kutstring_format(str));
    kut_decref(&func);
    kut_decref(&ret);
    free(str);
    return 0;
}
