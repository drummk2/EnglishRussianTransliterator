// This file contains helper functions that are automatically created from
// templates.

#include "nuitka/prelude.h"

extern PyObject *callPythonFunction( PyObject *func, PyObject **args, int count );


PyObject *CALL_FUNCTION_WITH_ARGS1( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 1; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 1 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 1; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 1 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 1 * sizeof(PyObject *) );
            memcpy( python_pars + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 1 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 1 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 1; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 1 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 1 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 1 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 1 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (1 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 1 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (1 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 1 );

            PyObject *result;

            assert( flags && METH_VARARGS );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags && METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 1, NULL );;
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            1
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 1 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS2( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 2; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 2 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 2; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 2 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 2 * sizeof(PyObject *) );
            memcpy( python_pars + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 2 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 2 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 2; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 2 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 2 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 2 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 2 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (2 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 2 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (2 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 2 );

            PyObject *result;

            assert( flags && METH_VARARGS );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags && METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 2, NULL );;
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            2
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 2 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS3( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 3; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 3 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 3; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 3 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 3 * sizeof(PyObject *) );
            memcpy( python_pars + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 3 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 3 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 3; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 3 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 3 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 3 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 3 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (3 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 3 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (3 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 3 );

            PyObject *result;

            assert( flags && METH_VARARGS );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags && METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 3, NULL );;
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            3
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 3 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_METHOD_WITH_ARGS1( PyObject *source, PyObject *attr_name, PyObject **args )
{
    CHECK_OBJECT( source );
    CHECK_OBJECT( attr_name );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 1; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    PyTypeObject *type = Py_TYPE( source );

    if ( type->tp_getattro == PyObject_GenericGetAttr )
    {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely( type->tp_dict == NULL ))
        {
            if (unlikely( PyType_Ready( type ) < 0 ))
            {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup( type, attr_name );
        descrgetfunc func = NULL;

        if ( descr != NULL )
        {
            Py_INCREF( descr );

#if PYTHON_VERSION < 300
            if ( PyType_HasFeature( Py_TYPE( descr ), Py_TPFLAGS_HAVE_CLASS ) )
            {
#endif
                func = Py_TYPE( descr )->tp_descr_get;

                if ( func != NULL && PyDescr_IsData( descr ) )
                {
                    PyObject *called_object = func( descr, source, (PyObject *)type );
                    Py_DECREF( descr );

                    PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                        called_object,
                        args
                    );
                    Py_DECREF( called_object );
                    return result;
                }
#if PYTHON_VERSION < 300
            }
#endif
        }

        Py_ssize_t dictoffset = type->tp_dictoffset;
        PyObject *dict = NULL;

        if ( dictoffset != 0 )
        {
            // Negative dictionary offsets have special meaning.
            if ( dictoffset < 0 )
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)source)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE( type, tsize );

                dictoffset += (long)size;
            }

            PyObject **dictptr = (PyObject **) ((char *)source + dictoffset);
            dict = *dictptr;
        }

        if ( dict != NULL )
        {
            CHECK_OBJECT( dict );

            Py_INCREF( dict );

            PyObject *called_object = PyDict_GetItem( dict, attr_name );

            if ( called_object != NULL )
            {
                Py_INCREF( called_object );
                Py_XDECREF( descr );
                Py_DECREF( dict );

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF( called_object );
                return result;
            }

            Py_DECREF( dict );
        }

        if ( func != NULL )
        {
            if ( func == Nuitka_Function_Type.tp_descr_get )
            {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    1
                );

                Py_DECREF( descr );

                return result;
            }
            else
            {
                PyObject *called_object = func( descr, source, (PyObject *)type );
                CHECK_OBJECT( called_object );

                Py_DECREF( descr );

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF( called_object );

                return result;
            }
        }

        if ( descr != NULL )
        {
            CHECK_OBJECT( descr );
            return CALL_FUNCTION_WITH_ARGS1(
                descr,
                args
            );
        }

#if PYTHON_VERSION < 300
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            PyString_AS_STRING( attr_name )
        );
#else
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%U'",
            type->tp_name,
            attr_name
        );
#endif
        return NULL;
    }
#if PYTHON_VERSION < 300
    else if ( type == &PyInstance_Type )
    {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert( attr_name != const_str_plain___dict__ );
        assert( attr_name != const_str_plain___class__ );

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            return CALL_FUNCTION_WITH_ARGS1( called_object, args );
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            descrgetfunc descr_get = Py_TYPE( called_object )->tp_descr_get;

            if ( descr_get == Nuitka_Function_Type.tp_descr_get )
            {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    1
                );
            }
            else
            {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely( method == NULL ))
                {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS1( method, args );
                Py_DECREF( method );
                return result;
            }
        }
        else if (unlikely( source_instance->in_class->cl_getattr == NULL ))
        {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        }
        else
        {
            // Finally allow the "__getattr__" override to provide it or else
            // it's an error.

            PyObject *args2[] = {
                source,
                attr_name
            };

            called_object = CALL_FUNCTION_WITH_ARGS2(
                source_instance->in_class->cl_getattr,
                args2
            );

            if (unlikely( called_object == NULL ))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                called_object,
                args
            );
            Py_DECREF( called_object );
            return result;
        }
    }
#endif
    else if ( type->tp_getattro != NULL )
    {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else if ( type->tp_getattr != NULL )
    {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            Nuitka_String_AsString_Unchecked( attr_name )
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else
    {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked( attr_name )
        );

        return NULL;
    }
}

