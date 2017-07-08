
#include "nuitka/prelude.h"

// Sentinel PyObject to be used for all our call iterator endings. It will
// become a PyCObject pointing to NULL. It's address is unique, and that's
// enough for us to use it as sentinel value.
PyObject *_sentinel_value = NULL;

PyObject *const_int_0;
PyObject *const_int_pos_1;
PyObject *const_str_empty;
PyObject *const_dict_empty;
PyObject *const_tuple_empty;
PyObject *const_str_plain_int;
PyObject *const_str_plain_len;
PyObject *const_str_plain_sum;
PyObject *const_str_plain_iter;
PyObject *const_str_plain_long;
PyObject *const_str_plain_open;
PyObject *const_str_plain_repr;
PyObject *const_str_plain_site;
PyObject *const_str_plain_type;
PyObject *const_str_plain_range;
PyObject *const_str_plain_format;
PyObject *const_str_plain_xrange;
PyObject *const_str_plain___all__;
PyObject *const_str_plain___cmp__;
PyObject *const_str_plain___doc__;
PyObject *const_str_plain_compile;
PyObject *const_str_plain_inspect;
PyObject *const_str_plain___dict__;
PyObject *const_str_plain___exit__;
PyObject *const_str_plain___file__;
PyObject *const_str_plain___iter__;
PyObject *const_str_plain___main__;
PyObject *const_str_plain___name__;
PyObject *const_str_plain___path__;
PyObject *const_str_plain_exc_type;
PyObject *const_str_plain___class__;
PyObject *const_str_plain___enter__;
PyObject *const_str_plain_exc_value;
PyObject *const_str_plain___import__;
PyObject *const_str_plain___module__;
PyObject *const_str_plain___delattr__;
PyObject *const_str_plain___getattr__;
PyObject *const_str_plain___setattr__;
PyObject *const_str_plain___builtins__;
PyObject *const_str_plain___metaclass__;
PyObject *const_str_plain_exc_traceback;
PyObject *const_str_digest_58961c73026e70b23b08cf381b49e04e;

