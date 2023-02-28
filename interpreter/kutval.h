#ifndef __KUTVAL_H__
#define __KUTVAL_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <iso646.h>

typedef struct KutValue KutValue;
typedef struct KutTable KutTable;
typedef struct KutString KutString;

typedef union KutData {
    bool boolean;
    double number;
    void* data;
} KutData;

typedef KutValue (*KutDispatchedFn)(KutValue* self, KutTable* args);
typedef KutDispatchedFn (*KutDispatchFn)(KutValue* self, KutString* message);

KutValue empty_dispatched(KutValue* self, KutTable* args); // TODO: Remove me!

KutDispatchedFn kutundefined_dispatch(KutValue* self, KutString* message);
KutDispatchedFn kutboolean_dispatch(KutValue* self, KutString* message);
KutDispatchedFn kutnumber_dispatch(KutValue* self, KutString* message);
KutDispatchedFn kutreference_dispatch(KutValue* self, KutString* message);
KutDispatchedFn kutstring_dispatch(KutValue* self, KutString* message);
KutDispatchedFn kutfunc_dispatch(KutValue* self, KutString* message);
KutDispatchedFn kuttable_dispatch(KutValue* self, KutString* message);

extern KutTable* const empty_table;

struct KutValue {
    size_t reference_count;
    KutData data;
    KutDispatchFn dispatch;
};

struct KutDispatchGperfPair {
    const char* name;
    KutDispatchedFn method;
};

#define istype(v, T) ((v).dispatch == T##_dispatch)
bool checkarg(KutTable* args, size_t index, KutDispatchFn type);
#define checkarg(args, index, type) checkarg(args, index, type##_dispatch)

static const KutValue kut_nil = {.dispatch = NULL};
static const KutValue kut_undefined = {.dispatch = kutundefined_dispatch};

static inline KutValue kut_wrap(KutData data, KutDispatchFn dispatch) { return (KutValue){.reference_count = 0, .data = data, .dispatch = dispatch}; }

static inline KutValue kutboolean_wrap(bool boolean) { return kut_wrap((KutData){.boolean = boolean}, kutboolean_dispatch); }
static inline KutValue kutnumber_wrap(double number) { return kut_wrap((KutData){.number = number}, kutnumber_dispatch); }

static inline double kutnumber_cast(KutValue val) { return istype(val, kutnumber) ? val.data.number : 0; }
static inline bool kutboolean_cast(KutValue val) { return istype(val, kutboolean) ? val.data.boolean : 0; }

void kut_addref(KutValue* self);
void kut_decref(KutValue* self);
KutString* kut_tostring(KutValue* self);

#define eval(x) x


typedef union KutInstruction KutInstruction;

#endif