PyObject *CALL_METHOD_WITH_ARGS2( PyObject *source, PyObject *attr_name, PyObject **args )
{
    CHECK_OBJECT( source );
    CHECK_OBJECT( attr_name );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 2; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    PyTypeObject *type = Py_TYPE( source );

    if ( type->tp_getattro == PyObject_GenericGetAttr )
    {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely( type->tp_dict == NULL ))
        {
            if (unlikely( PyType_Ready( type ) < 0 ))
            {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup( type, attr_name );
        descrgetfunc func = NULL;

        if ( descr != NULL )
        {
            Py_INCREF( descr );

#if PYTHON_VERSION < 300
            if ( PyType_HasFeature( Py_TYPE( descr ), Py_TPFLAGS_HAVE_CLASS ) )
            {
#endif
                func = Py_TYPE( descr )->tp_descr_get;

                if ( func != NULL && PyDescr_IsData( descr ) )
                {
                    PyObject *called_object = func( descr, source, (PyObject *)type );
                    Py_DECREF( descr );

                    PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                        called_object,
                        args
                    );
                    Py_DECREF( called_object );
                    return result;
                }
#if PYTHON_VERSION < 300
            }
#endif
        }

        Py_ssize_t dictoffset = type->tp_dictoffset;
        PyObject *dict = NULL;

        if ( dictoffset != 0 )
        {
            // Negative dictionary offsets have special meaning.
            if ( dictoffset < 0 )
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)source)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE( type, tsize );

                dictoffset += (long)size;
            }

            PyObject **dictptr = (PyObject **) ((char *)source + dictoffset);
            dict = *dictptr;
        }

        if ( dict != NULL )
        {
            CHECK_OBJECT( dict );

            Py_INCREF( dict );

            PyObject *called_object = PyDict_GetItem( dict, attr_name );

            if ( called_object != NULL )
            {
                Py_INCREF( called_object );
                Py_XDECREF( descr );
                Py_DECREF( dict );

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                    called_object,
                    args
                );
                Py_DECREF( called_object );
                return result;
            }

            Py_DECREF( dict );
        }

        if ( func != NULL )
        {
            if ( func == Nuitka_Function_Type.tp_descr_get )
            {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    2
                );

                Py_DECREF( descr );

                return result;
            }
            else
            {
                PyObject *called_object = func( descr, source, (PyObject *)type );
                CHECK_OBJECT( called_object );

                Py_DECREF( descr );

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                    called_object,
                    args
                );
                Py_DECREF( called_object );

                return result;
            }
        }

        if ( descr != NULL )
        {
            CHECK_OBJECT( descr );
            return CALL_FUNCTION_WITH_ARGS2(
                descr,
                args
            );
        }

#if PYTHON_VERSION < 300
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            PyString_AS_STRING( attr_name )
        );
#else
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%U'",
            type->tp_name,
            attr_name
        );
#endif
        return NULL;
    }
#if PYTHON_VERSION < 300
    else if ( type == &PyInstance_Type )
    {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert( attr_name != const_str_plain___dict__ );
        assert( attr_name != const_str_plain___class__ );

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            return CALL_FUNCTION_WITH_ARGS2( called_object, args );
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            descrgetfunc descr_get = Py_TYPE( called_object )->tp_descr_get;

            if ( descr_get == Nuitka_Function_Type.tp_descr_get )
            {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    2
                );
            }
            else
            {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely( method == NULL ))
                {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS2( method, args );
                Py_DECREF( method );
                return result;
            }
        }
        else if (unlikely( source_instance->in_class->cl_getattr == NULL ))
        {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        }
        else
        {
            // Finally allow the "__getattr__" override to provide it or else
            // it's an error.

            PyObject *args2[] = {
                source,
                attr_name
            };

            called_object = CALL_FUNCTION_WITH_ARGS2(
                source_instance->in_class->cl_getattr,
                args2
            );

            if (unlikely( called_object == NULL ))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                called_object,
                args
            );
            Py_DECREF( called_object );
            return result;
        }
    }