static void _createGlobalConstants( void )
{
    NUITKA_MAY_BE_UNUSED PyObject *exception_type, *exception_value;
    NUITKA_MAY_BE_UNUSED PyTracebackObject *exception_tb;

#ifdef _MSC_VER
    // Prevent unused warnings in case of simple programs, the attribute
    // NUITKA_MAY_BE_UNUSED doesn't work for MSVC.
    (void *)exception_type; (void *)exception_value; (void *)exception_tb;
#endif

    const_int_0 = PyInt_FromLong( 0l );
    const_int_pos_1 = PyInt_FromLong( 1l );
    const_str_empty = UNSTREAM_STRING( &constant_bin[ 0 ], 0, 0 );
    const_dict_empty = _PyDict_NewPresized( 0 );
    assert( PyDict_Size( const_dict_empty ) == 0 );
    const_tuple_empty = PyTuple_New( 0 );
    const_str_plain_int = UNSTREAM_STRING( &constant_bin[ 597 ], 3, 1 );
    const_str_plain_len = UNSTREAM_STRING( &constant_bin[ 1382 ], 3, 1 );
    const_str_plain_sum = UNSTREAM_STRING( &constant_bin[ 1385 ], 3, 1 );
    const_str_plain_iter = UNSTREAM_STRING( &constant_bin[ 160 ], 4, 1 );
    const_str_plain_long = UNSTREAM_STRING( &constant_bin[ 1388 ], 4, 1 );
    const_str_plain_open = UNSTREAM_STRING( &constant_bin[ 1119 ], 4, 1 );
    const_str_plain_repr = UNSTREAM_STRING( &constant_bin[ 1392 ], 4, 1 );
    const_str_plain_site = UNSTREAM_STRING( &constant_bin[ 1396 ], 4, 1 );
    const_str_plain_type = UNSTREAM_STRING( &constant_bin[ 1230 ], 4, 1 );
    const_str_plain_range = UNSTREAM_STRING( &constant_bin[ 1400 ], 5, 1 );
    const_str_plain_format = UNSTREAM_STRING( &constant_bin[ 1405 ], 6, 1 );
    const_str_plain_xrange = UNSTREAM_STRING( &constant_bin[ 1411 ], 6, 1 );
    const_str_plain___all__ = UNSTREAM_STRING( &constant_bin[ 1417 ], 7, 1 );
    const_str_plain___cmp__ = UNSTREAM_STRING( &constant_bin[ 1424 ], 7, 1 );
    const_str_plain___doc__ = UNSTREAM_STRING( &constant_bin[ 1431 ], 7, 1 );
    const_str_plain_compile = UNSTREAM_STRING( &constant_bin[ 1438 ], 7, 1 );
    const_str_plain_inspect = UNSTREAM_STRING( &constant_bin[ 1445 ], 7, 1 );
    const_str_plain___dict__ = UNSTREAM_STRING( &constant_bin[ 1452 ], 8, 1 );
    const_str_plain___exit__ = UNSTREAM_STRING( &constant_bin[ 1460 ], 8, 1 );
    const_str_plain___file__ = UNSTREAM_STRING( &constant_bin[ 1468 ], 8, 1 );
    const_str_plain___iter__ = UNSTREAM_STRING( &constant_bin[ 1476 ], 8, 1 );
    const_str_plain___main__ = UNSTREAM_STRING( &constant_bin[ 1484 ], 8, 1 );
    const_str_plain___name__ = UNSTREAM_STRING( &constant_bin[ 1492 ], 8, 1 );
    const_str_plain___path__ = UNSTREAM_STRING( &constant_bin[ 1500 ], 8, 1 );
    const_str_plain_exc_type = UNSTREAM_STRING( &constant_bin[ 1508 ], 8, 1 );
    const_str_plain___class__ = UNSTREAM_STRING( &constant_bin[ 1516 ], 9, 1 );
    const_str_plain___enter__ = UNSTREAM_STRING( &constant_bin[ 1525 ], 9, 1 );
    const_str_plain_exc_value = UNSTREAM_STRING( &constant_bin[ 1534 ], 9, 1 );
    const_str_plain___import__ = UNSTREAM_STRING( &constant_bin[ 1543 ], 10, 1 );
    const_str_plain___module__ = UNSTREAM_STRING( &constant_bin[ 1553 ], 10, 1 );
    const_str_plain___delattr__ = UNSTREAM_STRING( &constant_bin[ 1563 ], 11, 1 );
    const_str_plain___getattr__ = UNSTREAM_STRING( &constant_bin[ 1574 ], 11, 1 );
    const_str_plain___setattr__ = UNSTREAM_STRING( &constant_bin[ 1585 ], 11, 1 );
    const_str_plain___builtins__ = UNSTREAM_STRING( &constant_bin[ 1596 ], 12, 1 );
    const_str_plain___metaclass__ = UNSTREAM_STRING( &constant_bin[ 1608 ], 13, 1 );
    const_str_plain_exc_traceback = UNSTREAM_STRING( &constant_bin[ 1621 ], 13, 1 );
    const_str_digest_58961c73026e70b23b08cf381b49e04e = UNSTREAM_STRING( &constant_bin[ 1634 ], 15, 0 );

#if _NUITKA_EXE
    /* Set the "sys.executable" path to the original CPython executable. */
    PySys_SetObject(
        (char *)"executable",
        const_str_digest_58961c73026e70b23b08cf381b49e04e
    );
#endif
}

// In debug mode we can check that the constants were not tampered with in any
// given moment. We typically do it at program exit, but we can add extra calls
// for sanity.
#ifndef __NUITKA_NO_ASSERT__
void checkGlobalConstants( void )
{

}
#endif

void createGlobalConstants( void )
{
    if ( _sentinel_value == NULL )
    {
#if PYTHON_VERSION < 300
        _sentinel_value = PyCObject_FromVoidPtr( NULL, NULL );
#else
        // The NULL value is not allowed for a capsule, so use something else.
        _sentinel_value = PyCapsule_New( (void *)27, "sentinel", NULL );
#endif
        assert( _sentinel_value );

        _createGlobalConstants();
    }
}
