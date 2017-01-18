#include <Python.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "wav_header.h"
#include "audio.h"

#define SAMPLES_BUF 128              /*buffer length in samples*/
#define ERROR_STRING_MAX 128

static PyObject *ExceptionBase;
static PyObject *FileNotFoundError;
static PyObject *HeadersMismatchError;

static PyObject*
cancellate(PyObject* self, PyObject* args)
{
    FILE *fileA, *fileB;
    const char* fileA_name;
    const char* fileB_name;
    char error_string[ERROR_STRING_MAX];

    struct wavheader headerA;
    struct wavheader headerB;
    double res = 0.0, diff = 1.0;
    int header_check = 1, header_compare = 1, isfail = -1;

    if (!PyArg_ParseTuple(args, "ss", &fileA_name, &fileB_name))
        return NULL;

    if(NULL == (fileA = fopen(fileA_name, "rb"))) {
        PyErr_Format(FileNotFoundError, "cannot open file at %s", fileA_name);
        return NULL;
    }

    if(NULL == (fileB = fopen(fileB_name, "rb"))) {
        PyErr_Format(FileNotFoundError, "cannot open file at %s", fileB_name);
        return NULL;
    }

    if(!readheader(fileA, &headerA) && !readheader(fileB, &headerB)) {
        header_check = 0;
        header_compare = compare_headers_(&headerA, &headerB);
        if (0 == header_compare)
        {
            PyErr_Format(HeadersMismatchError, "");
        }
        if (0 == header_compare) {
            diff = cancellation(fileA, fileB, SAMPLES_BUF, headerA.numChannels, headerA.bitsPerSample / 8, headerA.subchunk2Size);
            if(0 == diff) {
                res = -999.0;
                isfail = 0;
            }
            else if(-1.0 == diff)
                isfail = 1;
            else {
                isfail = 0;
                res = db_value(diff);
            }
        }
    }

    fclose(fileA);
    fclose(fileB);

    Py_BuildValue("diii", res, header_check, header_compare, isfail);
}

static PyMethodDef HelloMethods[] =
{
     {"compare_files", (PyCFunction)cancellate, METH_VARARGS, "compares files."},
     {NULL, NULL, 0, NULL}
};

static struct PyModuleDef wav_cancel_module = {
     PyModuleDef_HEAD_INIT,
     "wav_cancel",
     "compare_files",
     -1,
     HelloMethods
};


// PyMODINIT_FUNC
PyInit_wav_cancel(void)
{
    PyObject* module;

    module = PyModule_Create(&wav_cancel_module);

    ExceptionBase = PyErr_NewException("wav_cancel.WavCancelBaseError", NULL, NULL);
    FileNotFoundError = PyErr_NewException("wav_cancel.FileNotFoundError", NULL, NULL);
    HeadersMismatchError = PyErr_NewException("wav_cancel.HeadersMismatchError", NULL, NULL);

    PyModule_AddObject(module, "WavCancelBaseError", ExceptionBase);
    PyModule_AddObject(module, "FileNotFoundError", FileNotFoundError);
    PyModule_AddObject(module, "HeadersMismatchError", HeadersMismatchError);

    return module;

}