#endif
    else if ( type->tp_getattro != NULL )
    {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS2(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else if ( type->tp_getattr != NULL )
    {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            Nuitka_String_AsString_Unchecked( attr_name )
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS2(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else
    {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked( attr_name )
        );

        return NULL;
    }
}
/* Code to register embedded modules for meta path based loading if any. */

#include "nuitka/unfreezing.h"

/* Table for lookup to find compiled or bytecode modules included in this
 * binary or module, or put along this binary as extension modules. We do
 * our own loading for each of these.
 */
MOD_INIT_DECL( __main__ );
static struct Nuitka_MetaPathBasedLoaderEntry meta_path_loader_entries[] =
{
    { (char *)"__main__", MOD_INIT_NAME( __main__ ), 0, 0, NUITKA_COMPILED_MODULE },
    { (char *)"_bsddb", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_cn", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_hk", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_iso2022", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_jp", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_kr", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_codecs_tw", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_csv", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_ctypes", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_curses", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_curses_panel", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_elementtree", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_hashlib", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_hotshot", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_json", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_lsprof", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_multibytecodec", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_multiprocessing", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_sqlite3", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_ssl", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"_tkinter", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"bz2", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"dbm", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"mmap", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"parser", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"readline", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"resource", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"termios", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { (char *)"BaseHTTPServer", NULL, 1649, 21682, NUITKA_BYTECODE_FLAG },
    { (char *)"Bastion", NULL, 23331, 6622, NUITKA_BYTECODE_FLAG },
    { (char *)"CGIHTTPServer", NULL, 29953, 10984, NUITKA_BYTECODE_FLAG },
    { (char *)"Canvas", NULL, 40937, 15396, NUITKA_BYTECODE_FLAG },
    { (char *)"ConfigParser", NULL, 56333, 25087, NUITKA_BYTECODE_FLAG },
    { (char *)"Cookie", NULL, 81420, 22574, NUITKA_BYTECODE_FLAG },
    { (char *)"Dialog", NULL, 103994, 1902, NUITKA_BYTECODE_FLAG },
    { (char *)"DocXMLRPCServer", NULL, 105896, 9791, NUITKA_BYTECODE_FLAG },
    { (char *)"FileDialog", NULL, 115687, 9672, NUITKA_BYTECODE_FLAG },
    { (char *)"FixTk", NULL, 125359, 2078, NUITKA_BYTECODE_FLAG },
    { (char *)"HTMLParser", NULL, 127437, 13656, NUITKA_BYTECODE_FLAG },
    { (char *)"MimeWriter", NULL, 141093, 7338, NUITKA_BYTECODE_FLAG },
    { (char *)"Queue", NULL, 148431, 9360, NUITKA_BYTECODE_FLAG },
    { (char *)"ScrolledText", NULL, 157791, 2646, NUITKA_BYTECODE_FLAG },
    { (char *)"SimpleDialog", NULL, 160437, 4309, NUITKA_BYTECODE_FLAG },
    { (char *)"SimpleHTTPServer", NULL, 164746, 7980, NUITKA_BYTECODE_FLAG },
    { (char *)"SimpleXMLRPCServer", NULL, 172726, 22782, NUITKA_BYTECODE_FLAG },
    { (char *)"SocketServer", NULL, 195508, 23948, NUITKA_BYTECODE_FLAG },
    { (char *)"StringIO", NULL, 219456, 11434, NUITKA_BYTECODE_FLAG },
    { (char *)"Tix", NULL, 230890, 95379, NUITKA_BYTECODE_FLAG },
    { (char *)"Tkconstants", NULL, 326269, 2236, NUITKA_BYTECODE_FLAG },
    { (char *)"Tkdnd", NULL, 328505, 12765, NUITKA_BYTECODE_FLAG },
    { (char *)"Tkinter", NULL, 341270, 199053, NUITKA_BYTECODE_FLAG },
    { (char *)"UserDict", NULL, 540323, 9613, NUITKA_BYTECODE_FLAG },
    { (char *)"UserList", NULL, 549936, 6501, NUITKA_BYTECODE_FLAG },
    { (char *)"UserString", NULL, 556437, 14720, NUITKA_BYTECODE_FLAG },
    { (char *)"_LWPCookieJar", NULL, 571157, 5513, NUITKA_BYTECODE_FLAG },
    { (char *)"_MozillaCookieJar", NULL, 576670, 4445, NUITKA_BYTECODE_FLAG },
    { (char *)"__future__", NULL, 581115, 4272, NUITKA_BYTECODE_FLAG },
    { (char *)"_abcoll", NULL, 585387, 25466, NUITKA_BYTECODE_FLAG },
    { (char *)"_osx_support", NULL, 610853, 11752, NUITKA_BYTECODE_FLAG },
    { (char *)"_pyio", NULL, 622605, 64318, NUITKA_BYTECODE_FLAG },
    { (char *)"_strptime", NULL, 686923, 15121, NUITKA_BYTECODE_FLAG },
    { (char *)"_sysconfigdata", NULL, 702044, 271, NUITKA_BYTECODE_FLAG },
    { (char *)"_sysconfigdata_nd", NULL, 702315, 20800, NUITKA_BYTECODE_FLAG },
    { (char *)"_threading_local", NULL, 723115, 6578, NUITKA_BYTECODE_FLAG },
    { (char *)"_weakrefset", NULL, 729693, 9574, NUITKA_BYTECODE_FLAG },
    { (char *)"abc", NULL, 739267, 6113, NUITKA_BYTECODE_FLAG },
    { (char *)"aifc", NULL, 745380, 30327, NUITKA_BYTECODE_FLAG },
    { (char *)"anydbm", NULL, 775707, 2786, NUITKA_BYTECODE_FLAG },
    { (char *)"argparse", NULL, 778493, 64033, NUITKA_BYTECODE_FLAG },
    { (char *)"ast", NULL, 842526, 12898, NUITKA_BYTECODE_FLAG },
    { (char *)"asynchat", NULL, 855424, 8751, NUITKA_BYTECODE_FLAG },
    { (char *)"asyncore", NULL, 864175, 18714, NUITKA_BYTECODE_FLAG },
    { (char *)"atexit", NULL, 882889, 2183, NUITKA_BYTECODE_FLAG },
    { (char *)"audiodev", NULL, 885072, 8407, NUITKA_BYTECODE_FLAG },
    { (char *)"bdb", NULL, 893479, 18971, NUITKA_BYTECODE_FLAG },
    { (char *)"binhex", NULL, 912450, 15349, NUITKA_BYTECODE_FLAG },
    { (char *)"bisect", NULL, 927799, 3053, NUITKA_BYTECODE_FLAG },
    { (char *)"bsddb", NULL, 930852, 12368, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"bsddb.db", NULL, 943220, 582, NUITKA_BYTECODE_FLAG },
    { (char *)"bsddb.dbobj", NULL, 943802, 18601, NUITKA_BYTECODE_FLAG },
    { (char *)"bsddb.dbrecio", NULL, 962403, 5269, NUITKA_BYTECODE_FLAG },
    { (char *)"bsddb.dbshelve", NULL, 967672, 12910, NUITKA_BYTECODE_FLAG },
    { (char *)"bsddb.dbtables", NULL, 980582, 24402, NUITKA_BYTECODE_FLAG },
    { (char *)"bsddb.dbutils", NULL, 1004984, 1615, NUITKA_BYTECODE_FLAG },
    { (char *)"cProfile", NULL, 1006599, 6272, NUITKA_BYTECODE_FLAG },
    { (char *)"calendar", NULL, 1012871, 27624, NUITKA_BYTECODE_FLAG },
    { (char *)"cgi", NULL, 1040495, 32417, NUITKA_BYTECODE_FLAG },
    { (char *)"cgitb", NULL, 1072912, 12140, NUITKA_BYTECODE_FLAG },
    { (char *)"chunk", NULL, 1085052, 5571, NUITKA_BYTECODE_FLAG },
    { (char *)"cmd", NULL, 1090623, 13989, NUITKA_BYTECODE_FLAG },
    { (char *)"code", NULL, 1104612, 10294, NUITKA_BYTECODE_FLAG },
    { (char *)"codeop", NULL, 1114906, 6569, NUITKA_BYTECODE_FLAG },
    { (char *)"collections", NULL, 1121475, 26051, NUITKA_BYTECODE_FLAG },
    { (char *)"colorsys", NULL, 1147526, 3967, NUITKA_BYTECODE_FLAG },
    { (char *)"commands", NULL, 1151493, 2449, NUITKA_BYTECODE_FLAG },
    { (char *)"compileall", NULL, 1153942, 6997, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler", NULL, 1160939, 1287, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"compiler.ast", NULL, 1162226, 71339, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.consts", NULL, 1233565, 727, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.future", NULL, 1234292, 3018, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.misc", NULL, 1237310, 3687, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.pyassem", NULL, 1240997, 25831, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.pycodegen", NULL, 1266828, 56161, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.symbols", NULL, 1322989, 17557, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.syntax", NULL, 1340546, 1862, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.transformer", NULL, 1342408, 47372, NUITKA_BYTECODE_FLAG },
    { (char *)"compiler.visitor", NULL, 1389780, 4159, NUITKA_BYTECODE_FLAG },
    { (char *)"contextlib", NULL, 1393939, 4422, NUITKA_BYTECODE_FLAG },
    { (char *)"cookielib", NULL, 1398361, 54567, NUITKA_BYTECODE_FLAG },
    { (char *)"copy", NULL, 1452928, 12140, NUITKA_BYTECODE_FLAG },
    { (char *)"csv", NULL, 1465068, 13454, NUITKA_BYTECODE_FLAG },
    { (char *)"ctypes", NULL, 1478522, 20192, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"ctypes._endian", NULL, 1498714, 2287, NUITKA_BYTECODE_FLAG },
    { (char *)"ctypes.util", NULL, 1501001, 7868, NUITKA_BYTECODE_FLAG },
    { (char *)"curses", NULL, 1508869, 1539, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"curses.ascii", NULL, 1510408, 5040, NUITKA_BYTECODE_FLAG },
    { (char *)"curses.has_key", NULL, 1515448, 5925, NUITKA_BYTECODE_FLAG },
    { (char *)"curses.panel", NULL, 1521373, 267, NUITKA_BYTECODE_FLAG },
    { (char *)"curses.textpad", NULL, 1521640, 6842, NUITKA_BYTECODE_FLAG },
    { (char *)"curses.wrapper", NULL, 1528482, 1206, NUITKA_BYTECODE_FLAG },
    { (char *)"dbhash", NULL, 1529688, 706, NUITKA_BYTECODE_FLAG },
    { (char *)"decimal", NULL, 1530394, 170849, NUITKA_BYTECODE_FLAG },
    { (char *)"difflib", NULL, 1701243, 61833, NUITKA_BYTECODE_FLAG },
    { (char *)"dircache", NULL, 1763076, 1560, NUITKA_BYTECODE_FLAG },
    { (char *)"dis", NULL, 1764636, 6204, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils", NULL, 1770840, 405, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"distutils.archive_util", NULL, 1771245, 7432, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.bcppcompiler", NULL, 1778677, 7856, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.ccompiler", NULL, 1786533, 36748, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.cmd", NULL, 1823281, 16722, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command", NULL, 1840003, 655, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"distutils.command.bdist", NULL, 1840658, 5199, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.bdist_dumb", NULL, 1845857, 5025, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.bdist_msi", NULL, 1850882, 23971, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.bdist_rpm", NULL, 1874853, 17724, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.bdist_wininst", NULL, 1892577, 10950, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.build", NULL, 1903527, 5125, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.build_clib", NULL, 1908652, 6405, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.build_ext", NULL, 1915057, 19417, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.build_py", NULL, 1934474, 11483, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.build_scripts", NULL, 1945957, 4512, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.check", NULL, 1950469, 6236, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.clean", NULL, 1956705, 3158, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.config", NULL, 1959863, 12642, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install", NULL, 1972505, 17971, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install_data", NULL, 1990476, 3142, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install_egg_info", NULL, 1993618, 4421, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install_headers", NULL, 1998039, 2274, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install_lib", NULL, 2000313, 6752, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.install_scripts", NULL, 2007065, 2976, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.register", NULL, 2010041, 10188, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.sdist", NULL, 2020229, 16657, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.command.upload", NULL, 2036886, 6308, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.config", NULL, 2043194, 3548, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.core", NULL, 2046742, 7634, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.cygwinccompiler", NULL, 2054376, 9793, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.debug", NULL, 2064169, 244, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.dep_util", NULL, 2064413, 3164, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.dir_util", NULL, 2067577, 6764, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.dist", NULL, 2074341, 39429, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.emxccompiler", NULL, 2113770, 7441, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.errors", NULL, 2121211, 6237, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.extension", NULL, 2127448, 7396, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.fancy_getopt", NULL, 2134844, 11908, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.file_util", NULL, 2146752, 6732, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.filelist", NULL, 2153484, 10714, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.log", NULL, 2164198, 2754, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.msvc9compiler", NULL, 2166952, 21439, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.msvccompiler", NULL, 2188391, 17465, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.spawn", NULL, 2205856, 6377, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.sysconfig", NULL, 2212233, 15100, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.text_file", NULL, 2227333, 9229, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.unixccompiler", NULL, 2236562, 8028, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.util", NULL, 2244590, 14315, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.version", NULL, 2258905, 7170, NUITKA_BYTECODE_FLAG },
    { (char *)"distutils.versionpredicate", NULL, 2266075, 5520, NUITKA_BYTECODE_FLAG },
    { (char *)"doctest", NULL, 2271595, 83440, NUITKA_BYTECODE_FLAG },
    { (char *)"dumbdbm", NULL, 2355035, 6538, NUITKA_BYTECODE_FLAG },
    { (char *)"dummy_thread", NULL, 2361573, 5356, NUITKA_BYTECODE_FLAG },
    { (char *)"dummy_threading", NULL, 2366929, 1275, NUITKA_BYTECODE_FLAG },
    { (char *)"email", NULL, 2368204, 2852, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"email._parseaddr", NULL, 2371056, 13763, NUITKA_BYTECODE_FLAG },
    { (char *)"email.base64mime", NULL, 2384819, 5305, NUITKA_BYTECODE_FLAG },
    { (char *)"email.charset", NULL, 2390124, 13499, NUITKA_BYTECODE_FLAG },
    { (char *)"email.encoders", NULL, 2403623, 2210, NUITKA_BYTECODE_FLAG },
    { (char *)"email.errors", NULL, 2405833, 3491, NUITKA_BYTECODE_FLAG },
    { (char *)"email.feedparser", NULL, 2409324, 11516, NUITKA_BYTECODE_FLAG },
    { (char *)"email.generator", NULL, 2420840, 10334, NUITKA_BYTECODE_FLAG },
    { (char *)"email.header", NULL, 2431174, 13622, NUITKA_BYTECODE_FLAG },
    { (char *)"email.iterators", NULL, 2444796, 2348, NUITKA_BYTECODE_FLAG },
    { (char *)"email.message", NULL, 2447144, 28576, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime", NULL, 2475720, 120, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"email.mime.application", NULL, 2475840, 1570, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.audio", NULL, 2477410, 2893, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.base", NULL, 2480303, 1102, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.image", NULL, 2481405, 2035, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.message", NULL, 2483440, 1434, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.multipart", NULL, 2484874, 1655, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.nonmultipart", NULL, 2486529, 874, NUITKA_BYTECODE_FLAG },
    { (char *)"email.mime.text", NULL, 2487403, 1294, NUITKA_BYTECODE_FLAG },
    { (char *)"email.parser", NULL, 2488697, 3804, NUITKA_BYTECODE_FLAG },
    { (char *)"email.quoprimime", NULL, 2492501, 8816, NUITKA_BYTECODE_FLAG },
    { (char *)"email.utils", NULL, 2501317, 9082, NUITKA_BYTECODE_FLAG },
    { (char *)"encodings.mbcs", NULL, 2510399, 2019, NUITKA_BYTECODE_FLAG },
    { (char *)"filecmp", NULL, 2512418, 9574, NUITKA_BYTECODE_FLAG },
    { (char *)"fileinput", NULL, 2521992, 14436, NUITKA_BYTECODE_FLAG },
    { (char *)"fnmatch", NULL, 2536428, 3594, NUITKA_BYTECODE_FLAG },
    { (char *)"formatter", NULL, 2540022, 19016, NUITKA_BYTECODE_FLAG },
    { (char *)"fpformat", NULL, 2559038, 4648, NUITKA_BYTECODE_FLAG },
    { (char *)"fractions", NULL, 2563686, 19647, NUITKA_BYTECODE_FLAG },
    { (char *)"ftplib", NULL, 2583333, 34434, NUITKA_BYTECODE_FLAG },
    { (char *)"genericpath", NULL, 2617767, 3487, NUITKA_BYTECODE_FLAG },
    { (char *)"getopt", NULL, 2621254, 6626, NUITKA_BYTECODE_FLAG },
    { (char *)"getpass", NULL, 2627880, 4722, NUITKA_BYTECODE_FLAG },
    { (char *)"gettext", NULL, 2632602, 15628, NUITKA_BYTECODE_FLAG },
    { (char *)"glob", NULL, 2648230, 2919, NUITKA_BYTECODE_FLAG },
    { (char *)"gzip", NULL, 2651149, 15126, NUITKA_BYTECODE_FLAG },
    { (char *)"hashlib", NULL, 2666275, 7041, NUITKA_BYTECODE_FLAG },
    { (char *)"heapq", NULL, 2673316, 14520, NUITKA_BYTECODE_FLAG },
    { (char *)"hmac", NULL, 2687836, 4514, NUITKA_BYTECODE_FLAG },
    { (char *)"hotshot", NULL, 2692350, 3454, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"hotshot.log", NULL, 2695804, 5507, NUITKA_BYTECODE_FLAG },
    { (char *)"hotshot.stats", NULL, 2701311, 3374, NUITKA_BYTECODE_FLAG },
    { (char *)"hotshot.stones", NULL, 2704685, 1144, NUITKA_BYTECODE_FLAG },
    { (char *)"htmlentitydefs", NULL, 2705829, 6357, NUITKA_BYTECODE_FLAG },
    { (char *)"htmllib", NULL, 2712186, 20119, NUITKA_BYTECODE_FLAG },
    { (char *)"httplib", NULL, 2732305, 36745, NUITKA_BYTECODE_FLAG },
    { (char *)"ihooks", NULL, 2769050, 21226, NUITKA_BYTECODE_FLAG },
    { (char *)"imaplib", NULL, 2790276, 45155, NUITKA_BYTECODE_FLAG },
    { (char *)"imghdr", NULL, 2835431, 4798, NUITKA_BYTECODE_FLAG },
    { (char *)"importlib", NULL, 2840229, 1488, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"imputil", NULL, 2841717, 15539, NUITKA_BYTECODE_FLAG },
    { (char *)"inspect", NULL, 2857256, 39936, NUITKA_BYTECODE_FLAG },
    { (char *)"io", NULL, 2897192, 3570, NUITKA_BYTECODE_FLAG },
    { (char *)"json", NULL, 2900762, 13937, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"json.decoder", NULL, 2914699, 11928, NUITKA_BYTECODE_FLAG },
    { (char *)"json.encoder", NULL, 2926627, 13668, NUITKA_BYTECODE_FLAG },
    { (char *)"json.scanner", NULL, 2940295, 2215, NUITKA_BYTECODE_FLAG },
    { (char *)"json.tool", NULL, 2942510, 1282, NUITKA_BYTECODE_FLAG },
    { (char *)"keyword", NULL, 2943792, 2093, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3", NULL, 2945885, 117, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"lib2to3.btm_matcher", NULL, 2946002, 5800, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.btm_utils", NULL, 2951802, 7529, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixer_base", NULL, 2959331, 7216, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixer_util", NULL, 2966547, 14607, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes", NULL, 2981154, 123, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"lib2to3.fixes.fix_apply", NULL, 2981277, 1869, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_asserts", NULL, 2983146, 1547, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_basestring", NULL, 2984693, 793, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_buffer", NULL, 2985486, 950, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_callable", NULL, 2986436, 1493, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_dict", NULL, 2987929, 3753, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_except", NULL, 2991682, 2993, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_exec", NULL, 2994675, 1418, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_execfile", NULL, 2996093, 2059, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_exitfunc", NULL, 2998152, 2739, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_filter", NULL, 3000891, 2256, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_funcattrs", NULL, 3003147, 1114, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_future", NULL, 3004261, 919, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_getcwdu", NULL, 3005180, 926, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_has_key", NULL, 3006106, 3184, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_idioms", NULL, 3009290, 4515, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_import", NULL, 3013805, 3233, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_imports", NULL, 3017038, 5352, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_imports2", NULL, 3022390, 622, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_input", NULL, 3023012, 1134, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_intern", NULL, 3024146, 1605, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_isinstance", NULL, 3025751, 1838, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_itertools", NULL, 3027589, 1791, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_itertools_imports", NULL, 3029380, 2016, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_long", NULL, 3031396, 841, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_map", NULL, 3032237, 3040, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_metaclass", NULL, 3035277, 6579, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_methodattrs", NULL, 3041856, 1138, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_ne", NULL, 3042994, 985, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_next", NULL, 3043979, 3531, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_nonzero", NULL, 3047510, 1086, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_numliterals", NULL, 3048596, 1249, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_operator", NULL, 3049845, 5112, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_paren", NULL, 3054957, 1543, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_print", NULL, 3056500, 2727, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_raise", NULL, 3059227, 2498, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_raw_input", NULL, 3061725, 936, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_reduce", NULL, 3062661, 1262, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_renames", NULL, 3063923, 2449, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_repr", NULL, 3066372, 1016, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_set_literal", NULL, 3067388, 1988, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_standarderror", NULL, 3069376, 853, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_sys_exc", NULL, 3070229, 1705, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_throw", NULL, 3071934, 1996, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_tuple_params", NULL, 3073930, 5430, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_types", NULL, 3079360, 2196, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_unicode", NULL, 3081556, 1716, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_urllib", NULL, 3083272, 7134, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_ws_comma", NULL, 3090406, 1382, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_xrange", NULL, 3091788, 3063, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_xreadlines", NULL, 3094851, 1152, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.fixes.fix_zip", NULL, 3096003, 1346, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.main", NULL, 3097349, 9803, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.patcomp", NULL, 3107152, 6634, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2", NULL, 3113786, 164, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"lib2to3.pgen2.conv", NULL, 3113950, 8169, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.driver", NULL, 3122119, 5363, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.grammar", NULL, 3127482, 6004, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.literals", NULL, 3133486, 2002, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.parse", NULL, 3135488, 7204, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.pgen", NULL, 3142692, 12101, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.token", NULL, 3154793, 2287, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pgen2.tokenize", NULL, 3157080, 16923, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pygram", NULL, 3174003, 1400, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.pytree", NULL, 3175403, 30143, NUITKA_BYTECODE_FLAG },
    { (char *)"lib2to3.refactor", NULL, 3205546, 23866, NUITKA_BYTECODE_FLAG },
    { (char *)"linecache", NULL, 3229412, 3252, NUITKA_BYTECODE_FLAG },
    { (char *)"logging", NULL, 3232664, 57300, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"logging.config", NULL, 3289964, 25834, NUITKA_BYTECODE_FLAG },
    { (char *)"logging.handlers", NULL, 3315798, 39156, NUITKA_BYTECODE_FLAG },
    { (char *)"macpath", NULL, 3354954, 7635, NUITKA_BYTECODE_FLAG },
    { (char *)"macurl2path", NULL, 3362589, 2228, NUITKA_BYTECODE_FLAG },
    { (char *)"mailbox", NULL, 3364817, 76279, NUITKA_BYTECODE_FLAG },
    { (char *)"mailcap", NULL, 3441096, 7045, NUITKA_BYTECODE_FLAG },
    { (char *)"markupbase", NULL, 3448141, 9281, NUITKA_BYTECODE_FLAG },
    { (char *)"md5", NULL, 3457422, 368, NUITKA_BYTECODE_FLAG },
    { (char *)"mhlib", NULL, 3457790, 33653, NUITKA_BYTECODE_FLAG },
    { (char *)"mimetools", NULL, 3491443, 8176, NUITKA_BYTECODE_FLAG },
    { (char *)"mimetypes", NULL, 3499619, 18385, NUITKA_BYTECODE_FLAG },
    { (char *)"mimify", NULL, 3518004, 11963, NUITKA_BYTECODE_FLAG },
    { (char *)"modulefinder", NULL, 3529967, 19074, NUITKA_BYTECODE_FLAG },
    { (char *)"multifile", NULL, 3549041, 5382, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing", NULL, 3554423, 8405, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"multiprocessing.connection", NULL, 3562828, 14354, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.dummy", NULL, 3577182, 5412, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"multiprocessing.dummy.connection", NULL, 3582594, 2732, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.forking", NULL, 3585326, 14336, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.heap", NULL, 3599662, 6861, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.managers", NULL, 3606523, 38278, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.pool", NULL, 3644801, 22367, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.process", NULL, 3667168, 9435, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.queues", NULL, 3676603, 11466, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.reduction", NULL, 3688069, 5972, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.sharedctypes", NULL, 3694041, 8572, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.synchronize", NULL, 3702613, 10988, NUITKA_BYTECODE_FLAG },
    { (char *)"multiprocessing.util", NULL, 3713601, 10066, NUITKA_BYTECODE_FLAG },
    { (char *)"mutex", NULL, 3723667, 2494, NUITKA_BYTECODE_FLAG },
    { (char *)"netrc", NULL, 3726161, 4646, NUITKA_BYTECODE_FLAG },
    { (char *)"new", NULL, 3730807, 852, NUITKA_BYTECODE_FLAG },
    { (char *)"nntplib", NULL, 3731659, 20948, NUITKA_BYTECODE_FLAG },
    { (char *)"ntpath", NULL, 3752607, 13077, NUITKA_BYTECODE_FLAG },
    { (char *)"nturl2path", NULL, 3765684, 1801, NUITKA_BYTECODE_FLAG },
    { (char *)"numbers", NULL, 3767485, 13880, NUITKA_BYTECODE_FLAG },
    { (char *)"opcode", NULL, 3781365, 6134, NUITKA_BYTECODE_FLAG },
    { (char *)"optparse", NULL, 3787499, 53841, NUITKA_BYTECODE_FLAG },
    { (char *)"os", NULL, 3841340, 25585, NUITKA_BYTECODE_FLAG },
    { (char *)"os2emxpath", NULL, 3866925, 4499, NUITKA_BYTECODE_FLAG },
    { (char *)"pdb", NULL, 3871424, 43433, NUITKA_BYTECODE_FLAG },
    { (char *)"pickle", NULL, 3914857, 38342, NUITKA_BYTECODE_FLAG },
    { (char *)"pickletools", NULL, 3953199, 57072, NUITKA_BYTECODE_FLAG },
    { (char *)"pipes", NULL, 4010271, 9268, NUITKA_BYTECODE_FLAG },
    { (char *)"pkgutil", NULL, 4019539, 18859, NUITKA_BYTECODE_FLAG },
    { (char *)"platform", NULL, 4038398, 37615, NUITKA_BYTECODE_FLAG },
    { (char *)"plistlib", NULL, 4076013, 19143, NUITKA_BYTECODE_FLAG },
    { (char *)"popen2", NULL, 4095156, 8985, NUITKA_BYTECODE_FLAG },
    { (char *)"poplib", NULL, 4104141, 13270, NUITKA_BYTECODE_FLAG },
    { (char *)"posixfile", NULL, 4117411, 7620, NUITKA_BYTECODE_FLAG },
    { (char *)"posixpath", NULL, 4125031, 11366, NUITKA_BYTECODE_FLAG },
    { (char *)"pprint", NULL, 4136397, 10148, NUITKA_BYTECODE_FLAG },
    { (char *)"profile", NULL, 4146545, 16372, NUITKA_BYTECODE_FLAG },
    { (char *)"pstats", NULL, 4162917, 24885, NUITKA_BYTECODE_FLAG },
    { (char *)"pty", NULL, 4187802, 4938, NUITKA_BYTECODE_FLAG },
    { (char *)"py_compile", NULL, 4192740, 6603, NUITKA_BYTECODE_FLAG },
    { (char *)"pyclbr", NULL, 4199343, 9617, NUITKA_BYTECODE_FLAG },
    { (char *)"pydoc", NULL, 4208960, 92288, NUITKA_BYTECODE_FLAG },
    { (char *)"pydoc_data", NULL, 4301248, 120, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"pydoc_data.topics", NULL, 4301368, 409111, NUITKA_BYTECODE_FLAG },
    { (char *)"random", NULL, 4710479, 25476, NUITKA_BYTECODE_FLAG },
    { (char *)"repr", NULL, 4735955, 5343, NUITKA_BYTECODE_FLAG },
    { (char *)"rexec", NULL, 4741298, 24068, NUITKA_BYTECODE_FLAG },
    { (char *)"rfc822", NULL, 4765366, 31721, NUITKA_BYTECODE_FLAG },
    { (char *)"rlcompleter", NULL, 4797087, 6054, NUITKA_BYTECODE_FLAG },
    { (char *)"robotparser", NULL, 4803141, 7904, NUITKA_BYTECODE_FLAG },
    { (char *)"runpy", NULL, 4811045, 8780, NUITKA_BYTECODE_FLAG },
    { (char *)"sched", NULL, 4819825, 4968, NUITKA_BYTECODE_FLAG },
    { (char *)"sets", NULL, 4824793, 16775, NUITKA_BYTECODE_FLAG },
    { (char *)"sgmllib", NULL, 4841568, 15334, NUITKA_BYTECODE_FLAG },
    { (char *)"sha", NULL, 4856902, 411, NUITKA_BYTECODE_FLAG },
    { (char *)"shelve", NULL, 4857313, 10194, NUITKA_BYTECODE_FLAG },
    { (char *)"shlex", NULL, 4867507, 7509, NUITKA_BYTECODE_FLAG },
    { (char *)"shutil", NULL, 4875016, 18547, NUITKA_BYTECODE_FLAG },
    { (char *)"site", NULL, 4893563, 17118, NUITKA_BYTECODE_FLAG },
    { (char *)"sitecustomize", NULL, 4910681, 224, NUITKA_BYTECODE_FLAG },
    { (char *)"smtpd", NULL, 4910905, 15826, NUITKA_BYTECODE_FLAG },
    { (char *)"smtplib", NULL, 4926731, 30169, NUITKA_BYTECODE_FLAG },
    { (char *)"sndhdr", NULL, 4956900, 7315, NUITKA_BYTECODE_FLAG },
    { (char *)"socket", NULL, 4964215, 16084, NUITKA_BYTECODE_FLAG },
    { (char *)"sqlite3", NULL, 4980299, 154, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"sqlite3.dbapi2", NULL, 4980453, 2682, NUITKA_BYTECODE_FLAG },
    { (char *)"sqlite3.dump", NULL, 4983135, 2057, NUITKA_BYTECODE_FLAG },
    { (char *)"sre", NULL, 4985192, 509, NUITKA_BYTECODE_FLAG },
    { (char *)"ssl", NULL, 4985701, 32205, NUITKA_BYTECODE_FLAG },
    { (char *)"stat", NULL, 5017906, 2723, NUITKA_BYTECODE_FLAG },
    { (char *)"statvfs", NULL, 5020629, 610, NUITKA_BYTECODE_FLAG },
    { (char *)"stringold", NULL, 5021239, 12487, NUITKA_BYTECODE_FLAG },
    { (char *)"subprocess", NULL, 5033726, 42004, NUITKA_BYTECODE_FLAG },
    { (char *)"sunau", NULL, 5075730, 18301, NUITKA_BYTECODE_FLAG },
    { (char *)"sunaudio", NULL, 5094031, 1969, NUITKA_BYTECODE_FLAG },
    { (char *)"symbol", NULL, 5096000, 3014, NUITKA_BYTECODE_FLAG },
    { (char *)"symtable", NULL, 5099014, 11678, NUITKA_BYTECODE_FLAG },
    { (char *)"sysconfig", NULL, 5110692, 18670, NUITKA_BYTECODE_FLAG },
    { (char *)"tabnanny", NULL, 5129362, 8195, NUITKA_BYTECODE_FLAG },
    { (char *)"tarfile", NULL, 5137557, 75638, NUITKA_BYTECODE_FLAG },
    { (char *)"telnetlib", NULL, 5213195, 23073, NUITKA_BYTECODE_FLAG },
    { (char *)"tempfile", NULL, 5236268, 20241, NUITKA_BYTECODE_FLAG },
    { (char *)"test", NULL, 5256509, 114, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"test.pystone", NULL, 5256623, 7981, NUITKA_BYTECODE_FLAG },
    { (char *)"textwrap", NULL, 5264604, 11996, NUITKA_BYTECODE_FLAG },
    { (char *)"this", NULL, 5276600, 1210, NUITKA_BYTECODE_FLAG },
    { (char *)"threading", NULL, 5277810, 42422, NUITKA_BYTECODE_FLAG },
    { (char *)"timeit", NULL, 5320232, 12149, NUITKA_BYTECODE_FLAG },
    { (char *)"tkColorChooser", NULL, 5332381, 1402, NUITKA_BYTECODE_FLAG },
    { (char *)"tkCommonDialog", NULL, 5333783, 1496, NUITKA_BYTECODE_FLAG },
    { (char *)"tkFileDialog", NULL, 5335279, 5121, NUITKA_BYTECODE_FLAG },
    { (char *)"tkFont", NULL, 5340400, 7076, NUITKA_BYTECODE_FLAG },
    { (char *)"tkMessageBox", NULL, 5347476, 3864, NUITKA_BYTECODE_FLAG },
    { (char *)"tkSimpleDialog", NULL, 5351340, 9053, NUITKA_BYTECODE_FLAG },
    { (char *)"toaiff", NULL, 5360393, 3090, NUITKA_BYTECODE_FLAG },
    { (char *)"token", NULL, 5363483, 3798, NUITKA_BYTECODE_FLAG },
    { (char *)"tokenize", NULL, 5367281, 14465, NUITKA_BYTECODE_FLAG },
    { (char *)"trace", NULL, 5381746, 22716, NUITKA_BYTECODE_FLAG },
    { (char *)"traceback", NULL, 5404462, 11631, NUITKA_BYTECODE_FLAG },
    { (char *)"ttk", NULL, 5416093, 62193, NUITKA_BYTECODE_FLAG },
    { (char *)"tty", NULL, 5478286, 1303, NUITKA_BYTECODE_FLAG },
    { (char *)"turtle", NULL, 5479589, 139008, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest", NULL, 5618597, 2954, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"unittest.case", NULL, 5621551, 40172, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.loader", NULL, 5661723, 11299, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.main", NULL, 5673022, 7985, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.result", NULL, 5681007, 7869, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.runner", NULL, 5688876, 7599, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.signals", NULL, 5696475, 2738, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.suite", NULL, 5699213, 10460, NUITKA_BYTECODE_FLAG },
    { (char *)"unittest.util", NULL, 5709673, 4494, NUITKA_BYTECODE_FLAG },
    { (char *)"urllib", NULL, 5714167, 50856, NUITKA_BYTECODE_FLAG },
    { (char *)"urllib2", NULL, 5765023, 47064, NUITKA_BYTECODE_FLAG },
    { (char *)"urlparse", NULL, 5812087, 14457, NUITKA_BYTECODE_FLAG },
    { (char *)"user", NULL, 5826544, 1714, NUITKA_BYTECODE_FLAG },
    { (char *)"uu", NULL, 5828258, 4294, NUITKA_BYTECODE_FLAG },
    { (char *)"uuid", NULL, 5832552, 22469, NUITKA_BYTECODE_FLAG },
    { (char *)"warnings", NULL, 5855021, 13570, NUITKA_BYTECODE_FLAG },
    { (char *)"wave", NULL, 5868591, 19911, NUITKA_BYTECODE_FLAG },
    { (char *)"weakref", NULL, 5888502, 15733, NUITKA_BYTECODE_FLAG },
    { (char *)"webbrowser", NULL, 5904235, 19681, NUITKA_BYTECODE_FLAG },
    { (char *)"whichdb", NULL, 5923916, 2229, NUITKA_BYTECODE_FLAG },
    { (char *)"wsgiref", NULL, 5926145, 719, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"wsgiref.handlers", NULL, 5926864, 16172, NUITKA_BYTECODE_FLAG },
    { (char *)"wsgiref.headers", NULL, 5943036, 7429, NUITKA_BYTECODE_FLAG },
    { (char *)"wsgiref.simple_server", NULL, 5950465, 6199, NUITKA_BYTECODE_FLAG },
    { (char *)"wsgiref.util", NULL, 5956664, 5961, NUITKA_BYTECODE_FLAG },
    { (char *)"wsgiref.validate", NULL, 5962625, 16786, NUITKA_BYTECODE_FLAG },
    { (char *)"xdrlib", NULL, 5979411, 9810, NUITKA_BYTECODE_FLAG },
    { (char *)"xml", NULL, 5989221, 1068, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"xml.dom", NULL, 5990289, 6427, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"xml.dom.NodeFilter", NULL, 5996716, 1112, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.domreg", NULL, 5997828, 3293, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.expatbuilder", NULL, 6001121, 33003, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.minicompat", NULL, 6034124, 3394, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.minidom", NULL, 6037518, 65227, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.pulldom", NULL, 6102745, 12986, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.dom.xmlbuilder", NULL, 6115731, 16396, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.etree", NULL, 6132127, 119, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"xml.etree.ElementInclude", NULL, 6132246, 1951, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.etree.ElementPath", NULL, 6134197, 7560, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.etree.ElementTree", NULL, 6141757, 34794, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.etree.cElementTree", NULL, 6176551, 166, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.parsers", NULL, 6176717, 304, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"xml.parsers.expat", NULL, 6177021, 277, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.sax", NULL, 6177298, 3679, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { (char *)"xml.sax._exceptions", NULL, 6180977, 6127, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.sax.expatreader", NULL, 6187104, 14701, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.sax.handler", NULL, 6201805, 12970, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.sax.saxutils", NULL, 6214775, 14685, NUITKA_BYTECODE_FLAG },
    { (char *)"xml.sax.xmlreader", NULL, 6229460, 19078, NUITKA_BYTECODE_FLAG },
    { (char *)"xmllib", NULL, 6248538, 26732, NUITKA_BYTECODE_FLAG },
    { (char *)"xmlrpclib", NULL, 6275270, 43832, NUITKA_BYTECODE_FLAG },
    { (char *)"zipfile", NULL, 6319102, 41594, NUITKA_BYTECODE_FLAG },
    { NULL, NULL, 0, 0, 0 }
};

void setupMetaPathBasedLoader( void )
{
    static bool init_done = false;

    if ( init_done == false )
    {
        registerMetaPathBasedUnfreezer( meta_path_loader_entries );
        init_done = true;
    }
}
