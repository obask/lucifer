/********************************************************************
 * A simple C extension module for Python, called "hello"; compile
 * this into a ".so" on python path, import and call hello.message;
 ********************************************************************/

#include <Python.h>
#include <stdio.h>
#include "wrap.h"

/* module functions */
static PyObject *                                 /* returns object */
message(PyObject *self, PyObject *args)           /* self unused in modules */
{                                                 /* args from Python call */
    Py_buffer fromPython;
    char result[1024];
//    printf("OLOLO: %d\n", plus(1,3));
    if (! PyArg_Parse(args, "(y*)", &fromPython))  /* convert Python -> C */
        return NULL;                              /* null=raise exception */
    else {
        printf("parsed buf: %p\n", fromPython.buf);
        printf("parsed len: %zu\n", fromPython.len);
        scanf("%s", result);
        PyBuffer_Release(&fromPython);
//        fromPython = NULL;
        strcpy(result, "Hello, ");                /* build up C string */
        result[6] = '\0';
        // strcat(result, fromPython);               /* add passed Python string */
        return Py_BuildValue("s", result);        /* convert C -> Python */
    }
}


static PyObject *                                 
saveIndex(PyObject *self, PyObject *args)           
{                                                 
    char *fromPython;
    if (! PyArg_Parse(args, "(s)", &fromPython))  /* convert Python -> C */
        return NULL;                              /* null=raise exception */
    else {
        int res = apiSaveIndex(fromPython);
        return Py_BuildValue("i", res);        /* convert C -> Python */
    }
}


static PyObject *                                 
loadIndex(PyObject *self, PyObject *args)           
{                                                 
    char *fromPython;
    if (! PyArg_Parse(args, "(s)", &fromPython))  /* convert Python -> C */
        return NULL;                              /* null=raise exception */
    else {
        int res = apiLoadIndex(fromPython);
        return Py_BuildValue("i", res);        /* convert C -> Python */
    }
}


static PyObject *
addDocument(PyObject *self, PyObject *args)           
{                                                 
    char *fromPython;
    char *_id;
    if (! PyArg_Parse(args, "(ss)", &_id, &fromPython))
        return NULL;
    // else
    int res = apiAddDocument(_id, fromPython);
    return Py_BuildValue("i", res);        /* convert C -> Python */
}


static PyObject *                                 
searchQuery(PyObject *self, PyObject *args)           
{                                                 
    char *fromPython;
    int f1;
    int f2;
    if (! PyArg_Parse(args, "(sii)", &fromPython, &f1, &f2))
        return NULL;
    //else
    const char *res = apiSearchQuery(fromPython, f1, f2);
    return Py_BuildValue("s", res);
}

static PyObject *                                 
showIndex(PyObject *self, PyObject *args)           
{                                                 
    char *fromPython;
    if (! PyArg_Parse(args, "(s)", &fromPython))
        return NULL;
    else {
        int res = apiShowIndex(fromPython);
        return Py_BuildValue("i", res);
    }
}


/* registration table  */
static PyMethodDef lucifer_methods[] = {
    {"message",  message, METH_VARARGS, "func doc"},    /* name, &func, fmt, doc */
    {"saveIndex",  saveIndex, METH_VARARGS, "func doc"},    /* name, &func, fmt, doc */
    {"loadIndex",  loadIndex, METH_VARARGS, "func doc"},
    {"addDocument",  addDocument, METH_VARARGS, "func doc"},
    {"searchQuery",  searchQuery, METH_VARARGS, "args: query, f_union, f_ranking"}, 
    {"showIndex",  showIndex, METH_VARARGS, "func doc"},    /* name, &func, fmt, doc */
    {NULL, NULL, 0, NULL}                               /* end of table marker */
};

/* module definition structure */
static struct PyModuleDef lucifermodule = {
   PyModuleDef_HEAD_INIT,
   "hello",         /* name of module */
   "mod doc",       /* module documentation, may be NULL */
   -1,              /* size of per-interpreter module state, -1=in global vars */
   lucifer_methods    /* link to methods table */
};

/* module initializer */
PyMODINIT_FUNC
PyInit_lucifer()                         /* called on first import */
{                                      /* name matters if loaded dynamically */
    return PyModule_Create(&lucifermodule);
}










